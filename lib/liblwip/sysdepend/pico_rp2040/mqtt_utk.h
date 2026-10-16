/*
 *	mqtt_utk.h
 *	MQTT client wiring for micro T-Kernel (RP2040, NO_SYS=1).
 *
 *	All calls into this client are made by cyw43_utk_task(), the owner of
 *	the CYW43439 and lwIP state.
 */

#ifndef MQTT_UTK_H
#define MQTT_UTK_H

/*
 * Call once, from cyw43_utk_task() the first time the link reaches
 * CYW43_LINK_UP (see cyw43_utk.c). Starts the MQTT connection to
 * MQTT_BROKER_IP:MQTT_BROKER_PORT (config/wifi_secrets.h). Once connected,
 * the service publishes heartbeats and queued vehicle telemetry, and
 * subscribes to validated remote commands. Safe to call more than once.
 */
void mqtt_utk_start(void);

#endif /* MQTT_UTK_H */
