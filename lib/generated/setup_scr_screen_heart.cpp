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

void setup_scr_screen_heart(lv_ui *ui)
{
    //Write codes screen_heart
    ui->screen_heart = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_heart, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_heart, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_heart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_heart, &_heart_bk_240x280, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_heart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_heart_btn_heart_back
    ui->screen_heart_btn_heart_back = lv_btn_create(ui->screen_heart);
    ui->screen_heart_btn_heart_back_label = lv_label_create(ui->screen_heart_btn_heart_back);
    lv_label_set_text(ui->screen_heart_btn_heart_back_label, "");
    lv_label_set_long_mode(ui->screen_heart_btn_heart_back_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_heart_btn_heart_back_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_heart_btn_heart_back, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_heart_btn_heart_back_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_heart_btn_heart_back, 176, 28);
    lv_obj_set_size(ui->screen_heart_btn_heart_back, 50, 40);

    //Write style for screen_heart_btn_heart_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_heart_btn_heart_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_heart_btn_heart_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_heart_btn_heart_back, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_heart_btn_heart_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_heart_btn_heart_back, &_left_50x40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_heart_btn_heart_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_heart_btn_heart_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_heart_btn_heart_back, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_heart_btn_heart_back, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_heart_btn_heart_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_heart_btn_heart_back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_heart_img_heart
    ui->screen_heart_img_heart = lv_img_create(ui->screen_heart);
    lv_obj_add_flag(ui->screen_heart_img_heart, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_heart_img_heart, &_xin_lu_alpha_50x50);
    lv_img_set_pivot(ui->screen_heart_img_heart, 50,50);
    lv_img_set_angle(ui->screen_heart_img_heart, 0);
    lv_obj_set_pos(ui->screen_heart_img_heart, 35, 74);
    lv_obj_set_size(ui->screen_heart_img_heart, 50, 50);

    //Write style for screen_heart_img_heart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_heart_img_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_heart_img_heart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_heart_img_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_heart_img_heart, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_heart_img_xueyang
    ui->screen_heart_img_xueyang = lv_img_create(ui->screen_heart);
    lv_obj_add_flag(ui->screen_heart_img_xueyang, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_heart_img_xueyang, &_xue_yang_alpha_50x50);
    lv_img_set_pivot(ui->screen_heart_img_xueyang, 50,50);
    lv_img_set_angle(ui->screen_heart_img_xueyang, 0);
    lv_obj_set_pos(ui->screen_heart_img_xueyang, 151, 74);
    lv_obj_set_size(ui->screen_heart_img_xueyang, 50, 50);

    //Write style for screen_heart_img_xueyang, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_heart_img_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_heart_img_xueyang, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_heart_img_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_heart_img_xueyang, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_heart_label_heart
    ui->screen_heart_label_heart = lv_label_create(ui->screen_heart);
    lv_label_set_text(ui->screen_heart_label_heart, "0bpm");
    lv_label_set_long_mode(ui->screen_heart_label_heart, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_heart_label_heart, 16, 157);
    lv_obj_set_size(ui->screen_heart_label_heart, 98, 20);

    //Write style for screen_heart_label_heart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_heart_label_heart, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_heart_label_heart, &lv_font_montserratMedium_21, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_heart_label_heart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_heart_label_heart, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_heart_label_heart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_heart_label_heart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_heart_label_xueyang
    ui->screen_heart_label_xueyang = lv_label_create(ui->screen_heart);
    lv_label_set_text(ui->screen_heart_label_xueyang, "0%");
    lv_label_set_long_mode(ui->screen_heart_label_xueyang, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_heart_label_xueyang, 127, 157);
    lv_obj_set_size(ui->screen_heart_label_xueyang, 85, 20);

    //Write style for screen_heart_label_xueyang, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_heart_label_xueyang, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_heart_label_xueyang, &lv_font_montserratMedium_21, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_heart_label_xueyang, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_heart_label_xueyang, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_heart_label_xueyang, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_heart_label_xueyang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_heart_label_wait
    ui->screen_heart_label_wait = lv_label_create(ui->screen_heart);
    lv_label_set_text(ui->screen_heart_label_wait, "waiting...");
    lv_label_set_long_mode(ui->screen_heart_label_wait, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_heart_label_wait, 7, 223);
    lv_obj_set_size(ui->screen_heart_label_wait, 100, 22);

    //Write style for screen_heart_label_wait, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_heart_label_wait, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_heart_label_wait, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_heart_label_wait, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_heart_label_wait, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_heart_label_wait, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_heart_label_wait, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_heart.

    my_timer_heart_spo2_begin();

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_heart);

    //Init events for screen.
    events_init_screen_heart(ui);
}
