#include "ui_core.h"

#define MAX_VISIBLE_LINES  4

// 通用的列表绘制引擎（从你原有的 Settings_Func 提取）
static void ui_core_draw_list(u8g2_t *u8g2, const char* title, ui_menu_item_t *items, uint8_t item_count, uint8_t cursor, uint8_t *frame_start)
{
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
    u8g2_DrawStr(u8g2, 2, 12, title);

    if (cursor < *frame_start) {
        *frame_start = cursor;
    }
    if (cursor >= *frame_start + MAX_VISIBLE_LINES) {
        *frame_start = cursor - MAX_VISIBLE_LINES + 1;
    }

    for (uint8_t i = 0; i < MAX_VISIBLE_LINES; i++) {
        uint8_t idx = *frame_start + i;
        if (idx >= item_count) break;

        int16_t y_pos = 28 + (i * 12);
        if (idx == cursor) {
            u8g2_SetDrawColor(u8g2, 1);
            u8g2_DrawBox(u8g2, 0, y_pos - 10, 128, 12);
            u8g2_SetDrawColor(u8g2, 0);
        } else {
            u8g2_SetDrawColor(u8g2, 1);
        }
        u8g2_DrawStr(u8g2, 5, y_pos, items[idx].name);
    }

    /* 滚动条 */
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawVLine(u8g2, 126, 18, 44);
    if (item_count > MAX_VISIBLE_LINES) {
        u8g2_DrawBox(u8g2, 125, 18 + (*frame_start * 44 / item_count), 3, (MAX_VISIBLE_LINES * 44 / item_count));
    } else {
        u8g2_DrawBox(u8g2, 125, 18, 3, 44);
    }
    u8g2_SendBuffer(u8g2);
}

// 通用的菜单交互运行器
void ui_core_run_menu(u8g2_t *u8g2, const char* title, ui_menu_item_t *items, uint8_t item_count)
{
    uint8_t exit_flag = 0;
    uint8_t cursor = 0;
    uint8_t frame_start = 0;

    while (!exit_flag) {
        ui_core_draw_list(u8g2, title, items, item_count, cursor, &frame_start);
        
        ui_key_t key = ui_port_get_key(0xFFFFFFFF);
        
        switch (key) {
            case UI_KEY_UP_CLICK:
            case UI_KEY_UP_LONG:
                cursor = (cursor == 0) ? (item_count - 1) : (cursor - 1);
                break;
                
            case UI_KEY_DOWN_CLICK:
            case UI_KEY_DOWN_LONG:
                cursor = (cursor + 1) % item_count;
                break;
                
            case UI_KEY_OK_CLICK:
                if (cursor == 0 && items[0].func == NULL) { // 约定：Func 为 NULL 的第一项作为 "Back" 返回
                    exit_flag = 1;
                } else if (items[cursor].func) {
                    items[cursor].func(u8g2); // 进入子页面
                }
                break;
                
            case UI_KEY_OK_DOUBLE: 
                exit_flag = 1; // 双击统一作为全局返回上一级
                break;
                
            default: break;
        }
    }
}