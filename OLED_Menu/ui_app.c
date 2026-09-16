#include "ui_app.h"
#include "ui_core.h"
#include "ui_port.h"
#include "dac.h"    
#include "adc.h"
#include "i2c.h"
#include "bmp280.h"
#include "rtc.h"
#include <stdio.h>
#include "cmsis_os.h"


int g_screen_brightness = 5;
int g_rotation_index    = 0;

uint16_t DISPLAY_SWITCH_MS = 3000;


/* =========================================================
 * 业务页面函数 
 * ========================================================= */
void Status_Func(u8g2_t *u8g2)
{
    char buf[32];
    uint8_t exit_flag = 0;

    while (!exit_flag)
    {
        RTC_TimeTypeDef sTime;
        RTC_DateTypeDef sDate;
        HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

        u8g2_ClearBuffer(u8g2);
        u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(u8g2, 0, 12, "--- System Info ---");

        sprintf(buf, "Time: %02d:%02d:%02d",
                sTime.Hours, sTime.Minutes, sTime.Seconds);
        u8g2_DrawStr(u8g2, 0, 28, buf);

        sprintf(buf, "Date: %04d-%02d-%02d",
                2000 + sDate.Year, sDate.Month, sDate.Date);
        u8g2_DrawStr(u8g2, 0, 44, buf);

        u8g2_DrawStr(u8g2, 0, 60, "OK to Exit");
        u8g2_SendBuffer(u8g2);

        ui_key_t key = ui_port_get_key(200);
        if (key == UI_KEY_OK_CLICK || key == UI_KEY_OK_DOUBLE) exit_flag = 1;
    }
}

void Brightness_Func(u8g2_t *u8g2) {
    uint8_t exit_flag = 0;
    char buf[16];
    while (!exit_flag) {
        u8g2_ClearBuffer(u8g2);
        u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(u8g2, 10, 20, "--- Brightness ---");
        u8g2_DrawFrame(u8g2, 14, 30, 100, 10);
        u8g2_DrawBox(u8g2, 16, 32, (int)(g_screen_brightness * 9.6), 6);
        sprintf(buf, "Level: %d", g_screen_brightness);
        u8g2_DrawStr(u8g2, 45, 55, buf);
        u8g2_SendBuffer(u8g2);

        ui_key_t key = ui_port_get_key(0xFFFFFFFF);
        if (key == UI_KEY_UP_CLICK || key == UI_KEY_UP_LONG) {
            if (g_screen_brightness < 10) g_screen_brightness++;
        } else if (key == UI_KEY_DOWN_CLICK || key == UI_KEY_DOWN_LONG) {
            if (g_screen_brightness > 0) g_screen_brightness--;
        } else if (key == UI_KEY_OK_CLICK || key == UI_KEY_OK_DOUBLE) {
            exit_flag = 1;
        }
        u8g2_SetContrast(u8g2, g_screen_brightness * 25);
    }
}

#define ADC_VREF_MV  3300U

/* ---- [ADC Test] ADC 采集测试页面 ---- */
void ADC_Test(u8g2_t *u8g2) 
{
    uint8_t exit_flag = 0;
    char buf[32];
    uint16_t adc_raw = 0;
    uint32_t adc_mv = 0;

    while (!exit_flag) {
        // 1. 读取 ADC1 (PA6 / IN6) 数据
        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, 50) == HAL_OK) {
            adc_raw = (uint16_t)HAL_ADC_GetValue(&hadc1);
            adc_mv  = (uint32_t)adc_raw * ADC_VREF_MV / 4095U;
        }
        HAL_ADC_Stop(&hadc1);

        // 2. 界面绘制
        u8g2_ClearBuffer(u8g2);
        u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(u8g2, 10, 12, "--- ADC Test ---");

        // 显示原始值 (RAW)
        sprintf(buf, "PA6 RAW: %d", adc_raw);
        u8g2_DrawStr(u8g2, 6, 30, buf);

        // 显示计算后的电压 (V)
        sprintf(buf, "Voltage: %d.%03dV", adc_mv / 1000, adc_mv % 1000);
        u8g2_DrawStr(u8g2, 6, 46, buf);

        // 底部提示
        u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
        u8g2_DrawStr(u8g2, 2, 62, "Double Click OK to Exit");
        u8g2_SendBuffer(u8g2);

        // 3. 按键处理 (设置 200ms 超时，兼顾实时刷新与按键响应)
        ui_key_t key = ui_port_get_key(200);
        if (key == UI_KEY_OK_DOUBLE) {
            exit_flag = 1; // 双击退出
        }
    }
}


// 优化后的 DAC 界面 (使用通用按键接口)
#define DAC_VREF_MV      3300U
#define DAC_FULL_SCALE   4095U
#define DAC_STEP_SMALL   16U
#define DAC_STEP_LARGE   256U

void DAC_Func(u8g2_t *u8g2) {
    uint8_t  exit_flag   = 0;
    uint8_t  focus       = 0;            /* 0: CH1(PA4), 1: CH2(PA5) */
    uint16_t dac_val[2]  = { 0, 3000 };  /* 原始值 0~4095 */
    uint16_t dac_mv[2]   = { 0, 0 };     /* 电压 mV */
    char     buf[24];
    uint8_t  need_update = 1;
    uint8_t  changed     = 0;

    // 启动 DAC
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_val[0]);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_val[1]);

    while (!exit_flag) {
        
        /* -------- 界面绘制 -------- */
        if (need_update) {
            u8g2_ClearBuffer(u8g2);
            u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
            u8g2_DrawStr(u8g2, 10, 12, "--- DAC Control ---");

            /* CH1 绘制 */
            dac_mv[0] = (uint16_t)((uint32_t)dac_val[0] * DAC_VREF_MV / DAC_FULL_SCALE);
            sprintf(buf, "CH1(PA4):%d.%03dV", dac_mv[0] / 1000, dac_mv[0] % 1000);
            if (focus == 0) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 4, 20, 120, 14); // 绘制高亮反白背景框
                u8g2_SetDrawColor(u8g2, 0);         // 文字设为黑色
            }
            u8g2_DrawStr(u8g2, 6, 31, buf);
            u8g2_SetDrawColor(u8g2, 1);             // 恢复普通绘制颜色

            /* CH2 绘制 */
            dac_mv[1] = (uint16_t)((uint32_t)dac_val[1] * DAC_VREF_MV / DAC_FULL_SCALE);
            sprintf(buf, "CH2(PA5):%d.%03dV", dac_mv[1] / 1000, dac_mv[1] % 1000);
            if (focus == 1) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 4, 36, 120, 14); // 绘制高亮反白背景框
                u8g2_SetDrawColor(u8g2, 0);         // 文字设为黑色
            }
            u8g2_DrawStr(u8g2, 6, 47, buf);
            u8g2_SetDrawColor(u8g2, 1);             // 恢复普通绘制颜色

            /* 底部状态栏绘制 (显示原始 RAW 值) */
            u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
            sprintf(buf, "RAW:%d/%d  SP/LP", dac_val[0], dac_val[1]);
            u8g2_DrawStr(u8g2, 2, 62, buf);

            u8g2_SendBuffer(u8g2);
            need_update = 0;
        }

        /* -------- 按键处理 -------- */
        ui_key_t key = ui_port_get_key(0xFFFFFFFF);
        uint16_t step = 0;

        switch (key) {
            case UI_KEY_UP_CLICK:
            case UI_KEY_UP_LONG:
                // 增加电压
                step = (key == UI_KEY_UP_CLICK) ? DAC_STEP_SMALL : DAC_STEP_LARGE;
                if (dac_val[focus] + step <= DAC_FULL_SCALE) {
                    dac_val[focus] += step;
                } else {
                    dac_val[focus] = DAC_FULL_SCALE; // 防溢出
                }
                changed = 1; 
                need_update = 1;
                break;
                
            case UI_KEY_DOWN_CLICK:
            case UI_KEY_DOWN_LONG:
                // 减少电压
                step = (key == UI_KEY_DOWN_CLICK) ? DAC_STEP_SMALL : DAC_STEP_LARGE;
                if (dac_val[focus] >= step) {
                    dac_val[focus] -= step;
                } else {
                    dac_val[focus] = 0; // 防下溢
                }
                changed = 1; 
                need_update = 1;
                break;
                
            case UI_KEY_OK_CLICK:
                // 短按切换调节通道
                focus = !focus; 
                need_update = 1; 
                break;
                
            case UI_KEY_OK_DOUBLE:
                // 双击退出界面
                exit_flag = 1; 
                break;
                
            default: 
                break;
        }

        /* -------- 执行输出 -------- */
        if (changed) {
            HAL_DAC_SetValue(&hdac, (focus == 0) ? DAC_CHANNEL_1 : DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_val[focus]);
            changed = 0;
        }
    }
}


