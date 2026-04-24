#ifndef __LVGL_KEY_H
#define __LVGL_KEY_H

#include "lvgl.h"           /* 引用LittlevGL主头文件 */
#include "lv_port_disp.h"   /* 引用与显示器相关的头文件 */
#include "lv_port_indev.h"  /* 引用与输入设备相关的头文件 */

void MyKEY_Init(void);
uint32_t MyKEY_Read(void);




#endif


