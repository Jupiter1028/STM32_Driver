#ifndef __UI_PORT_H
#define __UI_PORT_H

#include <stdint.h>
#include "u8g2.h"

// 抽象按键事件（与 flexible_button 和具体硬件解耦）
typedef enum {
    UI_KEY_NONE = 0,

    /* OK 键 (id=0) */
    UI_KEY_OK_CLICK,
    UI_KEY_OK_DOUBLE,
    UI_KEY_OK_LONG,

    /* DOWN 键 (id=1) */
    UI_KEY_DOWN_CLICK,
    UI_KEY_DOWN_DOUBLE,
    UI_KEY_DOWN_LONG,

    /* UP 键 (id=2) */
    UI_KEY_UP_CLICK,
    UI_KEY_UP_DOUBLE,
    UI_KEY_UP_LONG,

} ui_key_t;

ui_key_t ui_port_get_key(uint32_t timeout_ms);

// 移植接口 API
void     ui_port_delay(uint32_t ms);          // 替代 HAL_Delay / osDelay
uint32_t ui_port_get_tick(void);              // 替代 HAL_GetTick
ui_key_t ui_port_get_key(uint32_t timeout);   // 替代 osMessageGet 阻塞等按键

#endif