// [About] 关于页面
void About_Func(u8g2_t *u8g2) {
	uint8_t exit_flag = 0;
    
	while (!exit_flag) {
		u8g2_ClearBuffer(u8g2);
		u8g2_DrawRFrame(u8g2, 0, 0, 128, 64, 10);
		u8g2_DrawCircle(u8g2, 64, 20, 12, U8G2_DRAW_ALL);
		u8g2_DrawStr(u8g2, 62, 24, "i");
		u8g2_SetFont(u8g2, u8g2_font_7x14_tr);
		u8g2_DrawStr(u8g2, 35, 42, "Menu Test");
		u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
		u8g2_DrawStr(u8g2, 30, 55, "Build 2026.09.15");
		u8g2_SendBuffer(u8g2);
		
		// 4. 等待按键退出 (比死延时体验更好)
		ui_key_t key = ui_port_get_key(0xFFFFFFFF);
		if ( key == UI_KEY_OK_DOUBLE) {
				exit_flag = 1;
		}
	}
	
}

// [Help] 按键说明页面
void Help_Func(u8g2_t *u8g2) {
    uint8_t exit_flag = 0;
    
    while (!exit_flag) {
        u8g2_ClearBuffer(u8g2);
        
        // 1. 绘制外边框，圆角半径设为 4
        u8g2_DrawRFrame(u8g2, 0, 0, 128, 64, 4);
        
        // 2. 绘制标题 (使用稍大的字体)
        u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(u8g2, 32, 12, "Key Guide");
        u8g2_DrawHLine(u8g2, 10, 15, 108); // 标题下的分割线
        
        // 3. 绘制按键说明内容 (使用小字体以防超出屏幕)
        u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
        // K2(UP) 说明：短按上/长按连加
        u8g2_DrawStr(u8g2, 8, 28, "K2(UP): Prev / Hold++");
        // K1(DN) 说明：短按下/长按连减
        u8g2_DrawStr(u8g2, 8, 40, "K1(DN): Next / Hold--");
        // K0(OK) 说明：短按确认/双击返回
        u8g2_DrawStr(u8g2, 8, 52, "K0(OK): Sel / D-Clk Back");
        
        u8g2_SendBuffer(u8g2);
        
        // 4. 等待按键退出 (比死延时体验更好)
        ui_key_t key = ui_port_get_key(0xFFFFFFFF);
        if ( key == UI_KEY_OK_DOUBLE) {
            exit_flag = 1;
        }
    }
}


/*****************************Display_Func********************************************************/
#include <stdlib.h>
#include <string.h>

/* ============================================================
 *  随机显示测试页
 *  - 每 3 秒自动切换
 *  - 单击 OK / UP / DOWN 立即换
 *  - 双击 OK 退出
 * ============================================================ */


#define DISPLAY_KEY_POLL_MS  50

/* ---------- 自定义随机数发生器（基于 HAL_GetTick） ---------- */
static uint32_t rng_state = 0;

/* 每次重绘前调用，把 tick 混进去保证不重复 */
static void rng_seed(void)
{
    uint32_t t = HAL_GetTick();
    rng_state ^= (t * 2654435761u) ^ (rng_state << 13) ^ (rng_state >> 17);
    if (rng_state == 0) rng_state = 0xA5A5A5A5u;   /* 避免死锁 0 */
}

/* LCG：返回 32 位随机数 */
static uint32_t rng_next(void)
{
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state;
}

/* [min, max] 的随机整数 */
static uint8_t rand_u8(uint8_t min, uint8_t max)
{
    return min + (uint8_t)((rng_next() >> 16) % (max - min + 1));
}

/* 从数组里随机取一个字符串 */
static const char *rand_str(const char **list, uint8_t n)
{
    return list[rand_u8(0, n - 1)];
}

