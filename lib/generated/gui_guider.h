/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen_main;
	bool screen_main_del;
	lv_obj_t *screen_main_tileview_1;
	lv_obj_t *screen_main_tileview_1_page_main;
	lv_obj_t *screen_main_tileview_1_page_heart;
	lv_obj_t *screen_main_tileview_1_page_weather;
	lv_obj_t *screen_main_tileview_1_page_ai;
	lv_obj_t *screen_main_tileview_1_page_setting;
	lv_obj_t *screen_main_img_1_bk;
	lv_obj_t *screen_main_label_time_hour;
	lv_obj_t *screen_main_label_time_day;
	lv_obj_t *screen_main_img_2_bk;
	lv_obj_t *screen_main_btn_heart;
	lv_obj_t *screen_main_btn_heart_label;
	lv_obj_t *screen_main_img_3_bk;
	lv_obj_t *screen_main_btn_weather;
	lv_obj_t *screen_main_btn_weather_label;
	lv_obj_t *screen_main_img__4_bk;
	lv_obj_t *screen_main_btn_ai;
	lv_obj_t *screen_main_btn_ai_label;
	lv_obj_t *screen_main_img_5_bk;
	lv_obj_t *screen_main_img_setting;
	lv_obj_t *screen_main_img_top_wifi;
	lv_obj_t *screen_main_label_top_time;
	lv_obj_t *screen_main_img_1;
	lv_obj_t *screen_heart;
	bool screen_heart_del;
	lv_obj_t *screen_heart_btn_heart_back;
	lv_obj_t *screen_heart_btn_heart_back_label;
	lv_obj_t *screen_heart_img_heart;
	lv_obj_t *screen_heart_img_xueyang;
	lv_obj_t *screen_heart_label_heart;
	lv_obj_t *screen_heart_label_xueyang;
	lv_obj_t *screen_heart_label_wait;
	lv_obj_t *screen_weather;
	bool screen_weather_del;
	lv_obj_t *screen_weather_btn_weather_back;
	lv_obj_t *screen_weather_btn_weather_back_label;
	lv_obj_t *screen_weather_day1;
	lv_obj_t *screen_weather_wea1;
	lv_obj_t *screen_weather_tem1;
	lv_obj_t *screen_weather_tem2;
	lv_obj_t *screen_weather_wea2;
	lv_obj_t *screen_weather_day2;
	lv_obj_t *screen_weather_tem3;
	lv_obj_t *screen_weather_wea3;
	lv_obj_t *screen_weather_day3;
	lv_obj_t *screen_weather_tem4;
	lv_obj_t *screen_weather_day4;
	lv_obj_t *screen_weather_wea4;
	lv_obj_t *screen_weather_tem5;
	lv_obj_t *screen_weather_day5;
	lv_obj_t *screen_weather_wea5;
	lv_obj_t *screen_ai;
	bool screen_ai_del;
	lv_obj_t *screen_ai_btn_ai_back;
	lv_obj_t *screen_ai_btn_ai_back_label;
	lv_obj_t *screen_ai_img_doubao;
	lv_obj_t *screen_setting;
	bool screen_setting_del;
	lv_obj_t *screen_setting_slider_hight;
	lv_obj_t *screen_setting_label_hight;
	lv_obj_t *screen_setting_img_hight;
	lv_obj_t *screen_setting_btn_seeting_back;
	lv_obj_t *screen_setting_btn_seeting_back_label;
	lv_obj_t *screen_setting_btn_restart;
	lv_obj_t *screen_setting_btn_restart_label;
	lv_obj_t *screen_img;
	bool screen_img_del;
	lv_obj_t *screen_img_img_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_screen_main(lv_ui *ui);
void setup_scr_screen_heart(lv_ui *ui);
void setup_scr_screen_weather(lv_ui *ui);
void setup_scr_screen_ai(lv_ui *ui);
void setup_scr_screen_setting(lv_ui *ui);
void setup_scr_screen_img(lv_ui *ui);

LV_IMG_DECLARE(_background_240x280);
LV_IMG_DECLARE(_background_alpha_240x260);
LV_IMG_DECLARE(_background_alpha_240x260);

LV_IMG_DECLARE(_heart_rate_120x120);
LV_IMG_DECLARE(_background_alpha_240x260);

LV_IMG_DECLARE(_weather_120x120);
LV_IMG_DECLARE(_background_alpha_240x260);

LV_IMG_DECLARE(_deepseek_120x120);
LV_IMG_DECLARE(_background_alpha_240x260);
LV_IMG_DECLARE(_setting_alpha_120x120);
LV_IMG_DECLARE(_wifi_alpha_20x20);
LV_IMG_DECLARE(_dianliang_alpha_20x20);

LV_IMG_DECLARE(_heart_bk_240x280);

LV_IMG_DECLARE(_left_50x40);
LV_IMG_DECLARE(_xin_lu_alpha_50x50);
LV_IMG_DECLARE(_xue_yang_alpha_50x50);

LV_IMG_DECLARE(_weather_backgroud_240x280);

LV_IMG_DECLARE(_left_50x40);
LV_IMG_DECLARE(_qing_alpha_20x20);
LV_IMG_DECLARE(_duo_yun_alpha_20x20);
LV_IMG_DECLARE(_xiao_yu_alpha_20x20);
LV_IMG_DECLARE(_zhong_yu_alpha_20x20);
LV_IMG_DECLARE(_da_yu_alpha_20x20);

LV_IMG_DECLARE(_ai_bk_240x280);

LV_IMG_DECLARE(_left_50x40);
LV_IMG_DECLARE(_deepseek_ai_alpha_60x60);

LV_IMG_DECLARE(_background_240x280);
LV_IMG_DECLARE(_light_alpha_40x40);

LV_IMG_DECLARE(_left_50x40);

LV_IMG_DECLARE(_restart_80x80);
LV_IMG_DECLARE(_wifi_x_alpha_20x20);

LV_FONT_DECLARE(lv_font_montserratMedium_24)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_21)


#ifdef __cplusplus
}
#endif
#endif
