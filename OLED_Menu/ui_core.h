#ifndef __UI_CORE_H
#define __UI_CORE_H

#include "u8g2.h"
#include "ui_port.h"

typedef struct {
    const char *name;
    void (*func)(u8g2_t *);
} ui_menu_item_t;

// ºËÐÄ²Ëµ¥ÒýÇæ API
void ui_core_run_menu(u8g2_t *u8g2, const char* title, ui_menu_item_t *items, uint8_t item_count);

#endif