/* ============================================================ */
void Display_Func(u8g2_t *u8g2)
{
    uint8_t  exit_flag = 0;
    uint8_t  redraw    = 1;

    /* 初始种子 */
    rng_state = HAL_GetTick() | 1;

    uint32_t last_switch = HAL_GetTick();

    while (!exit_flag)
    {
        /* ---------- 自动轮播 ---------- */
        if (HAL_GetTick() - last_switch >= DISPLAY_SWITCH_MS)
        {
            last_switch = HAL_GetTick();
            redraw = 1;
        }

        /* ---------- 重绘 ---------- */
        if (redraw)
        {
            rng_seed();                     /* ★ 每次重绘重新播种 */

            u8g2_ClearBuffer(u8g2);
            u8g2_SetDrawColor(u8g2, 1);

            uint8_t mode = rand_u8(0, 4);

            switch (mode)
            {
            /* ---------- 0. 随机字体 + 随机字符串 + 随机位置 ---------- */
            case 0:
            {
                static const uint8_t *fonts[] = {
                    u8g2_font_4x6_tr,  u8g2_font_5x7_tr,
                    u8g2_font_6x10_tr, u8g2_font_6x12_tr,
                    u8g2_font_7x13_tr, u8g2_font_8x13_tr,
                    u8g2_font_9x15_tr, u8g2_font_10x20_tr,
                    u8g2_font_ncenB08_tr, u8g2_font_ncenB10_tr,
                    u8g2_font_ncenB12_tr, u8g2_font_ncenB14_tr,
                };
                #define FONT_CNT (sizeof(fonts) / sizeof(fonts[0]))

                static const char *words[] = {
                    "STM32", "U8g2", "Hello", "Test","$","^_^","RTC"
                    "ADC", "Good", "RANDOM", "OLED", "@"
                    "FreeRTOS", "I2C", "DAC", "PWM" ,"LED"
                };
                #define WORD_CNT (sizeof(words) / sizeof(words[0]))

                uint8_t lines = rand_u8(2, 4);
                for (uint8_t i = 0; i < lines; i++)
                {
                    u8g2_SetFont(u8g2, fonts[rand_u8(0, FONT_CNT - 1)]);
                    u8g2_DrawStr(u8g2,
                                 rand_u8(0, 60),          /* x 随机 */
                                 rand_u8(12, 62),         /* y 随机 */
                                 rand_str(words, WORD_CNT));
                }

                #undef FONT_CNT
                #undef WORD_CNT
                break;
            }

            /* ---------- 1. 随机图形（数量 + 类型 + 位置全随机） ---------- */
            case 1:
            {
                uint8_t n = rand_u8(5, 20);              /* ★ 数量 5~20 */
                for (uint8_t i = 0; i < n; i++)
                {
                    uint8_t x = rand_u8(0, 127);
                    uint8_t y = rand_u8(0, 63);
                    uint8_t w = rand_u8(4, 40);
                    uint8_t h = rand_u8(4, 30);
                    uint8_t r = rand_u8(2, 18);

                    switch (rand_u8(0, 6))
                    {
                    case 0: u8g2_DrawFrame (u8g2, x, y, w, h); break;
                    case 1: u8g2_DrawBox   (u8g2, x, y, w, h); break;
                    case 2: u8g2_DrawRFrame(u8g2, x, y, w, h, rand_u8(1, 6)); break;
                    case 3: u8g2_DrawRBox  (u8g2, x, y, w, h, rand_u8(1, 6)); break;
                    case 4: u8g2_DrawCircle(u8g2, x, y, r, U8G2_DRAW_ALL); break;
                    case 5: u8g2_DrawDisc  (u8g2, x, y, r, U8G2_DRAW_ALL); break;
                    case 6: u8g2_DrawEllipse(u8g2, x, y, r, r / 2, U8G2_DRAW_ALL); break;
                    }
                }
                break;
            }

            /* ---------- 2. 随机线条 ---------- */
            case 2:
            {
                uint8_t n = rand_u8(10, 40);
                for (uint8_t i = 0; i < n; i++)
                {
                    u8g2_DrawLine(u8g2,
                                  rand_u8(0, 127), rand_u8(0, 63),
                                  rand_u8(0, 127), rand_u8(0, 63));
                }
                break;
            }

            /* ---------- 3. 随机雪花点 ---------- */
            case 3:
            {
                uint16_t n = rand_u8(100, 800);
                for (uint16_t i = 0; i < n; i++)
                    u8g2_DrawPixel(u8g2, rand_u8(0, 127), rand_u8(0, 63));
                break;
            }

            /* ---------- 4. 反色挖洞 ---------- */
            case 4:
            {
                u8g2_DrawBox(u8g2, 0, 0, 128, 64);
                u8g2_SetDrawColor(u8g2, 0);
                uint8_t n = rand_u8(3, 12);
                for (uint8_t i = 0; i < n; i++)
                {
                    u8g2_DrawDisc(u8g2,
                                  rand_u8(0, 127), rand_u8(0, 63),
                                  rand_u8(4, 25), U8G2_DRAW_ALL);
                }
                u8g2_SetDrawColor(u8g2, 1);
                break;
            }
            }

            /* 右下角倒计时 */
            u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
            uint32_t remain = DISPLAY_SWITCH_MS -
                              (HAL_GetTick() - last_switch);
            char tip[12];
            sprintf(tip, "%lus", remain / 1000 + 1);
            u8g2_DrawStr(u8g2, 108, 63, tip);

            u8g2_SendBuffer(u8g2);
            redraw = 0;
        }

        /* ---------- 按键 ---------- */
        ui_key_t key = ui_port_get_key(DISPLAY_KEY_POLL_MS);
        switch (key)
        {
        case UI_KEY_OK_CLICK:
        case UI_KEY_UP_CLICK:
        case UI_KEY_DOWN_CLICK:
        case UI_KEY_UP_LONG:
        case UI_KEY_DOWN_LONG:
            redraw      = 1;
            last_switch = HAL_GetTick();
            break;

        case UI_KEY_OK_DOUBLE:
            exit_flag = 1;
            break;

        default:
            break;
        }
    }
}


/*****************************Display_Speed_Func********************************************************/
void Display_Speed_Func(u8g2_t *u8g2)
{
    uint8_t exit_flag = 0;
    char    buf[16];

    while (!exit_flag)
    {
        u8g2_ClearBuffer(u8g2);
        u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(u8g2, 10, 20, "--- Switch Time ---");

        sprintf(buf, "Interval: %d s", DISPLAY_SWITCH_MS / 1000);
        u8g2_DrawStr(u8g2, 20, 40, buf);

        u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
        u8g2_DrawStr(u8g2, 10, 60, "UP/DN:+/-1s  OK:Save");
        u8g2_SendBuffer(u8g2);

        ui_key_t key = ui_port_get_key(50);
        if (key == UI_KEY_UP_CLICK && DISPLAY_SWITCH_MS < 30000)
            DISPLAY_SWITCH_MS += 1000;
        else if (key == UI_KEY_DOWN_CLICK && DISPLAY_SWITCH_MS > 1000)
            DISPLAY_SWITCH_MS -= 1000;
        else if (key == UI_KEY_OK_CLICK || key == UI_KEY_OK_DOUBLE)
            exit_flag = 1;
    }
}

/*****************************BMP280_Display********************************************************/
/*
 *  BME280 温/压/湿 显示
 *  - 大字温度 + °C
 *  - 气压 hPa + 湿度 %RH
 *  - 海拔 m
 *  - 右上角气压趋势 UP / DN / --
 *  - 单击 OK 立即刷新，双击 OK 退出
 */

#include <math.h>
#include <stdlib.h>

#define BMP280_UPDATE_MS   500

extern void BMP280_Init(void);
extern uint8_t BMP280_HasHumidity(void);
extern void BMP280_CalTempPressHumAlt(int32_t *temperature,
                                      int32_t *pressure,
                                      int32_t *humidity,
                                      int32_t *Altitude);

void BMP280_Display(u8g2_t *u8g2)
{
    uint8_t  exit_flag = 0;
    char     buf[24];

    int32_t  t_cur   = 0;
    int32_t  p_cur   = 0;
    int32_t  h_cur   = 0;
    int32_t  alt_cur = 0;
    int32_t  p_last  = 0;
    int8_t   trend   = 0;

    uint32_t last_update = 0;
    uint8_t  first_run   = 1;

    BMP280_Init();

    while (!exit_flag)
    {
        uint32_t now = HAL_GetTick();

        if (first_run || (now - last_update >= BMP280_UPDATE_MS))
        {
            last_update = now;
            p_last      = p_cur;

            BMP280_CalTempPressHumAlt(&t_cur, &p_cur, &h_cur, &alt_cur);

            if (!first_run)
            {
                if      (p_cur - p_last >  5) trend =  1;
                else if (p_cur - p_last < -5) trend = -1;
                else                          trend =  0;
            }
            first_run = 0;

            /* ============================================================
             *  绘制
             * ============================================================ */
            u8g2_ClearBuffer(u8g2);
            u8g2_SetDrawColor(u8g2, 1);

            /* ---------- 标题 + 趋势 ---------- */
            u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
            u8g2_DrawStr(u8g2, 0, 8, BMP280_HasHumidity() ? "BME280" : "BMP280");

            const char *tr_str = (trend > 0) ? "UP"
                               : (trend < 0) ? "DN" : "--";
            u8g2_DrawStr(u8g2, 112, 8, tr_str);
            u8g2_DrawHLine(u8g2, 0, 10, 128);

            /* ---------- 温度大字 ---------- */
            int32_t t_int = t_cur / 10;
            int32_t t_dec = t_cur % 10;
            if (t_dec < 0) t_dec = -t_dec;
            sprintf(buf, "%d.%d", (int)t_int, (int)t_dec);

            u8g2_SetFont(u8g2, u8g2_font_logisoso24_tr);
            u8g2_DrawStr(u8g2, 4, 38, buf);

            u8g2_DrawCircle(u8g2, 82, 18, 2, U8G2_DRAW_ALL);
            u8g2_SetFont(u8g2, u8g2_font_10x20_tr);
            u8g2_DrawStr(u8g2, 88, 34, "C");

            u8g2_DrawHLine(u8g2, 0, 42, 128);

            /* ---------- 气压 hPa ---------- */
            int32_t p_int = p_cur / 100;
            int32_t p_dec = (p_cur % 100) / 10;
            if (p_dec < 0) p_dec = -p_dec;

            u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
            sprintf(buf, "%d.%d hPa", (int)p_int, (int)p_dec);
            u8g2_DrawStr(u8g2, 0, 54, buf);

            /* ---------- 湿度 %RH ---------- */
            if (BMP280_HasHumidity())
            {
                int32_t h_int = h_cur / 10;
                int32_t h_dec = h_cur % 10;
                if (h_dec < 0) h_dec = -h_dec;

                sprintf(buf, "%d.%d%%", (int)h_int, (int)h_dec);
                u8g2_DrawStr(u8g2, 92, 54, buf);
            }

            /* ---------- 海拔 ---------- */
            u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
            sprintf(buf, "Alt: %d m", (int)alt_cur);
            u8g2_DrawStr(u8g2, 0, 63, buf);

            u8g2_SendBuffer(u8g2);
        }

        /* ---------- 按键 ---------- */
        ui_key_t key = ui_port_get_key(20);
        switch (key)
        {
        case UI_KEY_OK_CLICK:
            first_run = 1;
            break;

        case UI_KEY_OK_DOUBLE:
            exit_flag = 1;
            break;

        default:
            break;
        }
    }
}



