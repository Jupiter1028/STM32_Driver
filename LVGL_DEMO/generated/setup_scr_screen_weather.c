/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
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



void setup_scr_screen_weather(lv_ui *ui)
{
	//Write codes screen_weather
	ui->screen_weather = lv_obj_create(NULL);
	ui->g_kb_screen_weather = lv_zh_keyboard_create(ui->screen_weather, &lv_font_SourceHanSerifSC_Regular_18);
	lv_obj_add_event_cb(ui->g_kb_screen_weather, kb_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(ui->g_kb_screen_weather, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(ui->g_kb_screen_weather, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_size(ui->screen_weather, 280, 240);
	lv_obj_set_scrollbar_mode(ui->screen_weather, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_weather, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_weather, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_weather_icon
	ui->screen_weather_weather_icon = lv_img_create(ui->screen_weather);
	lv_obj_add_flag(ui->screen_weather_weather_icon, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_weather_weather_icon, &_cloudy_alpha_100x100);
	lv_img_set_pivot(ui->screen_weather_weather_icon, 50,50);
	lv_img_set_angle(ui->screen_weather_weather_icon, 0);
	lv_obj_set_pos(ui->screen_weather_weather_icon, 142, 36);
	lv_obj_set_size(ui->screen_weather_weather_icon, 100, 100);

	//Write style for screen_weather_weather_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_weather_weather_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_temp
	ui->screen_weather_label_temp = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_temp, "温度");
	lv_label_set_long_mode(ui->screen_weather_label_temp, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_temp, 10, 200);
	lv_obj_set_size(ui->screen_weather_label_temp, 50, 40);

	//Write style for screen_weather_label_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_temp, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_temp, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_temp, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_temp, &lv_font_STKAITI_22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_temp, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_temp, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_in_temp
	ui->screen_weather_in_temp = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_in_temp, "15.8℃");
	lv_label_set_long_mode(ui->screen_weather_in_temp, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_in_temp, 60, 200);
	lv_obj_set_size(ui->screen_weather_in_temp, 75, 40);

	//Write style for screen_weather_in_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_in_temp, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_in_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_in_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_in_temp, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_in_temp, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_in_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_in_temp, &lv_font_STKAITI_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_in_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_in_temp, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_in_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_in_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_in_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_in_temp, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_in_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_in_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_in_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_in_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_in_humi
	ui->screen_weather_in_humi = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_in_humi, "湿度");
	lv_label_set_long_mode(ui->screen_weather_in_humi, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_in_humi, 150, 200);
	lv_obj_set_size(ui->screen_weather_in_humi, 50, 40);

	//Write style for screen_weather_in_humi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_in_humi, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_in_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_in_humi, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_in_humi, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_in_humi, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_in_humi, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_in_humi, &lv_font_STKAITI_22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_in_humi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_in_humi, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_in_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_in_humi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_in_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_in_humi, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_in_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_in_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_in_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_in_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_humi
	ui->screen_weather_label_humi = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_humi, "73%");
	lv_label_set_long_mode(ui->screen_weather_label_humi, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_humi, 200, 200);
	lv_obj_set_size(ui->screen_weather_label_humi, 60, 40);

	//Write style for screen_weather_label_humi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_humi, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_humi, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_humi, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_humi, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_humi, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_humi, &lv_font_STKAITI_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_humi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_humi, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_humi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_humi, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_temp_out
	ui->screen_weather_label_temp_out = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_temp_out, "18℃");
	lv_label_set_long_mode(ui->screen_weather_label_temp_out, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_temp_out, 194, 140);
	lv_obj_set_size(ui->screen_weather_label_temp_out, 85, 37);

	//Write style for screen_weather_label_temp_out, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_temp_out, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_temp_out, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_temp_out, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_temp_out, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_temp_out, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_temp_out, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_temp_out, &lv_font_STKAITI_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_temp_out, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_temp_out, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_temp_out, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_temp_out, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_temp_out, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_temp_out, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_temp_out, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_temp_out, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_temp_out, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_temp_out, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_date
	ui->screen_weather_label_date = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_date, "2024/3/10");
	lv_label_set_long_mode(ui->screen_weather_label_date, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_date, 7, 7);
	lv_obj_set_size(ui->screen_weather_label_date, 89, 32);

	//Write style for screen_weather_label_date, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_date, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_date, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_date, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_date, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_date, lv_color_hex(0xeefbfb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_date, &lv_font_Acme_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_date, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_date, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_date, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_time
	ui->screen_weather_label_time = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_time, "22:45");
	lv_label_set_long_mode(ui->screen_weather_label_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_time, 0, 45);
	lv_obj_set_size(ui->screen_weather_label_time, 100, 40);

	//Write style for screen_weather_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_time, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_time, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_time, lv_color_hex(0xfff700), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_time, &lv_font_Acme_Regular_34, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_time, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_time, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_second
	ui->screen_weather_label_second = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_second, "00");
	lv_label_set_long_mode(ui->screen_weather_label_second, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_second, 85, 57);
	lv_obj_set_size(ui->screen_weather_label_second, 39, 37);

	//Write style for screen_weather_label_second, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_second, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_second, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_second, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_second, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_second, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_second, lv_color_hex(0xfff709), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_second, &lv_font_Acme_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_second, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_second, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_second, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_second, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_second, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_second, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_second, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_second, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_second, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_second, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_week
	ui->screen_weather_label_week = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_week, "星期日");
	lv_label_set_long_mode(ui->screen_weather_label_week, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_week, -5, 85);
	lv_obj_set_size(ui->screen_weather_label_week, 101, 35);

	//Write style for screen_weather_label_week, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_week, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_week, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_week, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_week, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_week, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_week, &lv_font_STKAITI_22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_week, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_week, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_week, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_week, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_label_city
	ui->screen_weather_label_city = lv_label_create(ui->screen_weather);
	lv_label_set_text(ui->screen_weather_label_city, "深圳");
	lv_label_set_long_mode(ui->screen_weather_label_city, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_weather_label_city, 117, 142);
	lv_obj_set_size(ui->screen_weather_label_city, 100, 40);

	//Write style for screen_weather_label_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_weather_label_city, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_weather_label_city, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_weather_label_city, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_weather_label_city, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_weather_label_city, lv_color_hex(0x00bdff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_weather_label_city, &lv_font_STKAITI_22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_weather_label_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_weather_label_city, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_weather_label_city, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_weather_label_city, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_line_1
	ui->screen_weather_line_1 = lv_line_create(ui->screen_weather);
	static lv_point_t screen_weather_line_1[] ={{0, 60},{240, 60},};
	lv_line_set_points(ui->screen_weather_line_1, screen_weather_line_1, 2);
	lv_obj_set_pos(ui->screen_weather_line_1, 19, 140);
	lv_obj_set_size(ui->screen_weather_line_1, 260, 60);

	//Write style for screen_weather_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->screen_weather_line_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->screen_weather_line_1, lv_color_hex(0x4cd2e0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->screen_weather_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->screen_weather_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_img_bat
	ui->screen_weather_img_bat = lv_img_create(ui->screen_weather);
	lv_obj_add_flag(ui->screen_weather_img_bat, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_weather_img_bat, &_battery_alpha_22x18);
	lv_img_set_pivot(ui->screen_weather_img_bat, 50,50);
	lv_img_set_angle(ui->screen_weather_img_bat, 0);
	lv_obj_set_pos(ui->screen_weather_img_bat, 250, 6);
	lv_obj_set_size(ui->screen_weather_img_bat, 22, 18);

	//Write style for screen_weather_img_bat, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_weather_img_bat, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_img_wifi
	ui->screen_weather_img_wifi = lv_img_create(ui->screen_weather);
	lv_obj_add_flag(ui->screen_weather_img_wifi, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_weather_img_wifi, &_WIFI_alpha_20x20);
	lv_img_set_pivot(ui->screen_weather_img_wifi, 50,50);
	lv_img_set_angle(ui->screen_weather_img_wifi, 0);
	lv_obj_set_pos(ui->screen_weather_img_wifi, 186, 4);
	lv_obj_set_size(ui->screen_weather_img_wifi, 20, 20);
	lv_obj_add_flag(ui->screen_weather_img_wifi, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_weather_img_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_weather_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_img_nowifi
	ui->screen_weather_img_nowifi = lv_img_create(ui->screen_weather);
	lv_obj_add_flag(ui->screen_weather_img_nowifi, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_weather_img_nowifi, &_ic_wifi_alpha_20x20);
	lv_img_set_pivot(ui->screen_weather_img_nowifi, 50,50);
	lv_img_set_angle(ui->screen_weather_img_nowifi, 0);
	lv_obj_set_pos(ui->screen_weather_img_nowifi, 226, 3);
	lv_obj_set_size(ui->screen_weather_img_nowifi, 20, 20);
	lv_obj_add_flag(ui->screen_weather_img_nowifi, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_weather_img_nowifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_weather_img_nowifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_img_WIFI_G
	ui->screen_weather_img_WIFI_G = lv_img_create(ui->screen_weather);
	lv_obj_add_flag(ui->screen_weather_img_WIFI_G, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_weather_img_WIFI_G, &_WIFI_G_alpha_20x20);
	lv_img_set_pivot(ui->screen_weather_img_WIFI_G, 50,50);
	lv_img_set_angle(ui->screen_weather_img_WIFI_G, 0);
	lv_obj_set_pos(ui->screen_weather_img_WIFI_G, 225, 4);
	lv_obj_set_size(ui->screen_weather_img_WIFI_G, 20, 20);

	//Write style for screen_weather_img_WIFI_G, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_weather_img_WIFI_G, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_weather_animimg_1
	ui->screen_weather_animimg_1 = lv_animimg_create(ui->screen_weather);
	lv_animimg_set_src(ui->screen_weather_animimg_1, (const void **) screen_weather_animimg_1_imgs, 48, false);
	lv_animimg_set_duration(ui->screen_weather_animimg_1, 100*48);
	lv_animimg_set_repeat_count(ui->screen_weather_animimg_1, LV_ANIM_REPEAT_INFINITE);
	lv_animimg_start(ui->screen_weather_animimg_1);
	lv_obj_set_pos(ui->screen_weather_animimg_1, 52, 110);
	lv_obj_set_size(ui->screen_weather_animimg_1, 80, 93);

	//The custom code of screen_weather.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_weather);

}
