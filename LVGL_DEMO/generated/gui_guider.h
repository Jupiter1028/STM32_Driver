/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
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
  
	lv_obj_t *screen_weather;
	bool screen_weather_del;
	lv_obj_t *g_kb_screen_weather;
	lv_obj_t *screen_weather_weather_icon;
	lv_obj_t *screen_weather_label_temp;
	lv_obj_t *screen_weather_in_temp;
	lv_obj_t *screen_weather_in_humi;
	lv_obj_t *screen_weather_label_humi;
	lv_obj_t *screen_weather_label_temp_out;
	lv_obj_t *screen_weather_label_date;
	lv_obj_t *screen_weather_label_time;
	lv_obj_t *screen_weather_label_second;
	lv_obj_t *screen_weather_label_week;
	lv_obj_t *screen_weather_label_city;
	lv_obj_t *screen_weather_line_1;
	lv_obj_t *screen_weather_img_bat;
	lv_obj_t *screen_weather_img_wifi;
	lv_obj_t *screen_weather_img_nowifi;
	lv_obj_t *screen_weather_img_WIFI_G;
	lv_obj_t *screen_weather_animimg_1;
	lv_obj_t *screen_setting;
	bool screen_setting_del;
	lv_obj_t *g_kb_screen_setting;
	lv_obj_t *screen_setting_list_setting;
	lv_obj_t *screen_setting_list_setting_item0;
	lv_obj_t *screen_setting_list_setting_item1;
	lv_obj_t *screen_setting_list_setting_item2;
	lv_obj_t *screen_setting_list_setting_item3;
	lv_obj_t *screen_setting_list_setting_item4;
	lv_obj_t *screen_setting_list_setting_item5;
	lv_obj_t *screen_setting_list_setting_item6;
	lv_obj_t *screen_setting_label_set;
	lv_obj_t *screen_2;
	bool screen_2_del;
	lv_obj_t *g_kb_screen_2;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

//void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
//                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

//void ui_move_animation(void * var, int32_t duration, int32_t delay, int32_t x_end, int32_t y_end, lv_anim_path_cb_t path_cb,
//                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
//                       lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

//void ui_scale_animation(void * var, int32_t duration, int32_t delay, int32_t width, int32_t height, lv_anim_path_cb_t path_cb,
//                        uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
//                        lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

//void ui_img_zoom_animation(void * var, int32_t duration, int32_t delay, int32_t zoom, lv_anim_path_cb_t path_cb,
//                           uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
//                           lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

//void ui_img_rotate_animation(void * var, int32_t duration, int32_t delay, lv_coord_t x, lv_coord_t y, int32_t rotate,
//                   lv_anim_path_cb_t path_cb, uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time,
//                   uint32_t playback_delay, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;

void setup_scr_screen_weather(lv_ui *ui);
void setup_scr_screen_setting(lv_ui *ui);
void setup_scr_screen_2(lv_ui *ui);
LV_IMG_DECLARE(_cloudy_alpha_100x100);
LV_IMG_DECLARE(_battery_alpha_22x18);
LV_IMG_DECLARE(_WIFI_alpha_20x20);
LV_IMG_DECLARE(_ic_wifi_alpha_20x20);
LV_IMG_DECLARE(_WIFI_G_alpha_20x20);
//#include "extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(screen_weather_animimg_1running1_0);
LV_IMG_DECLARE(screen_weather_animimg_1running1_1);
LV_IMG_DECLARE(screen_weather_animimg_1running1_2);
LV_IMG_DECLARE(screen_weather_animimg_1running1_3);
LV_IMG_DECLARE(screen_weather_animimg_1running1_4);
LV_IMG_DECLARE(screen_weather_animimg_1running1_5);
LV_IMG_DECLARE(screen_weather_animimg_1running1_6);
LV_IMG_DECLARE(screen_weather_animimg_1running1_7);
LV_IMG_DECLARE(screen_weather_animimg_1running1_8);
LV_IMG_DECLARE(screen_weather_animimg_1running1_9);
LV_IMG_DECLARE(screen_weather_animimg_1running1_10);
LV_IMG_DECLARE(screen_weather_animimg_1running1_11);
LV_IMG_DECLARE(screen_weather_animimg_1running1_12);
LV_IMG_DECLARE(screen_weather_animimg_1running1_13);
LV_IMG_DECLARE(screen_weather_animimg_1running1_14);
LV_IMG_DECLARE(screen_weather_animimg_1running1_15);
LV_IMG_DECLARE(screen_weather_animimg_1running1_16);
LV_IMG_DECLARE(screen_weather_animimg_1running1_17);
LV_IMG_DECLARE(screen_weather_animimg_1running1_18);
LV_IMG_DECLARE(screen_weather_animimg_1running1_19);
LV_IMG_DECLARE(screen_weather_animimg_1running1_20);
LV_IMG_DECLARE(screen_weather_animimg_1running1_21);
LV_IMG_DECLARE(screen_weather_animimg_1running1_22);
LV_IMG_DECLARE(screen_weather_animimg_1running1_23);
LV_IMG_DECLARE(screen_weather_animimg_1running1_0);
LV_IMG_DECLARE(screen_weather_animimg_1running1_1);
LV_IMG_DECLARE(screen_weather_animimg_1running1_2);
LV_IMG_DECLARE(screen_weather_animimg_1running1_3);
LV_IMG_DECLARE(screen_weather_animimg_1running1_4);
LV_IMG_DECLARE(screen_weather_animimg_1running1_5);
LV_IMG_DECLARE(screen_weather_animimg_1running1_6);
LV_IMG_DECLARE(screen_weather_animimg_1running1_7);
LV_IMG_DECLARE(screen_weather_animimg_1running1_8);
LV_IMG_DECLARE(screen_weather_animimg_1running1_9);
LV_IMG_DECLARE(screen_weather_animimg_1running1_10);
LV_IMG_DECLARE(screen_weather_animimg_1running1_11);
LV_IMG_DECLARE(screen_weather_animimg_1running1_12);
LV_IMG_DECLARE(screen_weather_animimg_1running1_13);
LV_IMG_DECLARE(screen_weather_animimg_1running1_14);
LV_IMG_DECLARE(screen_weather_animimg_1running1_15);
LV_IMG_DECLARE(screen_weather_animimg_1running1_16);
LV_IMG_DECLARE(screen_weather_animimg_1running1_17);
LV_IMG_DECLARE(screen_weather_animimg_1running1_18);
LV_IMG_DECLARE(screen_weather_animimg_1running1_19);
LV_IMG_DECLARE(screen_weather_animimg_1running1_20);
LV_IMG_DECLARE(screen_weather_animimg_1running1_21);
LV_IMG_DECLARE(screen_weather_animimg_1running1_22);
LV_IMG_DECLARE(screen_weather_animimg_1running1_23);

LV_FONT_DECLARE(lv_font_STKAITI_22)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_STKAITI_25)
LV_FONT_DECLARE(lv_font_Acme_Regular_18)
LV_FONT_DECLARE(lv_font_Acme_Regular_34)
LV_FONT_DECLARE(lv_font_montserratMedium_22)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_Acme_Regular_25)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18)


#ifdef __cplusplus
}
#endif
#endif