/*****************************ADC_Wave********************************************************/

/*
 *  ADC 波形显示（实时打点，20ms/点）
 *  - 环形缓冲，每来一个新点全屏重绘一次
 *  - 屏幕上从左到右 = 最旧 → 最新，波形向左滚动
 *  - 单击 OK 冻结/继续，双击 OK 退出
 */

#define ADC_WAVE_POINTS     128
#define ADC_WAVE_TOP        10
#define ADC_WAVE_BOTTOM     63
#define ADC_WAVE_HEIGHT     (ADC_WAVE_BOTTOM - ADC_WAVE_TOP)

#define ADC_WAVE_SAMPLE_MS  20          /* ★ 20ms 一个点 */

/* ---------- 环形缓冲 ---------- */
static uint16_t s_buf[ADC_WAVE_POINTS];
static uint8_t  s_head = 0;             /* 下一个写入位置（也是最旧的点） */

/* ---------- 单次采样 ---------- */
static uint16_t adc_read_once(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 5);
    uint16_t v = (uint16_t)HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return v;
}

/* ---------- ADC 值 → 屏幕 Y ---------- */
static uint8_t adc_to_y(uint16_t v)
{
    if (v > 4095) v = 4095;
    uint32_t offset = ((uint32_t)v * ADC_WAVE_HEIGHT) / 4095;
    return (uint8_t)(ADC_WAVE_BOTTOM - offset);
}

/* ---------- 全屏重绘 ---------- */
static void adc_wave_draw(u8g2_t *u8g2, uint8_t freeze)
{
    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 1);

    /* 顶部状态：最新点的电压 */
    u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
    uint8_t  new_idx = (s_head + ADC_WAVE_POINTS - 1) % ADC_WAVE_POINTS;
    uint16_t mv      = (uint16_t)((uint32_t)s_buf[new_idx] * 3300 / 4095);
    char     tip[24];
    sprintf(tip, "%d.%02dV %s",
            mv / 1000, (mv % 1000) / 10, freeze ? "HOLD" : "RUN");
    u8g2_DrawStr(u8g2, 0, 8, tip);

    /* 中线（2048）虚线 */
    uint8_t y_mid = adc_to_y(2048);
    for (uint8_t x = 0; x < 128; x += 4)
        u8g2_DrawPixel(u8g2, x, y_mid);

    /* 波形：从 s_head 起按逻辑顺序遍历 = 屏幕左 → 右 */
    uint8_t prev_y = adc_to_y(s_buf[s_head]);
    for (uint8_t i = 1; i < ADC_WAVE_POINTS; i++)
    {
        uint8_t idx = (s_head + i) % ADC_WAVE_POINTS;
        uint8_t y   = adc_to_y(s_buf[idx]);
        u8g2_DrawLine(u8g2, i - 1, prev_y, i, y);
        prev_y = y;
    }

    u8g2_SendBuffer(u8g2);
}

/* ---------- 页面主函数 ---------- */
void ADC_Wave(u8g2_t *u8g2)
{
    uint8_t  exit_flag   = 0;
    uint8_t  freeze      = 0;
    uint32_t last_sample = 0;

    /* 预填缓冲区，避免一进来是空屏 */
    for (uint8_t i = 0; i < ADC_WAVE_POINTS; i++)
    {
        s_buf[i] = adc_read_once();
        osDelay(ADC_WAVE_SAMPLE_MS);
    }
    s_head       = 0;
    last_sample  = HAL_GetTick();

    while (!exit_flag)
    {
        /* ---------- 到点就采一次 + 刷新 ---------- */
        if (!freeze && (HAL_GetTick() - last_sample >= ADC_WAVE_SAMPLE_MS))
        {
            last_sample = HAL_GetTick();

            s_buf[s_head] = adc_read_once();
            s_head = (s_head + 1) % ADC_WAVE_POINTS;

            adc_wave_draw(u8g2, freeze);
        }

        /* ---------- 按键：短超时，保证采样节奏 ---------- */
        ui_key_t key = ui_port_get_key(2);      /* 2ms 轮询 */
        switch (key)
        {
        case UI_KEY_OK_CLICK:
            freeze = !freeze;
            adc_wave_draw(u8g2, freeze);        /* 立即刷新一次状态 */
            break;

        case UI_KEY_OK_DOUBLE:
            exit_flag = 1;
            break;

        default:
            break;
        }
    }
}



/*************************************************************************************/
#define I2C_LINE_MAX        4
#define I2C_PROBE_TIMEOUT   2      /* ms，从 10 降下来 */
#define I2C_PROBE_TRIALS    1
#define I2C_YIELD_EVERY     4      /* 每扫 4 个地址让出一次 CPU */
#define I2C_MAX_DEVS        16     /* 你想显示的上限 */

typedef struct {
    uint8_t bus;    /* 1 起 */
    uint8_t addr;   /* 7 位地址 */
} i2c_dev_t;

static I2C_HandleTypeDef *i2c_bus_list[] = {
    &hi2c1,
    &hi2c2,      
		&hi2c3
};
#define I2C_BUS_NUM  (sizeof(i2c_bus_list) / sizeof(i2c_bus_list[0]))

/* ============ 扫描辅助 ============ */
static void i2c_do_scan(i2c_dev_t *devs, uint8_t *dev_cnt, uint8_t *total_page)
{
    uint8_t yield_cnt = 0;
    *dev_cnt = 0;

    for (uint8_t b = 0; b < I2C_BUS_NUM; b++)
    {
        /* 只扫合法 7 位地址：0x08 ~ 0x77 */
        for (uint8_t addr = 0x08; addr <= 0x77; addr++)
        {
            if (HAL_I2C_IsDeviceReady(i2c_bus_list[b],
                                      (uint16_t)(addr << 1),
                                      I2C_PROBE_TRIALS,
                                      I2C_PROBE_TIMEOUT) == HAL_OK)
            {
                if (*dev_cnt < I2C_MAX_DEVS) {
                    devs[*dev_cnt].bus  = b + 1;
                    devs[*dev_cnt].addr = addr;
                    (*dev_cnt)++;
                }
            }

            /* 关键：周期性让出 CPU，避免系统冻结 */
            if (++yield_cnt >= I2C_YIELD_EVERY) {
                yield_cnt = 0;
                osDelay(1);
            }
        }
    }

    *total_page = (*dev_cnt + I2C_LINE_MAX - 1) / I2C_LINE_MAX;
    if (*total_page == 0) *total_page = 1;

    printf("I2C scan done. %d device(s):\r\n", *dev_cnt);
    for (uint8_t i = 0; i < *dev_cnt; i++)
        printf("  [Bus%d] 0x%02X\r\n", devs[i].bus, devs[i].addr);
}

