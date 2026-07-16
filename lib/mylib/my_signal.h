#ifndef __MY_SIGNAL_H
#define __MY_SIGNAL_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include "WiFi.h"

void refresh_weather(void);
void refresh_time(void);
void refresh_wifi_flag(void);
void refresh_weather_screen_date_weat_temp(void);
void time_task(void);

void buzzer_init();
void buzzer_on();
void buzzer_off();
void buzzer_on_5s();

String* get_t_day();
String* get_t_weather();
String* get_t_temp();

String get_time_day();
String get_time_hour();

bool get_wifi_flag();

char* get_time_YMD();
char* get_time_HMS();
char* get_time_HM();

float get_humidity();
float get_temperature();



void time_task(void* para);

#endif

//写代码麻烦死了，最近天气还冷得很
