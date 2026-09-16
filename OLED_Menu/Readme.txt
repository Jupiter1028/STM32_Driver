这份 OLED_Menu 移植与使用说明 专为嵌入式环境设计，采用三层解耦架构，方便在不同 MCU 和 RTOS 之间快速移植。

一、 依赖项 (Dependencies)本项目依赖以下开源库与系统环境：u8g2: 用于 OLED 屏幕的底层驱动与图形绘制。flexible_button: 用于按键去抖、单击、双击及长按事件的识别。
RTOS: 示例基于 CMSIS-RTOS (FreeRTOS) 实现任务调度与消息队列通信。

二、 核心架构系统分为解耦的三个层级：移植层 (ui_port.c/.h)：接管硬件与 OS。负责延时、获取系统 Tick，以及将底层的物理按键事件映射为 UI 虚拟按键。核心层 (ui_core.c/.h)：纯 C 语言编写的菜单引擎。
负责列表渲染、滚动条计算与光标控制，无需修改。应用层 (ui_app.c/.h)：用户业务逻辑。包含你的具体业务页面（如 DAC 调节、设置页面）及菜单项数组的定义。

三、 移植步骤 (Porting)
1. 准备底层驱动确保你的工程中已成功移植 u8g2 并能点亮屏幕，同时 flexible_button 能在定时器或任务中正常扫描并产生事件。
2. 配置移植层 (ui_port.c)每次移植到新硬件，
只需修改此文件：
延时与时间：替换为你当前 OS 的 API。
void ui_port_delay(uint32_t ms) { osDelay(ms); }
uint32_t ui_port_get_tick(void) { return HAL_GetTick(); }

按键映射：在 ui_port_get_key(uint32_t timeout) 函数中，接收 flexible_button 发出的队列消息，并将其映射为标准的虚拟按键（如 UI_KEY_UP_CLICK、UI_KEY_OK_DOUBLE 等）。
代码中已包含利用 100ms 超时机制实现“长按连发”的处理。

#include "ui_port.h"
#include "cmsis_os.h"
#include "main.h" 
#include "flexible_button.h"
#include <stdio.h> // 需要包含printf头文件

extern osMessageQId Key_MessageQueue;

/* ---- 1. flexible_button 按键事件回调函数 ---- */
static void btn_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    // 过滤掉无动作事件
    if (btn->event != FLEX_BTN_PRESS_NONE)
    {
        // 打印调试日志
        printf("[KEY LOG] ID: %d, Event: %d\r\n", btn->id, btn->event);
        
        // 拼接：高16位存储 event，低16位存储 id
        uint32_t msg = ((uint32_t)btn->event << 16) | ((uint32_t)btn->id & 0xFFFF);
        
        // 发送消息到队列（超时为 0，中断中安全）
        osMessagePut(Key_MessageQueue, msg, 0);
    }
}

3. 启动 UI 任务在你的 RTOS 任务中，包含 ui_app.h 并按照以下范式调用：
	#include "ui_app.h"

	void StartUITask(void const * argument) {
		// 1. 硬件/屏幕初始化
		u8g2Init(&u8g2);
		
		// 2. 开机画面 (按需添加)
		draw(&u8g2);
		osDelay(1000); // 延时保持开机画面
		
		// 3. 移交控制权给 UI 引擎
		while(1) {
			// App_UI_Init 内部包含了菜单绘制、按键获取和页面跳转的死循环
			App_UI_Init(&u8g2); 
			
			// 安全机制：防止在最顶层主菜单触发“退出”导致任务跑飞
			osDelay(10); 
		}
	}

四、 如何添加自定义菜单在 ui_app.c 中添加新页面极其简单：
步骤 1：编写业务函数遵循 void Func_Name(u8g2_t *u8g2) 格式。使用 ui_port_get_key 获取按键并处理退出标志：Cvoid MyCustom_Func(u8g2_t *u8g2) {
    uint8_t exit_flag = 0;
    while (!exit_flag) {
        u8g2_ClearBuffer(u8g2);
        u8g2_DrawStr(u8g2, 10, 30, "Hello World");
        u8g2_SendBuffer(u8g2);
        
        ui_key_t key = ui_port_get_key(0xFFFFFFFF);
        if (key == UI_KEY_OK_DOUBLE) exit_flag = 1; // 约定双击退出
    }
}

步骤 2：注册到菜单数组将函数挂载到任意级别的菜单数组中即可：Cstatic ui_menu_item_t main_items[] = {
    { "1. System Status", Status_Func  },
    { "2. My Custom App", MyCustom_Func }, // 新增的菜单
};
五、 默认交互操作指南本系统默认采用极简的 3 键操作逻辑：
物理按键		操作方式		系统行为响应
K2 (UP)		短按			光标上移 / 数值步进增加
			长按			快速连续上移 / 快速连续增加
K1 (DOWN)	短按			光标下移 / 数值步进减少
			长按			快速连续下移 / 快速连续减少
K0 (OK)		短按			进入子菜单 / 切换焦点 / 确认
			双击			取消 / 返回上一级菜单