/* ============ 主界面 ============ */
void I2C_Scan(u8g2_t *u8g2)
{
    i2c_dev_t devs[I2C_MAX_DEVS];
    uint8_t   dev_cnt    = 0;
    uint8_t   total_page = 1;
    uint8_t   page       = 0;
    uint8_t   exit_flag  = 0;
    char      buf[24];

    /* ---------- 扫描前先给出反馈 ---------- */
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
    u8g2_DrawStr(u8g2, 10, 24, "Scanning I2C...");
    u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
    u8g2_DrawStr(u8g2, 10, 42, "Please wait");
    u8g2_SendBuffer(u8g2);

    /* ---------- 首次扫描 ---------- */
    i2c_do_scan(devs, &dev_cnt, &total_page);

    /* ---------- 交互显示循环 ---------- */
    while (!exit_flag)
    {
        u8g2_ClearBuffer(u8g2);
        u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(u8g2, 0, 12, "--- I2C Scan ---");

        uint8_t start = page * I2C_LINE_MAX;
        for (uint8_t i = 0; i < I2C_LINE_MAX; i++)
        {
            uint8_t idx = start + i;
            if (idx >= dev_cnt) break;

            const char *tag = "";
            if      (devs[idx].addr == 0x3C || devs[idx].addr == 0x3D) tag = " OLED";
            else if (devs[idx].addr == 0x68 || devs[idx].addr == 0x69) tag = " RTC";
            else if (devs[idx].addr == 0x76 || devs[idx].addr == 0x77) tag = " BMP";
            else if (devs[idx].addr >= 0x50 && devs[idx].addr <= 0x57) tag = " EEPROM";

            sprintf(buf, "[%d] 0x%02X%s", devs[idx].bus, devs[idx].addr, tag);
            u8g2_DrawStr(u8g2, 6, 26 + i * 12, buf);
        }

        if (dev_cnt == 0)
            u8g2_DrawStr(u8g2, 8, 26, "No device found!");

        u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
        sprintf(buf, "P%d/%d OK:Scan 2xOK:ESC", page + 1, total_page);
        u8g2_DrawStr(u8g2, 0, 62, buf);

        u8g2_SendBuffer(u8g2);

        /* ---------- 按键 ---------- */
        ui_key_t key = ui_port_get_key(0xFFFFFFFF);
        switch (key)
        {
            case UI_KEY_UP_CLICK:
            case UI_KEY_UP_LONG:
                if (total_page > 1)
                    page = (page == 0) ? (total_page - 1) : (page - 1);
                break;

            case UI_KEY_DOWN_CLICK:
            case UI_KEY_DOWN_LONG:
                if (total_page > 1)
                    page = (page + 1) % total_page;
                break;

            case UI_KEY_OK_CLICK:
                /* 重扫前再给一次反馈，顺便清屏 */
                u8g2_ClearBuffer(u8g2);
                u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
                u8g2_DrawStr(u8g2, 10, 32, "Scanning again...");
                u8g2_SendBuffer(u8g2);

                i2c_do_scan(devs, &dev_cnt, &total_page);
                page = 0;
                break;

            case UI_KEY_OK_DOUBLE:
                exit_flag = 1;
                break;

            default:
                break;
        }
    }
}
/*************************************Time_Set_Func************************************************/
/* ============================================================
 *  [Set Time] 时间设置（HAL RTC 版）
 *  - UP/DOWN：+/- 1
 *  - UP/DOWN 长按：+/- 快速
 *  - OK：切换 时 → 分 → 秒 → 保存退出
 *  - 双击 OK：取消退出（不改）
 * ============================================================ */
void Time_Set_Func(u8g2_t *u8g2)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    uint8_t exit_flag   = 0;
    uint8_t focus       = 0;          /* 0=时 1=分 2=秒 */
    uint8_t need_update = 1;
    char    buf[16];

    /* ---- 读取当前 RTC 时间作为初值 ---- */
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);   /* 读 time 后必须读 date */

    uint8_t tmp_h = sTime.Hours;
    uint8_t tmp_m = sTime.Minutes;
    uint8_t tmp_s = sTime.Seconds;

    while (!exit_flag)
    {
        if (need_update)
        {
            u8g2_ClearBuffer(u8g2);
            u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
            u8g2_DrawStr(u8g2, 10, 15, "--- Set Time ---");

            u8g2_SetFont(u8g2, u8g2_font_10x20_me);

            /* 时 */
            sprintf(buf, "%02d", tmp_h);
            if (focus == 0) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 10, 25, 22, 22);
                u8g2_SetDrawColor(u8g2, 0);
            }
            u8g2_DrawStr(u8g2, 12, 43, buf);
            u8g2_SetDrawColor(u8g2, 1);
            u8g2_DrawStr(u8g2, 34, 42, ":");

            /* 分 */
            sprintf(buf, "%02d", tmp_m);
            if (focus == 1) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 44, 25, 22, 22);
                u8g2_SetDrawColor(u8g2, 0);
            }
            u8g2_DrawStr(u8g2, 46, 43, buf);
            u8g2_SetDrawColor(u8g2, 1);
            u8g2_DrawStr(u8g2, 68, 42, ":");

            /* 秒 */
            sprintf(buf, "%02d", tmp_s);
            if (focus == 2) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 78, 25, 22, 22);
                u8g2_SetDrawColor(u8g2, 0);
            }
            u8g2_DrawStr(u8g2, 80, 43, buf);
            u8g2_SetDrawColor(u8g2, 1);

            /* 底部提示 */
            u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
            u8g2_DrawStr(u8g2, 2, 62, "OK:Next  2xOK:Cancel");

            u8g2_SendBuffer(u8g2);
            need_update = 0;
        }

        ui_key_t key = ui_port_get_key(50);
        switch (key)
        {
        /* ---------- 增加 ---------- */
        case UI_KEY_UP_CLICK:
        case UI_KEY_UP_LONG:
            if (focus == 0) tmp_h = (tmp_h + 1) % 24;
            else if (focus == 1) tmp_m = (tmp_m + 1) % 60;
            else                 tmp_s = (tmp_s + 1) % 60;
            need_update = 1;
            if (key == UI_KEY_UP_LONG) osDelay(60);
            break;

        /* ---------- 减少 ---------- */
        case UI_KEY_DOWN_CLICK:
        case UI_KEY_DOWN_LONG:
            if (focus == 0) tmp_h = (tmp_h == 0) ? 23 : tmp_h - 1;
            else if (focus == 1) tmp_m = (tmp_m == 0) ? 59 : tmp_m - 1;
            else                 tmp_s = (tmp_s == 0) ? 59 : tmp_s - 1;
            need_update = 1;
            if (key == UI_KEY_DOWN_LONG) osDelay(60);
            break;

        /* ---------- 下一个字段 / 保存退出 ---------- */
        case UI_KEY_OK_CLICK:
            focus++;
            if (focus > 2)
            {
                /* ---- 写回 RTC ---- */
                sTime.Hours   = tmp_h;
                sTime.Minutes = tmp_m;
                sTime.Seconds = tmp_s;
                sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
                sTime.StoreOperation = RTC_STOREOPERATION_RESET;
                HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
                HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);   /* 写 time 后也写 date */

                exit_flag = 1;
            }
            need_update = 1;
            break;

        /* ---------- 取消退出 ---------- */
        case UI_KEY_OK_DOUBLE:
            exit_flag = 1;
            break;

        default:
            break;
        }
    }
}

