#ifndef __0D96LCD_H
#define __0D96LCD_H

#include "main.h"

/*==================================================================
 * 0.96寸 TFT LCD (ST7735) 驱动 - 头文件 (HAL 版)
 * 分辨率: 80x160
 * 通信:  硬件 SPI1 (SCK=PA5, MOSI=PA7)
 * 控制:  RES=PC4, DC=PC5, CS=PB0, BLK=PB1
 * 说明:  GPIO 与 SPI 需在外部（CubeMX/其它模块）完成初始化
 *==================================================================*/

/*---------------- 显示方向 ----------------
 * 0/1 : 竖屏 (80x160)
 * 2/3 : 横屏 (160x80)
 *------------------------------------------*/
#define USE_HORIZONTAL  2

#if (USE_HORIZONTAL == 0) || (USE_HORIZONTAL == 1)
#define LCD_W   80
#define LCD_H   160
#else
#define LCD_W   160
#define LCD_H   80
#endif

/*---------------- SPI 句柄 ----------------
 * 若你的工程 SPI1 handle 名字不是 hspi1，改这里即可
 *------------------------------------------*/
extern SPI_HandleTypeDef hspi1;
#define LCD_SPI_HANDLE   (&hspi1)

/*---------------- LCD 端口定义 ----------------*/
#define LCD_RES_Clr()   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET)   /* RES */
#define LCD_RES_Set()   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET)

#define LCD_DC_Clr()    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)   /* DC  */
#define LCD_DC_Set()    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define LCD_CS_Clr()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)   /* CS  */
#define LCD_CS_Set()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)

#define LCD_BLK_Clr()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)   /* BLK */
#define LCD_BLK_Set()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

/*---------------- 底层接口 ----------------*/
void LCD_Writ_Bus(uint8_t dat);                                  /* SPI 发送一字节 */
void LCD_WR_DATA8(uint8_t dat);                                  /* 写一个字节     */
void LCD_WR_DATA(uint16_t dat);                                  /* 写两个字节     */
void LCD_WR_REG(uint8_t dat);                                    /* 写一个指令     */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Init(void);                                             /* LCD 初始化    */

/*---------------- 绘图接口 ----------------*/
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

/*---------------- 汉字显示 ----------------*/
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);

/*---------------- 字符 / 字符串 / 数字 ----------------*/
void  LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void  LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
uint32_t mypow(uint8_t m, uint8_t n);
void  LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void  LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);

/*---------------- 图片显示 ----------------*/
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[]);

/*---------------- 常用颜色 ----------------*/
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define BRED           0XF81F
#define GRED           0XFFE0
#define GBLUE          0X07FF
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define BROWN          0XBC40   /* 棕色              */
#define BRRED          0XFC07   /* 棕红色            */
#define GRAY           0X8430   /* 灰色              */
#define DARKBLUE       0X01CF   /* 深蓝色            */
#define LIGHTBLUE      0X7D7C   /* 浅蓝色            */
#define GRAYBLUE       0X5458   /* 灰蓝色            */
#define LIGHTGREEN     0X841F   /* 浅绿色            */
#define LGRAY          0XC618   /* 浅灰色(窗体背景)   */
#define LGRAYBLUE      0XA651   /* 浅灰蓝色(中间层)   */
#define LBBLUE         0X2B12   /* 浅棕蓝色(选择反色) */

#endif /* __0D96LCD_H */