#include "my_timer.h"

#include <Arduino.h>
#include "gui_guider.h"
#include "lvgl.h"

#include "my_signal.h"
#include "my_max30102.h"
#include "WiFiUser.h"
#include "my_mqtt.h"

lv_timer_t *timer_time;
lv_timer_t *timer_heart_spo2;
lv_timer_t *timer_mqtt;

static void my_timer1(lv_timer_t *timer);
static void my_timer2(lv_timer_t *timer);
static void my_timer3(lv_timer_t *timer);

void my_timer_time_begin(void)
{
    timer_time = lv_timer_create(my_timer1, 1000, NULL);
}
void my_timer_heart_spo2_begin(void)
{
    timer_heart_spo2 = lv_timer_create(my_timer2, 1000, NULL);
}
void my_timer_mqtt_begin(void)
{
    timer_mqtt = lv_timer_create(my_timer3, 3000, NULL);
}
void my_timer1(lv_timer_t *timer)
{
    time_task();
    lv_label_set_text(guider_ui.screen_main_label_top_time, get_time_HM());
    lv_label_set_text(guider_ui.screen_main_label_time_day, get_time_YMD());
    lv_label_set_text(guider_ui.screen_main_label_time_hour, get_time_HMS());
    refresh_wifi_flag();
    if (get_wifi_flag())
    {
        lv_img_set_src(guider_ui.screen_main_img_top_wifi, &_wifi_alpha_20x20);
    }
    else
    {
        lv_img_set_src(guider_ui.screen_main_img_top_wifi, &_wifi_x_alpha_20x20);
    }
}
void my_timer2(lv_timer_t *timer)
{
    char heart_timer[10];
    char spo2_timer[10];
    sprintf(heart_timer, "%dbpm", get_heart_rate());
    sprintf(spo2_timer, "%d%%", get_spo2());
    lv_label_set_text(guider_ui.screen_heart_label_heart, heart_timer);
    lv_label_set_text(guider_ui.screen_heart_label_xueyang, spo2_timer);
    if (get_heart_rate() != 0 && get_spo2() != 0)
        lv_label_set_text(guider_ui.screen_heart_label_wait, "");
}
void my_timer3(lv_timer_t *timer)
{
    my_mqtt_publish();
}

void refresh_weather_screen_date(String *date)
{
    lv_label_set_text(guider_ui.screen_weather_day1, date[0].c_str());
    lv_label_set_text(guider_ui.screen_weather_day2, date[1].c_str());
    lv_label_set_text(guider_ui.screen_weather_day3, date[2].c_str());
    lv_label_set_text(guider_ui.screen_weather_day4, date[3].c_str());
    lv_label_set_text(guider_ui.screen_weather_day5, date[4].c_str());
}
void refresh_weather_screen_temp(String *temp)
{
    lv_label_set_text(guider_ui.screen_weather_tem1, temp[0].c_str());
    lv_label_set_text(guider_ui.screen_weather_tem2, temp[1].c_str());
    lv_label_set_text(guider_ui.screen_weather_tem3, temp[2].c_str());
    lv_label_set_text(guider_ui.screen_weather_tem4, temp[3].c_str());
    lv_label_set_text(guider_ui.screen_weather_tem5, temp[4].c_str());
}
void refresh_weather_screen_weather_select(lv_obj_t *img, String weather)
{
    if (weather == "晴")
    {
        lv_img_set_src(img, &_qing_alpha_20x20);
    }
    else if (weather == "晴转多云"||weather == "阴转阵雨"||weather == "阴")
    {
        lv_img_set_src(img, &_duo_yun_alpha_20x20);
    }
    else if (weather == "多云转小雨"||weather == "阴转小雨"||weather == "小雨"||weather == "小雨转阴")
    {
        lv_img_set_src(img, &_xiao_yu_alpha_20x20);
    }
    else if (weather == "多云转中雨"||weather == "多云"||weather == "中雨")
    {
        lv_img_set_src(img, &_zhong_yu_alpha_20x20);
    }
    else if (weather == "多云转大雨"||weather == "大雨")
    {
        lv_img_set_src(img, &_da_yu_alpha_20x20);
    }
    else
    {
        lv_img_set_src(img, &_qing_alpha_20x20);
    }
}
void refresh_weather_screen_weather(String *weather)
{
    refresh_weather_screen_weather_select(guider_ui.screen_weather_wea1, weather[0]);
    refresh_weather_screen_weather_select(guider_ui.screen_weather_wea2, weather[1]);
    refresh_weather_screen_weather_select(guider_ui.screen_weather_wea3, weather[2]);
    refresh_weather_screen_weather_select(guider_ui.screen_weather_wea4, weather[3]);
    refresh_weather_screen_weather_select(guider_ui.screen_weather_wea5, weather[4]);
}
void refresh_weather_screen_date_weat_temp(void)
{
    refresh_weather_screen_date(get_t_day());
    refresh_weather_screen_weather(get_t_weather());
    refresh_weather_screen_temp(get_t_temp());
}

void my_tft_init()
{
    ledcSetup(1, 5000 /*freq*/, 10 /*resolution*/);
    ledcAttachPin(4, 1);
    analogReadResolution(10);
    ledcWrite(1, 200);
    Serial.println("背光set done");
}
void my_tft_light_set(int value)
{
    char light[6];
    ledcWrite(1, value * 1024 / 100);
    sprintf(light, "%d%%", value);
    lv_label_set_text(guider_ui.screen_setting_label_hight, light);
}

void watch_restart(void)
{
    Serial.println("\n重启清空网络连保存接信息.");
    restoreWiFi(); // 删除保存的wifi信息
    ESP.restart(); // 重启复位esp32
}