/* ============================================================
 *  [Set Date] 日期设置（HAL RTC 版）
 *  - UP / DOWN       ：+/- 1
 *  - UP / DOWN 长按  ：快速调整
 *  - OK              ：切换 年 → 月 → 日 → 保存退出
 *  - 双击 OK         ：取消退出（不保存）
 * ============================================================ */
void Date_Set_Func(u8g2_t *u8g2)
{
    RTC_DateTypeDef sDate = {0};
    RTC_TimeTypeDef sTime = {0};

    uint8_t exit_flag   = 0;
    uint8_t focus       = 0;          /* 0=年 1=月 2=日 */
    uint8_t need_update = 1;
    char    buf[16];

    /* ---- 读取当前 RTC 日期作为初值 ---- */
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);   /* 先读 time */
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);   /* 再读 date */

    uint16_t tmp_y = 2000 + sDate.Year;   /* HAL 的 Year 是 0~99 */
    uint8_t  tmp_m = sDate.Month;
    uint8_t  tmp_d = sDate.Date;

    /* 闰年判断辅助 */
    #define IS_LEAP(y)   (((y) % 4 == 0 && (y) % 100 != 0) || ((y) % 400 == 0))
    #define DAYS_IN_MONTH(y, m) \
        ((m) == 2 ? (IS_LEAP(y) ? 29 : 28) : \
         ((m) == 4 || (m) == 6 || (m) == 9 || (m) == 11) ? 30 : 31)

    while (!exit_flag)
    {
        /* ---- 每帧同步天数的上限，防止 2 月 31 日这种非法值 ---- */
        uint8_t max_day = DAYS_IN_MONTH(tmp_y, tmp_m);
        if (tmp_d > max_day) tmp_d = max_day;

        if (need_update)
        {
            u8g2_ClearBuffer(u8g2);
            u8g2_SetFont(u8g2, u8g2_font_6x12_tr);
            u8g2_DrawStr(u8g2, 10, 15, "--- Set Date ---");

            u8g2_SetFont(u8g2, u8g2_font_10x20_me);

            /* ---------- 年（4 位） ---------- */
            sprintf(buf, "%04d", tmp_y);
            if (focus == 0) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 4, 25, 42, 22);
                u8g2_SetDrawColor(u8g2, 0);
            }
            u8g2_DrawStr(u8g2, 6, 43, buf);
            u8g2_SetDrawColor(u8g2, 1);
            u8g2_DrawStr(u8g2, 48, 42, "-");

            /* ---------- 月（2 位） ---------- */
            sprintf(buf, "%02d", tmp_m);
            if (focus == 1) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 58, 25, 22, 22);
                u8g2_SetDrawColor(u8g2, 0);
            }
            u8g2_DrawStr(u8g2, 60, 43, buf);
            u8g2_SetDrawColor(u8g2, 1);
            u8g2_DrawStr(u8g2, 82, 42, "-");

            /* ---------- 日（2 位） ---------- */
            sprintf(buf, "%02d", tmp_d);
            if (focus == 2) {
                u8g2_SetDrawColor(u8g2, 1);
                u8g2_DrawBox(u8g2, 92, 25, 22, 22);
                u8g2_SetDrawColor(u8g2, 0);
            }
            u8g2_DrawStr(u8g2, 94, 43, buf);
            u8g2_SetDrawColor(u8g2, 1);

            /* ---------- 底部提示 ---------- */
            u8g2_SetFont(u8g2, u8g2_font_5x8_tr);
            u8g2_DrawStr(u8g2, 2, 62, "OK:Next  2xOK:Cancel");

            u8g2_SendBuffer(u8g2);
            need_update = 0;
        }

        ui_key_t key = ui_port_get_key(50);
        switch (key)
        {
        /* ---------------- 增加 ---------------- */
        case UI_KEY_UP_CLICK:
        case UI_KEY_UP_LONG:
            if (focus == 0) {
                tmp_y++;
                if (tmp_y > 2099) tmp_y = 2000;
            }
            else if (focus == 1) {
                tmp_m = (tmp_m % 12) + 1;    /* 1~12 循环 */
            }
            else {
                tmp_d = (tmp_d % max_day) + 1;
            }
            need_update = 1;
            if (key == UI_KEY_UP_LONG) osDelay(60);
            break;

        /* ---------------- 减少 ---------------- */
        case UI_KEY_DOWN_CLICK:
        case UI_KEY_DOWN_LONG:
            if (focus == 0) {
                if (tmp_y == 2000) tmp_y = 2099;
                else               tmp_y--;
            }
            else if (focus == 1) {
                tmp_m = (tmp_m == 1) ? 12 : tmp_m - 1;
            }
            else {
                tmp_d = (tmp_d == 1) ? max_day : tmp_d - 1;
            }
            need_update = 1;
            if (key == UI_KEY_DOWN_LONG) osDelay(60);
            break;

        /* ---------------- 下一个字段 / 保存退出 ---------------- */
        case UI_KEY_OK_CLICK:
            focus++;
            if (focus > 2)
            {
                /* ---- 写回 RTC ---- */
                sDate.Year  = (uint8_t)(tmp_y - 2000);   /* 2000~2099 → 0~99 */
                sDate.Month = tmp_m;
                sDate.Date  = tmp_d;

                HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);   /* 必须先写 time */
                HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);   /* 再写 date */

                exit_flag = 1;
            }
            need_update = 1;
            break;

        /* ---------------- 取消退出 ---------------- */
        case UI_KEY_OK_DOUBLE:
            exit_flag = 1;
            break;

        default:
            break;
        }
    }

    #undef IS_LEAP
    #undef DAYS_IN_MONTH
}


/*****************************Tetris********************************************************/
/*****************************Tetris********************************************************/
/*
 *  俄罗斯方块
 *  - 棋盘 10×15，每格 4px，占据左侧 40×60
 *  - 右侧显示分数、消除行数、等级、下一块预览
 *  - UP 短按 = 左移一格
 *  - UP 长按 = 左移连发（80ms）
 *  - UP 双击 = 旋转
 *  - DOWN 短按 = 右移一格
 *  - DOWN 长按 = 右移连发（80ms）
 *  - DOWN 双击 = 软降一格
 *  - OK 短按 = 硬降 / 游戏结束后重开
 *  - OK 双击 = 暂停 / 继续
 *  - OK 长按 = 退出
 */

#include <stdlib.h>
#include <string.h>

/* ============================================================
 *  常量
 * ============================================================ */
#define TETRIS_COLS     10
#define TETRIS_ROWS     15
#define TETRIS_CELL     4           /* 每格像素 */

#define TETRIS_X        2           /* 棋盘在屏幕上的左上角 */
#define TETRIS_Y        2

#define TETRIS_FALL_MS  500         /* 0 级自动下落间隔（ms） */
#define TETRIS_HMOVE_MS 80          /* 长按左右连发间隔 */

/* ============================================================
 *  7 种方块 × 4 个旋转状态（4×4 位掩码）
 *  位序：位 0 = 左上角，位 4 = 第二行首列，…，位 15 = 右下角
 * ============================================================ */
