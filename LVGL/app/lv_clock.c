#include "lv_clock.h"

static lv_obj_t * screen;
static lv_obj_t * label_time;
static lv_obj_t * label_date;
 
void create_time_label(void)
{
	// 创建一个基本容器
	screen = lv_obj_create(lv_scr_act());  //在当前激活的屏幕对象创建一个容器

	// 创建一个页面容器
	lv_obj_t *page_container = lv_obj_create(screen);
	lv_obj_set_size(page_container, LV_HOR_RES, LV_VER_RES);  // 设置页面容器大小为整个屏幕大小

	// 创建时间标签
	label_time = lv_label_create(page_container);
	lv_obj_align(label_time, NULL,  0, 20);
	lv_label_set_text(label_time, "00:00");

	// 创建日期标签
	label_date = lv_label_create(page_container);
	lv_obj_align(label_date, NULL,0, 60);
	lv_label_set_text(label_date, "1 January");
}
 
//void update_time_label(RTC_DateTypeDef *Date,RTC_TimeTypeDef *Time)
//{
//		lv_label_set_text_fmt(label_date,"%02d,%02d,%02d %02d:%02d:%02d",Date->Year,Date->Month,Date->Date,Time->Hours,Time->Minutes,Time->Seconds);
//}
// 
