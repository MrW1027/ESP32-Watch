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



void setup_scr_screen_setting(lv_ui *ui)
{
    //Write codes screen_setting
    ui->screen_setting = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_setting, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_setting, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_setting, &_background_240x280, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_slider_hight
    ui->screen_setting_slider_hight = lv_slider_create(ui->screen_setting);
    lv_slider_set_range(ui->screen_setting_slider_hight, 0, 100);
    lv_slider_set_mode(ui->screen_setting_slider_hight, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_setting_slider_hight, 50, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_setting_slider_hight, 61, 225);
    lv_obj_set_size(ui->screen_setting_slider_hight, 160, 8);

    //Write style for screen_setting_slider_hight, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_hight, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_hight, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_hight, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_hight, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_setting_slider_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_slider_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_hight, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_hight, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_hight, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_hight, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_hight, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_hight, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_hight, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_hight, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_hight, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_hight, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_hight
    ui->screen_setting_label_hight = lv_label_create(ui->screen_setting);
    lv_label_set_text(ui->screen_setting_label_hight, "50%");
    lv_label_set_long_mode(ui->screen_setting_label_hight, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_setting_label_hight, -20, 253);
    lv_obj_set_size(ui->screen_setting_label_hight, 100, 32);

    //Write style for screen_setting_label_hight, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_hight, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_hight, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_hight, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_hight, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_hight, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_img_hight
    ui->screen_setting_img_hight = lv_img_create(ui->screen_setting);
    lv_obj_add_flag(ui->screen_setting_img_hight, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_setting_img_hight, &_light_alpha_40x40);
    lv_img_set_pivot(ui->screen_setting_img_hight, 50,50);
    lv_img_set_angle(ui->screen_setting_img_hight, 0);
    lv_obj_set_pos(ui->screen_setting_img_hight, 5, 209);
    lv_obj_set_size(ui->screen_setting_img_hight, 40, 40);

    //Write style for screen_setting_img_hight, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_setting_img_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_setting_img_hight, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_img_hight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_setting_img_hight, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_btn_seeting_back
    ui->screen_setting_btn_seeting_back = lv_btn_create(ui->screen_setting);
    ui->screen_setting_btn_seeting_back_label = lv_label_create(ui->screen_setting_btn_seeting_back);
    lv_label_set_text(ui->screen_setting_btn_seeting_back_label, "");
    lv_label_set_long_mode(ui->screen_setting_btn_seeting_back_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_btn_seeting_back_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_btn_seeting_back, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_btn_seeting_back_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_setting_btn_seeting_back, 180, 21);
    lv_obj_set_size(ui->screen_setting_btn_seeting_back, 50, 40);

    //Write style for screen_setting_btn_seeting_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_btn_seeting_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_btn_seeting_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_btn_seeting_back, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_btn_seeting_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_setting_btn_seeting_back, &_left_50x40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_setting_btn_seeting_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_setting_btn_seeting_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_btn_seeting_back, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_btn_seeting_back, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_btn_seeting_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_btn_seeting_back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_btn_restart
    ui->screen_setting_btn_restart = lv_btn_create(ui->screen_setting);
    ui->screen_setting_btn_restart_label = lv_label_create(ui->screen_setting_btn_restart);
    lv_label_set_text(ui->screen_setting_btn_restart_label, "");
    lv_label_set_long_mode(ui->screen_setting_btn_restart_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_setting_btn_restart_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_setting_btn_restart, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_setting_btn_restart_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_setting_btn_restart, 80, 94);
    lv_obj_set_size(ui->screen_setting_btn_restart, 80, 80);

    //Write style for screen_setting_btn_restart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_btn_restart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_setting_btn_restart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_btn_restart, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_btn_restart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_setting_btn_restart, &_restart_80x80, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->screen_setting_btn_restart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->screen_setting_btn_restart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_btn_restart, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_btn_restart, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_btn_restart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_btn_restart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_setting.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_setting);

    //Init events for screen.
    events_init_screen_setting(ui);
}
