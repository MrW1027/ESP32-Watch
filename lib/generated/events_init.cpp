/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#include "my_timer.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static void screen_main_btn_heart_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_heart, guider_ui.screen_heart_del, &guider_ui.screen_main_del, setup_scr_screen_heart, LV_SCR_LOAD_ANIM_FADE_ON, 500, 200, false, false);
        break;
    }
    default:
        break;
    }
}

static void screen_main_btn_weather_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_weather, guider_ui.screen_weather_del, &guider_ui.screen_main_del, setup_scr_screen_weather, LV_SCR_LOAD_ANIM_FADE_ON, 500, 200, false, false);
        break;
    }
    default:
        break;
    }
}

static void screen_main_btn_ai_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_ai, guider_ui.screen_ai_del, &guider_ui.screen_main_del, setup_scr_screen_ai, LV_SCR_LOAD_ANIM_FADE_ON, 500, 200, false, false);
        break;
    }
    default:
        break;
    }
}

static void screen_main_img_setting_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting, guider_ui.screen_setting_del, &guider_ui.screen_main_del, setup_scr_screen_setting, LV_SCR_LOAD_ANIM_FADE_ON, 500, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_main(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_main_btn_heart, screen_main_btn_heart_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_btn_weather, screen_main_btn_weather_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_btn_ai, screen_main_btn_ai_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_img_setting, screen_main_img_setting_event_handler, LV_EVENT_ALL, ui);
}

static void screen_heart_btn_heart_back_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_heart_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_heart(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_heart_btn_heart_back, screen_heart_btn_heart_back_event_handler, LV_EVENT_ALL, ui);
}

static void screen_weather_btn_weather_back_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_weather_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 200, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_weather(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_weather_btn_weather_back, screen_weather_btn_weather_back_event_handler, LV_EVENT_ALL, ui);
}

static void screen_ai_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_ai_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_OVER_LEFT, 400, 200, false, false);
        break;
    }
    default:
        break;
    }
}

static void screen_ai_btn_ai_back_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_ai_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 200, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_ai(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_ai, screen_ai_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_ai_btn_ai_back, screen_ai_btn_ai_back_event_handler, LV_EVENT_ALL, ui);
}

static void screen_setting_slider_hight_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        int value = lv_slider_get_value(guider_ui.screen_setting_slider_hight);
        my_tft_light_set(value);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_btn_seeting_back_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_setting_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 200, false, false);
        break;
    }
    default:
        break;
    }
}

static void screen_setting_btn_restart_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        watch_restart();
        break;
    }
    default:
        break;
    }
}

void events_init_screen_setting(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_setting_slider_hight, screen_setting_slider_hight_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_btn_seeting_back, screen_setting_btn_seeting_back_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_setting_btn_restart, screen_setting_btn_restart_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{
}
