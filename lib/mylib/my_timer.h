#ifndef __MY_TIMER_H
#define __MY_TIMER_H


void my_timer_time_begin(void);
void my_timer_heart_spo2_begin(void);
void my_timer_mqtt_begin(void);
void refresh_weather_screen_date_weat_temp(void);

void my_tft_init();
void my_tft_light_set(int value);
void watch_restart(void);
#endif