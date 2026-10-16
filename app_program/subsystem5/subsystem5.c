/*
 *	subsystem5.c
 *	Buddy 1 application-side queues for MQTT telemetry and commands.
 *
 *	This module owns only the T-Kernel message buffers. The existing
 *	cyw43_utk_task() remains the sole owner of CYW43, lwIP and MQTT calls.
 */

#include "subsystem5.h"

#define BUDDY1_MBF_ATTR		TA_TFIFO

LOCAL ID	mbf_telemetry;
LOCAL ID	mbf_commands;

EXPORT ER subsystem5_init(void)
{
	T_CMBF	cmbf = {0};

	cmbf.mbfatr = BUDDY1_MBF_ATTR;
	cmbf.bufsz = BUDDY1_TELEMETRY_DEPTH * sizeof(BUDDY1_TELEMETRY);
	cmbf.maxmsz = sizeof(BUDDY1_TELEMETRY);
	mbf_telemetry = tk_cre_mbf(&cmbf);
	if (mbf_telemetry < E_OK) {
		return mbf_telemetry;
	}

	cmbf.bufsz = BUDDY1_COMMAND_DEPTH * sizeof(BUDDY1_COMMAND);
	cmbf.maxmsz = sizeof(BUDDY1_COMMAND);
	mbf_commands = tk_cre_mbf(&cmbf);
	if (mbf_commands < E_OK) {
		return mbf_commands;
	}

	return E_OK;
}

EXPORT ER subsystem5_start(void)
{
	/* MQTT work runs in the kernel's existing radio owner task. */
	return E_OK;
}

EXPORT ER subsystem5_publish_telemetry(const BUDDY1_TELEMETRY *telemetry)
{
	if (telemetry == NULL) {
		return E_PAR;
	}

	return tk_snd_mbf(mbf_telemetry, (void *)telemetry,
			 sizeof(*telemetry), TMO_POL);
}

EXPORT ER subsystem5_receive_command(BUDDY1_COMMAND *command, TMO timeout)
{
	INT	len;

	if (command == NULL) {
		return E_PAR;
	}

	len = tk_rcv_mbf(mbf_commands, command, timeout);
	if (len < E_OK) {
		return (ER)len;
	}
	if (len != sizeof(*command)) {
		return E_SYS;
	}

	return E_OK;
}

/*
 * Consume all queued snapshots and return only the newest one. Telemetry is
 * state, not an event log: sending older queued values after a network outage
 * would report stale vehicle state.
 */
EXPORT int buddy1_mqtt_take_latest_telemetry(BUDDY1_TELEMETRY *telemetry)
{
	BUDDY1_TELEMETRY	next;
	INT			len;
	INT			i;
	BOOL			found = FALSE;

	if (telemetry == NULL) {
		return BUDDY1_MQTT_ERROR;
	}

	for (i = 0; i < BUDDY1_TELEMETRY_DEPTH; i++) {
		len = tk_rcv_mbf(mbf_telemetry, &next, TMO_POL);
		if (len == E_TMOUT) {
			break;
		}
		if (len < E_OK) {
			return BUDDY1_MQTT_ERROR;
		}
		if (len != sizeof(next)) {
			return BUDDY1_MQTT_ERROR;
		}
		*telemetry = next;
		found = TRUE;
	}

	return found ? E_OK : BUDDY1_MQTT_NO_DATA;
}

EXPORT int buddy1_mqtt_submit_command(const char *payload, int length)
{
	BUDDY1_COMMAND	command;
	ER		er;

	if (payload == NULL || length <= 0) {
		return BUDDY1_MQTT_REJECTED;
	}

	if (length == 5 && payload[0] == 's' && payload[1] == 't' &&
	    payload[2] == 'a' && payload[3] == 'r' && payload[4] == 't') {
		command = BUDDY1_CMD_START;
	} else if (length == 4 && payload[0] == 's' && payload[1] == 't' &&
		   payload[2] == 'o' && payload[3] == 'p') {
		command = BUDDY1_CMD_STOP;
	} else if (length == 5 && payload[0] == 'p' && payload[1] == 'a' &&
		   payload[2] == 'u' && payload[3] == 's' &&
		   payload[4] == 'e') {
		command = BUDDY1_CMD_PAUSE;
	} else if (length == 6 && payload[0] == 'r' && payload[1] == 'e' &&
		   payload[2] == 's' && payload[3] == 'u' &&
		   payload[4] == 'm' && payload[5] == 'e') {
		command = BUDDY1_CMD_RESUME;
	} else {
		return BUDDY1_MQTT_REJECTED;
	}

	er = tk_snd_mbf(mbf_commands, &command, sizeof(command), TMO_POL);
	if (er == E_TMOUT) {
		return BUDDY1_MQTT_QUEUE_FULL;
	}
	if (er < E_OK) {
		return BUDDY1_MQTT_ERROR;
	}

	return E_OK;
}
