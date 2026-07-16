/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

#include "my_timer.h"

void setup_scr_screen_main(lv_ui *ui)
{
    //Write codes screen_main
    ui->screen_main = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_main, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_main, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_main, &_background_240x280, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_tileview_1
    ui->screen_main_tileview_1 = lv_tileview_create(ui->screen_main);
    ui->screen_main_tileview_1_page_main = lv_tileview_add_tile(ui->screen_main_tileview_1, 0, 0, LV_DIR_RIGHT);
    ui->screen_main_tileview_1_page_heart = lv_tileview_add_tile(ui->screen_main_tileview_1, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    ui->screen_main_tileview_1_page_weather = lv_tileview_add_tile(ui->screen_main_tileview_1, 2, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    ui->screen_main_tileview_1_page_ai = lv_tileview_add_tile(ui->screen_main_tileview_1, 3, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    ui->screen_main_tileview_1_page_setting = lv_tileview_add_tile(ui->screen_main_tileview_1, 4, 0, LV_DIR_LEFT);
    lv_obj_set_pos(ui->screen_main_tileview_1, 0, 20);
    lv_obj_set_size(ui->screen_main_tileview_1, 240, 260);
    lv_obj_set_scrollbar_mode(ui->screen_main_tileview_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_main_tileview_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main_tileview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_main_tileview_1, lv_color_hex(0xf6f6f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_main_tileview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_main_tileview_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main_tileview_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_main_tileview_1, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_main_tileview_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



    //Write codes screen_main_img_1_bk
    ui->screen_main_img_1_bk = lv_img_create(ui->screen_main_tileview_1_page_main);
    lv_obj_add_flag(ui->screen_main_img_1_bk, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img_1_bk, &_background_alpha_240x260);
    lv_img_set_pivot(ui->screen_main_img_1_bk, 50,50);
    lv_img_set_angle(ui->screen_main_img_1_bk, 0);
    lv_obj_set_pos(ui->screen_main_img_1_bk, 0, 0);
    lv_obj_set_size(ui->screen_main_img_1_bk, 240, 260);

    //Write style for screen_main_img_1_bk, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img_1_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img_1_bk, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img_1_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img_1_bk, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_time_hour
    ui->screen_main_label_time_hour = lv_label_create(ui->screen_main_tileview_1_page_main);
    lv_label_set_text(ui->screen_main_label_time_hour, "12:00:22");
    lv_label_set_long_mode(ui->screen_main_label_time_hour, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_main_label_time_hour, 31, 34);
    lv_obj_set_size(ui->screen_main_label_time_hour, 181, 20);

    //Write style for screen_main_label_time_hour, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_time_hour, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_time_hour, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_time_hour, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_time_hour, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_time_hour, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_time_day
    ui->screen_main_label_time_day = lv_label_create(ui->screen_main_tileview_1_page_main);
    lv_label_set_text(ui->screen_main_label_time_day, "2025:3:15");
    lv_label_set_long_mode(ui->screen_main_label_time_day, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_main_label_time_day, 17, 85);
    lv_obj_set_size(ui->screen_main_label_time_day, 203, 19);

    //Write style for screen_main_label_time_day, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_time_day, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_time_day, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_time_day, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_time_day, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_time_day, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_time_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes screen_main_img_2_bk
    ui->screen_main_img_2_bk = lv_img_create(ui->screen_main_tileview_1_page_heart);
    lv_obj_add_flag(ui->screen_main_img_2_bk, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img_2_bk, &_background_alpha_240x260);
    lv_img_set_pivot(ui->screen_main_img_2_bk, 50,50);
    lv_img_set_angle(ui->screen_main_img_2_bk, 0);
    lv_obj_set_pos(ui->screen_main_img_2_bk, 0, 0);
    lv_obj_set_size(ui->screen_main_img_2_bk, 240, 260);

    //Write style for screen_main_img_2_bk, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img_2_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img_2_bk, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img_2_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img_2_bk, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_btn_heart
    ui->screen_main_btn_heart = lv_btn_create(ui->screen_main_tileview_1_page_heart);
    ui->screen_main_btn_heart_label = lv_label_create(ui->screen_main_btn_heart);
    lv_label_set_text(ui->screen_main_btn_heart_label, "");
    lv_label_set_long_mode(ui->screen_main_btn_heart_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_btn_heart_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_btn_heart, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_main_btn_heart_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_main_btn_heart, 60, 80);
    lv_obj_set_size(ui->screen_main_btn_heart, 120, 120);

    //Write style for screen_main_btn_heart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main_btn_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_main_btn_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_btn_heart, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_btn_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_main_btn_heart, &_heart_rate_120x120, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_main_btn_heart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_main_btn_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_btn_heart, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_btn_heart, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_btn_heart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_btn_heart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes screen_main_img_3_bk
    ui->screen_main_img_3_bk = lv_img_create(ui->screen_main_tileview_1_page_weather);
    lv_obj_add_flag(ui->screen_main_img_3_bk, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img_3_bk, &_background_alpha_240x260);
    lv_img_set_pivot(ui->screen_main_img_3_bk, 50,50);
    lv_img_set_angle(ui->screen_main_img_3_bk, 0);
    lv_obj_set_pos(ui->screen_main_img_3_bk, 0, 0);
    lv_obj_set_size(ui->screen_main_img_3_bk, 240, 260);

    //Write style for screen_main_img_3_bk, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img_3_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img_3_bk, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img_3_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img_3_bk, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_btn_weather
    ui->screen_main_btn_weather = lv_btn_create(ui->screen_main_tileview_1_page_weather);
    ui->screen_main_btn_weather_label = lv_label_create(ui->screen_main_btn_weather);
    lv_label_set_text(ui->screen_main_btn_weather_label, "");
    lv_label_set_long_mode(ui->screen_main_btn_weather_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_btn_weather_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_btn_weather, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_main_btn_weather_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_main_btn_weather, 60, 80);
    lv_obj_set_size(ui->screen_main_btn_weather, 120, 120);

    //Write style for screen_main_btn_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main_btn_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_main_btn_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_btn_weather, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_btn_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_main_btn_weather, &_weather_120x120, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_main_btn_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_main_btn_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_btn_weather, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_btn_weather, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_btn_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_btn_weather, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes screen_main_img__4_bk
    ui->screen_main_img__4_bk = lv_img_create(ui->screen_main_tileview_1_page_ai);
    lv_obj_add_flag(ui->screen_main_img__4_bk, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img__4_bk, &_background_alpha_240x260);
    lv_img_set_pivot(ui->screen_main_img__4_bk, 50,50);
    lv_img_set_angle(ui->screen_main_img__4_bk, 0);
    lv_obj_set_pos(ui->screen_main_img__4_bk, 0, 0);
    lv_obj_set_size(ui->screen_main_img__4_bk, 240, 260);

    //Write style for screen_main_img__4_bk, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img__4_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img__4_bk, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img__4_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img__4_bk, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_btn_ai
    ui->screen_main_btn_ai = lv_btn_create(ui->screen_main_tileview_1_page_ai);
    ui->screen_main_btn_ai_label = lv_label_create(ui->screen_main_btn_ai);
    lv_label_set_text(ui->screen_main_btn_ai_label, "");
    lv_label_set_long_mode(ui->screen_main_btn_ai_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_main_btn_ai_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_main_btn_ai, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_main_btn_ai_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_main_btn_ai, 60, 80);
    lv_obj_set_size(ui->screen_main_btn_ai, 120, 120);

    //Write style for screen_main_btn_ai, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main_btn_ai, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_main_btn_ai, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_btn_ai, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_btn_ai, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_main_btn_ai, &_deepseek_120x120, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_main_btn_ai, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_main_btn_ai, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_btn_ai, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_btn_ai, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_btn_ai, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_btn_ai, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes screen_main_img_5_bk
    ui->screen_main_img_5_bk = lv_img_create(ui->screen_main_tileview_1_page_setting);
    lv_obj_add_flag(ui->screen_main_img_5_bk, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img_5_bk, &_background_alpha_240x260);
    lv_img_set_pivot(ui->screen_main_img_5_bk, 50,50);
    lv_img_set_angle(ui->screen_main_img_5_bk, 0);
    lv_obj_set_pos(ui->screen_main_img_5_bk, 0, 0);
    lv_obj_set_size(ui->screen_main_img_5_bk, 240, 260);

    //Write style for screen_main_img_5_bk, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img_5_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img_5_bk, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img_5_bk, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img_5_bk, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_setting
    ui->screen_main_img_setting = lv_img_create(ui->screen_main_tileview_1_page_setting);
    lv_obj_add_flag(ui->screen_main_img_setting, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img_setting, &_setting_alpha_120x120);
    lv_img_set_pivot(ui->screen_main_img_setting, 50,50);
    lv_img_set_angle(ui->screen_main_img_setting, 0);
    lv_obj_set_pos(ui->screen_main_img_setting, 60, 80);
    lv_obj_set_size(ui->screen_main_img_setting, 120, 120);

    //Write style for screen_main_img_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img_setting, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_top_wifi
    ui->screen_main_img_top_wifi = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_main_img_top_wifi, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img_top_wifi, &_wifi_alpha_20x20);
    lv_img_set_pivot(ui->screen_main_img_top_wifi, 50,50);
    lv_img_set_angle(ui->screen_main_img_top_wifi, 0);
    lv_obj_set_pos(ui->screen_main_img_top_wifi, 183, 0);
    lv_obj_set_size(ui->screen_main_img_top_wifi, 20, 20);

    //Write style for screen_main_img_top_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img_top_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img_top_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img_top_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img_top_wifi, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_top_time
    ui->screen_main_label_top_time = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_main_label_top_time, "12:20");
    lv_label_set_long_mode(ui->screen_main_label_top_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_main_label_top_time, 20, 0);
    lv_obj_set_size(ui->screen_main_label_top_time, 60, 20);

    //Write style for screen_main_label_top_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_top_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_top_time, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_top_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_top_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_top_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_top_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_1
    ui->screen_main_img_1 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_main_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_main_img_1, &_dianliang_alpha_20x20);
    lv_img_set_pivot(ui->screen_main_img_1, 50,50);
    lv_img_set_angle(ui->screen_main_img_1, 0);
    lv_obj_set_pos(ui->screen_main_img_1, 160, 0);
    lv_obj_set_size(ui->screen_main_img_1, 20, 20);

    //Write style for screen_main_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_main_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_main_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_main_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_main.
    my_timer_time_begin();
    my_timer_mqtt_begin();
    
    //Update current screen layout.
    lv_obj_update_layout(ui->screen_main);

    //Init events for screen.
    events_init_screen_main(ui);
}