static const uint16_t TETROMINO[7][4] = {
    /* I */ { 0x00F0, 0x4444, 0x0F00, 0x2222 },
    /* O */ { 0x0033, 0x0033, 0x0033, 0x0033 },
    /* T */ { 0x0072, 0x0262, 0x0270, 0x0232 },
    /* S */ { 0x0036, 0x0462, 0x0036, 0x0462 },
    /* Z */ { 0x0063, 0x0264, 0x0063, 0x0264 },
    /* J */ { 0x0071, 0x0226, 0x0470, 0x0322 },
    /* L */ { 0x0074, 0x0622, 0x0170, 0x0223 },
};

/* ============================================================
 *  状态
 * ============================================================ */
typedef struct {
    uint16_t board[TETRIS_ROWS];   /* 每行一个 10 位掩码 */
    uint8_t  piece;                 /* 0~6 */
    uint8_t  rot;                   /* 0~3 */
    int8_t   px, py;                /* 方块左上角在棋盘中的坐标 */
    uint32_t score;
    uint16_t lines;
    uint8_t  level;
    uint32_t last_fall;
    uint8_t  next_piece;
} TetrisState;

/* ============================================================
 *  碰撞检测
 *  返回 1 = 合法位置，0 = 碰撞
 * ============================================================ */
static uint8_t tetris_check(const TetrisState *s,
                            uint8_t piece, uint8_t rot,
                            int8_t px, int8_t py)
{
    uint16_t mask = TETROMINO[piece][rot];

    for (uint8_t r = 0; r < 4; r++)
    {
        uint8_t bits = (uint8_t)((mask >> (r * 4)) & 0x0F);
        if (bits == 0) continue;

        int board_y = py + r;
        if (board_y < 0) continue;              /* 棋盘上方允许 */
        if (board_y >= TETRIS_ROWS) return 0;   /* 超出底部 */

        for (uint8_t c = 0; c < 4; c++)
        {
            if (!(bits & (1 << c))) continue;

            int board_x = px + c;
            if (board_x < 0 || board_x >= TETRIS_COLS) return 0;   /* 越界 */
            if (s->board[board_y] & (1 << board_x))    return 0;   /* 撞已有块 */
        }
    }
    return 1;
}

/* ============================================================
 *  把当前方块锁定到棋盘，并处理消行
 * ============================================================ */
static void tetris_lock(TetrisState *s)
{
    uint16_t mask = TETROMINO[s->piece][s->rot];

    /* 写入棋盘 */
    for (uint8_t r = 0; r < 4; r++)
    {
        uint8_t bits = (uint8_t)((mask >> (r * 4)) & 0x0F);
        if (bits == 0) continue;
        int board_y = s->py + r;
        if (board_y < 0 || board_y >= TETRIS_ROWS) continue;

        for (uint8_t c = 0; c < 4; c++)
        {
            if (!(bits & (1 << c))) continue;
            int board_x = s->px + c;
            if (board_x >= 0 && board_x < TETRIS_COLS)
                s->board[board_y] |= (uint16_t)(1 << board_x);
        }
    }

    /* 消行：整行填满即清除，并让上方行下移 */
    uint8_t cleared = 0;
    for (int r = TETRIS_ROWS - 1; r >= 0; )
    {
        if (s->board[r] == 0x3FF)          /* 10 位全 1 */
        {
            for (int rr = r; rr > 0; rr--)
                s->board[rr] = s->board[rr - 1];
            s->board[0] = 0;
            cleared++;
            /* 不清 r，继续检查同一位置（上方行已下移） */
        }
        else
        {
            r--;
        }
    }

    /* 计分 */
    if (cleared > 0)
    {
        static const uint16_t pts[5] = { 0, 100, 300, 500, 800 };
        if (cleared > 4) cleared = 4;
        s->score += (uint32_t)pts[cleared] * (s->level + 1);
        s->lines += cleared;
        s->level  = s->lines / 10;
    }
}

/* ============================================================
 *  生成新方块（使用 next_piece）
 *  返回 1 = 成功，0 = 顶到天花板 → 游戏结束
 * ============================================================ */
static uint8_t tetris_spawn(TetrisState *s)
{
    s->piece = s->next_piece;
    s->next_piece = (uint8_t)(rand() % 7);
    s->rot = 0;
    s->px = (TETRIS_COLS - 4) / 2;   /* 居中对齐 */
    s->py = 0;

    return tetris_check(s, s->piece, s->rot, s->px, s->py);
}

/* ============================================================
 *  绘制（★ 整帧只 SendBuffer 一次，paused 和 game_over 都并入其中）
 * ============================================================ */
static void tetris_draw(u8g2_t *u8g2, const TetrisState *s,
                        uint8_t game_over, uint8_t paused)
{
    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 1);

    /* ---------- 棋盘外框 ---------- */
    u8g2_DrawFrame(u8g2,
                   TETRIS_X - 1, TETRIS_Y - 1,
                   TETRIS_COLS * TETRIS_CELL + 2,
                   TETRIS_ROWS * TETRIS_CELL + 2);

    /* ---------- 已锁定的方块 ---------- */
    for (uint8_t r = 0; r < TETRIS_ROWS; r++)
    {
        for (uint8_t c = 0; c < TETRIS_COLS; c++)
        {
            if (s->board[r] & (1 << c))
            {
                u8g2_DrawBox(u8g2,
                             TETRIS_X + c * TETRIS_CELL,
                             TETRIS_Y + r * TETRIS_CELL,
                             TETRIS_CELL, TETRIS_CELL);
            }
        }
    }

    /* ---------- 当前活动方块 ---------- */
    uint16_t mask = TETROMINO[s->piece][s->rot];
    for (uint8_t r = 0; r < 4; r++)
    {
        uint8_t bits = (uint8_t)((mask >> (r * 4)) & 0x0F);
        if (bits == 0) continue;

        int board_y = s->py + r;
        if (board_y < 0 || board_y >= TETRIS_ROWS) continue;

        for (uint8_t c = 0; c < 4; c++)
        {
            if (!(bits & (1 << c))) continue;
            int board_x = s->px + c;
            if (board_x < 0 || board_x >= TETRIS_COLS) continue;

            u8g2_DrawBox(u8g2,
                         TETRIS_X + board_x * TETRIS_CELL,
                         TETRIS_Y + board_y * TETRIS_CELL,
                         TETRIS_CELL, TETRIS_CELL);
        }
    }

    /* ---------- 右侧信息 ---------- */
    char buf[20];
    u8g2_SetFont(u8g2, u8g2_font_5x8_tr);

    sprintf(buf, "SC %lu", (unsigned long)s->score);
    u8g2_DrawStr(u8g2, 50, 10, buf);

    sprintf(buf, "LN %u", s->lines);
    u8g2_DrawStr(u8g2, 50, 22, buf);

    sprintf(buf, "LV %u", s->level);
    u8g2_DrawStr(u8g2, 50, 34, buf);

    u8g2_DrawStr(u8g2, 50, 46, "NEXT");

    /* ---------- 下一块预览 ---------- */
    uint16_t nmask = TETROMINO[s->next_piece][0];
    uint8_t  nx0 = 100, ny0 = 42;
    for (uint8_t r = 0; r < 4; r++)
    {
        uint8_t bits = (uint8_t)((nmask >> (r * 4)) & 0x0F);
        if (bits == 0) continue;

        for (uint8_t c = 0; c < 4; c++)
        {
            if (!(bits & (1 << c))) continue;
            u8g2_DrawBox(u8g2, nx0 + c * 3, ny0 + r * 3, 3, 3);
        }
    }

    /* ---------- 游戏结束遮罩 ---------- */
    if (game_over)
    {
        u8g2_SetDrawColor(u8g2, 1);
        u8g2_DrawBox(u8g2, 12, 20, 104, 24);
        u8g2_SetDrawColor(u8g2, 0);
        u8g2_SetFont(u8g2, u8g2_font_7x14_tr);
        u8g2_DrawStr(u8g2, 30, 38, "GAME OVER");
        u8g2_SetDrawColor(u8g2, 1);
    }

    /* ---------- 暂停遮罩 ---------- */
    if (paused && !game_over)
    {
        u8g2_SetDrawColor(u8g2, 1);
        u8g2_DrawBox(u8g2, 16, 22, 96, 20);
        u8g2_SetDrawColor(u8g2, 0);
        u8g2_SetFont(u8g2, u8g2_font_7x14_tr);
        u8g2_DrawStr(u8g2, 38, 38, "PAUSED");
        u8g2_SetDrawColor(u8g2, 1);
    }

    /* ---------- ★ 整帧只刷新一次 ---------- */
    u8g2_SendBuffer(u8g2);
}

