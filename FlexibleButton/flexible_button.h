/**
 * @File: flexible_button.h
 * @Author: MurphyZhao
 * @Date: 2018-09-29
 *  https://mp.weixin.qq.com/s/3pAGKiO_BYFXqDSVSzaUVQ
 *  https://github.com/murphyzhao/FlexibleButton  
 */

#ifndef __FLEXIBLE_BUTTON_H__
#define __FLEXIBLE_BUTTON_H__

#include "stdint.h"
#include "main.h"

#define FLEX_BTN_SCAN_FREQ_HZ   50 				// 按键扫描频率,设置50hz比较合适
#define FLEX_MS_TO_SCAN_CNT(ms) (ms / (1000 / FLEX_BTN_SCAN_FREQ_HZ))     

/* 点击间隔时间, 默认 300ms */
#define MAX_MULTIPLE_CLICKS_INTERVAL (FLEX_MS_TO_SCAN_CNT(300))

typedef void (*flex_button_response_callback)(void*); //定义一个函数指针

typedef enum
{
	FLEX_BTN_PRESS_DOWN = 0, // 按下事件
	FLEX_BTN_PRESS_CLICK, // 单击事件
	FLEX_BTN_PRESS_DOUBLE_CLICK, // 双击事件
	FLEX_BTN_PRESS_REPEAT_CLICK, // 连击事件，使用 flex_button_t 中的 click_cnt 断定连击次数
	FLEX_BTN_PRESS_SHORT_START, // 短按开始事件
	FLEX_BTN_PRESS_SHORT_UP, // 短按抬起事件
	FLEX_BTN_PRESS_LONG_START, // 长按开始事件
	FLEX_BTN_PRESS_LONG_UP, // 长按抬起事件
	FLEX_BTN_PRESS_LONG_HOLD, // 长按保持事件
	FLEX_BTN_PRESS_LONG_HOLD_UP, // 长按保持的抬起事件
	FLEX_BTN_PRESS_MAX,
	FLEX_BTN_PRESS_NONE,
}flex_button_event_t;

typedef struct flex_button
{
	struct flex_button* next; // 按键库使用单向链表串起所有的按键
	uint8_t (*usr_button_read)(void *); // 用户设备的按键引脚电平读取函数，重要
	flex_button_response_callback cb; // 设置按键事件回调，用于应用层对按键事件的分类处理
	uint16_t scan_cnt; // 用于记录扫描次数，按键按下是开始从零计数
	uint16_t click_cnt; // 记录单击次数，用于判定单击、连击
	uint16_t max_multiple_clicks_interval; // 连击间隙，用于判定是否结束连击计数，有默认值 
	uint16_t debounce_tick; // 消抖时间，暂未使用，依靠扫描间隙进行消抖
	uint16_t short_press_start_tick; // 设置短按事件触发的起始 tick
	uint16_t long_press_start_tick; // 设置长按事件触发的起始 tick
	uint16_t long_hold_start_tick; // 设置长按保持事件触发的起始 tick
	uint8_t id; // 当多个按键使用同一个回调函数时，用于断定属于哪个按键
	uint8_t pressed_logic_level : 1; // 设置按键按下的逻辑电平
	uint8_t event : 4; // 用于记录当前按键事件
	uint8_t status : 3; // 用于记录当前按键的状态，用于内部状态机
}flex_button_t;

#define USER_BUTTON_PORT     GPIOB
	
#define USER_BUTTON_0_PIN   GPIO_PIN_0
#define USER_BUTTON_1_PIN   GPIO_PIN_1   
#define USER_BUTTON_2_PIN   GPIO_PIN_2  
	
#define Read_Key0_State()   HAL_GPIO_ReadPin(USER_BUTTON_PORT,USER_BUTTON_0_PIN)
#define Read_Key1_State()   HAL_GPIO_ReadPin(USER_BUTTON_PORT,USER_BUTTON_1_PIN)
#define Read_Key2_State()   HAL_GPIO_ReadPin(USER_BUTTON_PORT,USER_BUTTON_2_PIN)
	
typedef enum
{
    USER_BUTTON_0 = 0,
		USER_BUTTON_1,
    USER_BUTTON_2,
//		USER_BUTTON_3,
		USER_BUTTON_MAX
}user_button_t;
	
int32_t flex_button_register(flex_button_t *button);   //按键注册 
flex_button_event_t flex_button_event_read(flex_button_t* button);  //读取按键事件  

void user_button_init(void);          //主函数中需要调用初始化函数
uint8_t flex_button_scan(void);       //按键扫描函数，20ms扫描一次比较合适，FLEX_BTN_SCAN_FREQ_HZ设置50
	

#endif /* __FLEXIBLE_BUTTON_H__ */ 



