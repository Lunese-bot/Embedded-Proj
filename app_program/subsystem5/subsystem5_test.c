/*
 *	subsystem5_test.c
 *	On-target tests for subsystem 5.  Built only with: make TEST=1
 *	Record results in testing/subsystem5/TEST_LOG.md.  IDs: S5-U01, ...
 */

#include "subsystem5.h"
#include "../test_report.h"

#if APP_TEST

EXPORT void subsystem5_test(void)
{
	BUDDY1_COMMAND	command;
	BUDDY1_TELEMETRY	sent = {1234, 2, 250, 4200, -1, 180, 35, 0};
	BUDDY1_TELEMETRY	received;
	ER	er;
	int	result;

	TEST_INIT();

	TEST_CHECK("S5-U01", tk_get_tid() > 0,
		   "communication interfaces run in task context");

	/* Remote commands are a strict allowlist, not arbitrary motor commands. */
	result = buddy1_mqtt_submit_command("start", 5);
	er = subsystem5_receive_command(&command, TMO_POL);
	TEST_CHECK("S5-U02", result == E_OK && er == E_OK &&
		   command == BUDDY1_CMD_START,
		   "valid start command reaches the Vehicle Controller queue");

	result = buddy1_mqtt_submit_command("motor=100", 9);
	TEST_CHECK("S5-U03", result == BUDDY1_MQTT_REJECTED,
		   "unknown command payload is rejected");

	er = subsystem5_publish_telemetry(&sent);
	result = buddy1_mqtt_take_latest_telemetry(&received);
	TEST_CHECK("S5-U04", er == E_OK && result == E_OK &&
		   received.timestamp_ms == sent.timestamp_ms &&
		   received.mission_state == sent.mission_state &&
		   received.speed_mm_s == sent.speed_mm_s &&
		   received.distance_mm == sent.distance_mm &&
		   received.line_error == sent.line_error &&
		   received.obstacle_distance_mm == sent.obstacle_distance_mm &&
		   received.hump_peak_mm == sent.hump_peak_mm &&
		   received.fault_code == sent.fault_code,
		   "telemetry snapshot passes through the bounded queue");

	TEST_SUMMARY("subsystem5");
}

#endif /* APP_TEST */
