#include "ui_port.h"
#include "cmsis_os.h"
#include "main.h" 
#include "flexible_button.h"

extern osMessageQId Key_MessageQueue;

void ui_port_delay(uint32_t ms) {
    osDelay(ms); 
}

uint32_t ui_port_get_tick(void) {
    return HAL_GetTick();
}

// 记录当前是否处于长按连发状态
static ui_key_t holding_key = UI_KEY_NONE;

/* ============================================================
 *  从 Key_MessageQueue 取按键消息并翻译成 ui_key_t
 *
 *  消息格式（由 btn_cb 打包）：
 *    bit 31~16 : flex_button_event_t
 *    bit 15~0  : button id
 *
 *  按键 id 约定：
 *    0 = OK, 1 = DOWN, 2 = UP
 * ============================================================ */
ui_key_t ui_port_get_key(uint32_t timeout_ms)
{
    osEvent event = osMessageGet(Key_MessageQueue, timeout_ms);
    if (event.status != osEventMessage) {
        return UI_KEY_NONE;         /* 超时，无按键 */
    }

    uint32_t msg       = (uint32_t)event.value.v;
    uint8_t  btn_id    = (uint8_t)(msg & 0xFFFF);
    uint8_t  btn_event = (uint8_t)((msg >> 16) & 0xFFFF);

    switch (btn_event)
    {
    /* ---------------- 单击 ---------------- */
    case FLEX_BTN_PRESS_CLICK:
        switch (btn_id) {
        case 0: return UI_KEY_OK_CLICK;
        case 1: return UI_KEY_DOWN_CLICK;
        case 2: return UI_KEY_UP_CLICK;
        }
        break;

    /* ---------------- 双击 ---------------- */
    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        switch (btn_id) {
        case 0: return UI_KEY_OK_DOUBLE;
        case 1: return UI_KEY_DOWN_DOUBLE;
        case 2: return UI_KEY_UP_DOUBLE;
        }
        break;

    /* ---------------- 长按开始 / 长按保持 ---------------- */
    case FLEX_BTN_PRESS_LONG_START:
    case FLEX_BTN_PRESS_LONG_HOLD:      /* 持续触发，让调用方节流 */
        switch (btn_id) {
        case 0: return UI_KEY_OK_LONG;
        case 1: return UI_KEY_DOWN_LONG;
        case 2: return UI_KEY_UP_LONG;
        }
        break;

    /* ---------------- 其它事件不向上抛 ---------------- */
    case FLEX_BTN_PRESS_DOWN:
    case FLEX_BTN_PRESS_SHORT_START:
    case FLEX_BTN_PRESS_SHORT_UP:
    case FLEX_BTN_PRESS_LONG_UP:
    case FLEX_BTN_PRESS_LONG_HOLD_UP:
    case FLEX_BTN_PRESS_REPEAT_CLICK:
    default:
        break;
    }

    return UI_KEY_NONE;
}
