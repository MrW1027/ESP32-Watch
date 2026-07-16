#ifndef __MY_MQTT_H
#define __MY_MQTT_H
#include <Arduino.h>

void my_mqtt_init();
void reconnect();
void my_mqtt_publish();
void my_mqtt_task(void* para);
#endif