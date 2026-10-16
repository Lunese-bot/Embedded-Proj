/*
 *	buddy1_mqtt.h
 *	Types shared between application tasks and the MQTT service.
 *
 *	Keep this header independent of T-Kernel and lwIP headers: the Pico SDK
 *	and T-Kernel define some of the same standard-library names.
 */

#ifndef BUDDY1_MQTT_H
#define BUDDY1_MQTT_H

typedef enum {
	BUDDY1_CMD_START = 1,
	BUDDY1_CMD_STOP,
	BUDDY1_CMD_PAUSE,
	BUDDY1_CMD_RESUME
} BUDDY1_COMMAND;

typedef struct {
	unsigned int	timestamp_ms;
	unsigned int	mission_state;
	int		speed_mm_s;
	unsigned int	distance_mm;
	int		line_error;
	int		obstacle_distance_mm;	/* -1 when unavailable */
	unsigned int	hump_peak_mm;
	unsigned int	fault_code;
} BUDDY1_TELEMETRY;

#define BUDDY1_MQTT_NO_DATA		1
#define BUDDY1_MQTT_REJECTED		-1
#define BUDDY1_MQTT_QUEUE_FULL		-2
#define BUDDY1_MQTT_ERROR		-3

/* MQTT-service entry points. Call only from cyw43_utk_task(). */
int buddy1_mqtt_take_latest_telemetry(BUDDY1_TELEMETRY *telemetry);
int buddy1_mqtt_submit_command(const char *payload, int length);

#endif /* BUDDY1_MQTT_H */