/* ============================================================
 *  主入口
 * ============================================================ */
void Tetris_Func(u8g2_t *u8g2)
{
    TetrisState state;
    uint8_t  exit_flag = 0;
    uint8_t  game_over = 0;
    uint8_t  paused    = 0;
    uint32_t last_h_move = 0;

    srand(HAL_GetTick());

    memset(&state, 0, sizeof(state));
    state.next_piece = (uint8_t)(rand() % 7);
    if (!tetris_spawn(&state)) game_over = 1;
    state.last_fall = HAL_GetTick();

    while (!exit_flag)
    {
        uint32_t now = HAL_GetTick();

        /* ============================================================
         *  自动下落（暂停或游戏结束时不动）
         * ============================================================ */
        if (!game_over && !paused)
        {
            uint16_t fall_ms = TETRIS_FALL_MS;
            for (uint8_t i = 0; i < state.level; i++)
                if (fall_ms > 100) fall_ms -= 40;

            if (now - state.last_fall >= fall_ms)
            {
                state.last_fall = now;
                if (tetris_check(&state, state.piece, state.rot,
                                 state.px, state.py + 1))
                {
                    state.py++;
                }
                else
                {
                    tetris_lock(&state);
                    if (!tetris_spawn(&state)) game_over = 1;
                }
            }
        }

        /* ============================================================
         *  绘制（★ 一次调用，一次 SendBuffer）
         * ============================================================ */
        tetris_draw(u8g2, &state, game_over, paused);

        /* ============================================================
         *  按键：暂停时轮询放宽到 100ms，减少无谓重绘
         * ============================================================ */
        ui_key_t key = ui_port_get_key(paused ? 100 : 30);
        switch (key)
        {
        /* -------- UP 短按：左移一格 -------- */
        case UI_KEY_UP_CLICK:
            if (!game_over && !paused)
            {
                if (tetris_check(&state, state.piece, state.rot,
                                 state.px - 1, state.py))
                    state.px--;
            }
            break;

        /* -------- UP 长按：左移连发 -------- */
        case UI_KEY_UP_LONG:
            if (!game_over && !paused && now - last_h_move >= TETRIS_HMOVE_MS)
            {
                last_h_move = now;
                if (tetris_check(&state, state.piece, state.rot,
                                 state.px - 1, state.py))
                    state.px--;
            }
            break;

        /* -------- UP 双击：旋转（带踢墙） -------- */
        case UI_KEY_UP_DOUBLE:
            if (!game_over && !paused)
            {
                uint8_t nr = (state.rot + 1) & 3;
                if (tetris_check(&state, state.piece, nr, state.px, state.py))
                {
                    state.rot = nr;
                }
                else if (tetris_check(&state, state.piece, nr, state.px - 1, state.py))
                {
                    state.rot = nr; state.px--;
                }
                else if (tetris_check(&state, state.piece, nr, state.px + 1, state.py))
                {
                    state.rot = nr; state.px++;
                }
            }
            break;

        /* -------- DOWN 短按：右移一格 -------- */
        case UI_KEY_DOWN_CLICK:
            if (!game_over && !paused)
            {
                if (tetris_check(&state, state.piece, state.rot,
                                 state.px + 1, state.py))
                    state.px++;
            }
            break;

        /* -------- DOWN 长按：右移连发 -------- */
        case UI_KEY_DOWN_LONG:
            if (!game_over && !paused && now - last_h_move >= TETRIS_HMOVE_MS)
            {
                last_h_move = now;
                if (tetris_check(&state, state.piece, state.rot,
                                 state.px + 1, state.py))
                    state.px++;
            }
            break;

        /* -------- DOWN 双击：软降一格 -------- */
        case UI_KEY_DOWN_DOUBLE:
            if (!game_over && !paused)
            {
                if (tetris_check(&state, state.piece, state.rot,
                                 state.px, state.py + 1))
                {
                    state.py++;
                    state.last_fall = now;
                }
            }
            break;

        /* -------- OK 短按：硬降 / 游戏结束后重开 -------- */
        case UI_KEY_OK_CLICK:
            if (!game_over && !paused)
            {
                while (tetris_check(&state, state.piece, state.rot,
                                    state.px, state.py + 1))
                    state.py++;

                tetris_lock(&state);
                if (!tetris_spawn(&state)) game_over = 1;
                state.last_fall = now;
            }
            else if (game_over)
            {
                memset(&state, 0, sizeof(state));
                state.next_piece = (uint8_t)(rand() % 7);
                if (!tetris_spawn(&state)) game_over = 1;
                state.last_fall = HAL_GetTick();
                game_over = 0;
                paused    = 0;
            }
            break;

        /* -------- OK 双击：暂停 / 继续 -------- */
        case UI_KEY_OK_DOUBLE:
            if (!game_over)
            {
                paused = !paused;
                if (!paused)
                    state.last_fall = HAL_GetTick();   /* 恢复时重置下落计时 */
            }
            break;

        /* -------- OK 长按：退出 -------- */
        case UI_KEY_OK_LONG:
            exit_flag = 1;
            break;

        default:
            break;
        }
    }
}

/* =========================================================
 * 菜单配置及系统入口
 * ========================================================= */

// 声明，因为要在 main_items 里使用
void Settings_Menu_Entry(u8g2_t *u8g2);

/* ---- 主菜单项优化排序 ---- */
static ui_menu_item_t main_items[] = {
    { "1. System Status", Status_Func },
    { "2. DAC Control",   DAC_Func    },
    { "3. ADC Test",      ADC_Test    },
		{ "4. ADC Wave",      ADC_Wave    },
    { "5. I2C Scan",      I2C_Scan    },
		{ "6. BMP280",				BMP280_Display }, 
    { "7. Settings",      Settings_Menu_Entry }, 
    { "8. Key Guide",     Help_Func   },
    { "9. Display_Func",  Display_Func  },
		{ "10. Tetris",       Tetris_Func  },
		{ "11. About Device", About_Func  }
};

// 子菜单配置
static ui_menu_item_t setting_items[] = {
    { "- Brightness",    Brightness_Func },
    { "- Set Time",      Time_Set_Func   },
		{ "- Set Data",      Date_Set_Func   },
		{ "- Display Speed", Display_Speed_Func   }
		
		
    // 你可以继续加...
};

// 包装函数：调用核心层引擎运行子菜单
void Settings_Menu_Entry(u8g2_t *u8g2) {
    ui_core_run_menu(u8g2, "=== SETTINGS ===", setting_items, sizeof(setting_items) / sizeof(ui_menu_item_t));
}

// UI 主入口（可以在 main 函数的任务里死循环调用）
void App_UI_Init(u8g2_t *u8g2) {
    ui_core_run_menu(u8g2, "=== MAIN MENU ===", main_items, sizeof(main_items) / sizeof(ui_menu_item_t));
}


