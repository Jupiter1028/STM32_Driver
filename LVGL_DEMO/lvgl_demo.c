#include "lvgl_demo.h"

lv_obj_t * home_screen ;
lv_obj_t * setting_screen ;
lv_obj_t * city_label;


void Home_screen_init(void){
	home_screen = lv_obj_create(NULL);		//创建主页屏幕
	lv_obj_set_size( home_screen, 280, 240 );	//设置大小
	lv_obj_set_pos(home_screen, 0, 0);//设置位置
//	lv_scr_load(home_screen);							//加载屏幕
//	lv_scr_act();													//获取当前活动屏幕
	city_label = lv_label_create(home_screen);
	lv_obj_set_size( city_label, 80, 40 );	//设置大小
	lv_obj_set_pos(city_label, 20, 0);//设置位置
}










