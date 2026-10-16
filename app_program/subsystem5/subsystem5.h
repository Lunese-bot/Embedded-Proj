/*
 *	subsystem5.h
 *	Buddy 1 application interface for Wi-Fi telemetry and commands.
 *
 *	lwIP/MQTT remains owned by cyw43_utk_task(). Application tasks use
 *	these functions to exchange data with that service without calling
 *	network APIs themselves.
 */

#ifndef SUBSYSTEM5_H
#define SUBSYSTEM5_H

#include "../app.h"
#include "buddy1_mqtt.h"

#define BUDDY1_TELEMETRY_DEPTH	4
#define BUDDY1_COMMAND_DEPTH	4

/* Applications may enqueue telemetry without waiting. If its bounded queue
   is full, the caller receives E_TMOUT and can count/report the dropped
   update. MQTT drains the queue to the newest snapshot every 200 ms. */
IMPORT ER subsystem5_publish_telemetry(const BUDDY1_TELEMETRY *telemetry);

/* The Vehicle Controller consumes validated MQTT commands from this queue. */
IMPORT ER subsystem5_receive_command(BUDDY1_COMMAND *command, TMO timeout);

#endif /* SUBSYSTEM5_H */
