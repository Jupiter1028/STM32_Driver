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



void setup_scr_screen_setting(lv_ui *ui)
{
	//Write codes screen_setting
	ui->screen_setting = lv_obj_create(NULL);
	ui->g_kb_screen_setting = lv_zh_keyboard_create(ui->screen_setting, &lv_font_SourceHanSerifSC_Regular_18);
	lv_obj_add_event_cb(ui->g_kb_screen_setting, kb_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(ui->g_kb_screen_setting, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(ui->g_kb_screen_setting, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_size(ui->screen_setting, 280, 240);
	lv_obj_set_scrollbar_mode(ui->screen_setting, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_setting, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_setting, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_setting_list_setting
	ui->screen_setting_list_setting = lv_list_create(ui->screen_setting);
	ui->screen_setting_list_setting_item0 = lv_list_add_btn(ui->screen_setting_list_setting, LV_SYMBOL_WIFI, "WIFI");
	ui->screen_setting_list_setting_item1 = lv_list_add_btn(ui->screen_setting_list_setting, LV_SYMBOL_HOME, "城市设置");
	ui->screen_setting_list_setting_item2 = lv_list_add_btn(ui->screen_setting_list_setting, LV_SYMBOL_SHUFFLE, "时间设置");
	ui->screen_setting_list_setting_item3 = lv_list_add_btn(ui->screen_setting_list_setting, LV_SYMBOL_CHARGE, "背光设置");
	ui->screen_setting_list_setting_item4 = lv_list_add_btn(ui->screen_setting_list_setting, LV_SYMBOL_VOLUME_MID, "音量");
	ui->screen_setting_list_setting_item5 = lv_list_add_btn(ui->screen_setting_list_setting, LV_SYMBOL_BELL, "闹钟设置");
	ui->screen_setting_list_setting_item6 = lv_list_add_btn(ui->screen_setting_list_setting, LV_SYMBOL_SAVE, "显示设置");
	lv_obj_set_pos(ui->screen_setting_list_setting, 20, 50);
	lv_obj_set_size(ui->screen_setting_list_setting, 200, 170);
	lv_obj_set_scrollbar_mode(ui->screen_setting_list_setting, LV_SCROLLBAR_MODE_AUTO);

	//Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_setting_main_main_default
	static lv_style_t style_screen_setting_list_setting_main_main_default;
	ui_init_style(&style_screen_setting_list_setting_main_main_default);
	
	lv_style_set_pad_top(&style_screen_setting_list_setting_main_main_default, 5);
	lv_style_set_pad_left(&style_screen_setting_list_setting_main_main_default, 5);
	lv_style_set_pad_right(&style_screen_setting_list_setting_main_main_default, 5);
	lv_style_set_pad_bottom(&style_screen_setting_list_setting_main_main_default, 5);
	lv_style_set_bg_opa(&style_screen_setting_list_setting_main_main_default, 255);
	lv_style_set_bg_color(&style_screen_setting_list_setting_main_main_default, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_screen_setting_list_setting_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_border_width(&style_screen_setting_list_setting_main_main_default, 1);
	lv_style_set_border_opa(&style_screen_setting_list_setting_main_main_default, 255);
	lv_style_set_border_color(&style_screen_setting_list_setting_main_main_default, lv_color_hex(0xa2a2a2));
	lv_style_set_border_side(&style_screen_setting_list_setting_main_main_default, LV_BORDER_SIDE_FULL);
	lv_style_set_radius(&style_screen_setting_list_setting_main_main_default, 5);
	lv_style_set_shadow_width(&style_screen_setting_list_setting_main_main_default, 0);
	lv_obj_add_style(ui->screen_setting_list_setting, &style_screen_setting_list_setting_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_setting_main_scrollbar_default
	static lv_style_t style_screen_setting_list_setting_main_scrollbar_default;
	ui_init_style(&style_screen_setting_list_setting_main_scrollbar_default);
	
	lv_style_set_radius(&style_screen_setting_list_setting_main_scrollbar_default, 3);
	lv_style_set_bg_opa(&style_screen_setting_list_setting_main_scrollbar_default, 255);
	lv_style_set_bg_color(&style_screen_setting_list_setting_main_scrollbar_default, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_screen_setting_list_setting_main_scrollbar_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(ui->screen_setting_list_setting, &style_screen_setting_list_setting_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_setting_extra_btns_main_default
	static lv_style_t style_screen_setting_list_setting_extra_btns_main_default;
	ui_init_style(&style_screen_setting_list_setting_extra_btns_main_default);
	
	lv_style_set_pad_top(&style_screen_setting_list_setting_extra_btns_main_default, 5);
	lv_style_set_pad_left(&style_screen_setting_list_setting_extra_btns_main_default, 5);
	lv_style_set_pad_right(&style_screen_setting_list_setting_extra_btns_main_default, 5);
	lv_style_set_pad_bottom(&style_screen_setting_list_setting_extra_btns_main_default, 5);
	lv_style_set_border_width(&style_screen_setting_list_setting_extra_btns_main_default, 0);
	lv_style_set_text_color(&style_screen_setting_list_setting_extra_btns_main_default, lv_color_hex(0x0D3055));
	lv_style_set_text_font(&style_screen_setting_list_setting_extra_btns_main_default, &lv_font_montserratMedium_22);
	lv_style_set_text_opa(&style_screen_setting_list_setting_extra_btns_main_default, 255);
	lv_style_set_radius(&style_screen_setting_list_setting_extra_btns_main_default, 3);
	lv_style_set_bg_opa(&style_screen_setting_list_setting_extra_btns_main_default, 255);
	lv_style_set_bg_color(&style_screen_setting_list_setting_extra_btns_main_default, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_screen_setting_list_setting_extra_btns_main_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(ui->screen_setting_list_setting_item6, &style_screen_setting_list_setting_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_setting_list_setting_item5, &style_screen_setting_list_setting_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_setting_list_setting_item4, &style_screen_setting_list_setting_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_setting_list_setting_item3, &style_screen_setting_list_setting_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_setting_list_setting_item2, &style_screen_setting_list_setting_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_setting_list_setting_item1, &style_screen_setting_list_setting_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_setting_list_setting_item0, &style_screen_setting_list_setting_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_setting_list_setting_extra_texts_main_default
	static lv_style_t style_screen_setting_list_setting_extra_texts_main_default;
	ui_init_style(&style_screen_setting_list_setting_extra_texts_main_default);
	
	lv_style_set_pad_top(&style_screen_setting_list_setting_extra_texts_main_default, 5);
	lv_style_set_pad_left(&style_screen_setting_list_setting_extra_texts_main_default, 5);
	lv_style_set_pad_right(&style_screen_setting_list_setting_extra_texts_main_default, 5);
	lv_style_set_pad_bottom(&style_screen_setting_list_setting_extra_texts_main_default, 5);
	lv_style_set_border_width(&style_screen_setting_list_setting_extra_texts_main_default, 0);
	lv_style_set_text_color(&style_screen_setting_list_setting_extra_texts_main_default, lv_color_hex(0x0D3055));
	lv_style_set_text_font(&style_screen_setting_list_setting_extra_texts_main_default, &lv_font_montserratMedium_12);
	lv_style_set_text_opa(&style_screen_setting_list_setting_extra_texts_main_default, 255);
	lv_style_set_radius(&style_screen_setting_list_setting_extra_texts_main_default, 3);
	lv_style_set_bg_opa(&style_screen_setting_list_setting_extra_texts_main_default, 255);
	lv_style_set_bg_color(&style_screen_setting_list_setting_extra_texts_main_default, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_screen_setting_list_setting_extra_texts_main_default, LV_GRAD_DIR_NONE);

	//Write codes screen_setting_label_set
	ui->screen_setting_label_set = lv_label_create(ui->screen_setting);
	lv_label_set_text(ui->screen_setting_label_set, "设置");
	lv_label_set_long_mode(ui->screen_setting_label_set, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_setting_label_set, 10, 3);
	lv_obj_set_size(ui->screen_setting_label_set, 60, 40);

	//Write style for screen_setting_label_set, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_setting_label_set, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_setting_label_set, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_setting_label_set, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_setting_label_set, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_setting_label_set, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_setting_label_set, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_setting_label_set, &lv_font_Acme_Regular_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_setting_label_set, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_setting_label_set, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_setting_label_set, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_setting_label_set, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_setting_label_set, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_setting_label_set, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_setting_label_set, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_setting_label_set, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_setting_label_set, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_setting_label_set, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_setting_label_set, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_setting_label_set, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_setting.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_setting);

}
