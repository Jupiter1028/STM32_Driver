#ifndef __WS2812B_H__
#define __WS2812B_H__

#include "tim.h"

/*这里是上文计算所得CCR的宏定义*/
#define CODE_1       (56)       //1码定时器计数次数
#define CODE_0       (28)       //0码定时器计数次数
 
/*建立一个定义单个LED三原色值大小的结构体*/
typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}RGB_Color_TypeDef;
 
#define Pixel_NUM 8  //LED数量宏定义，这里我使用一个LED，（单词pixel为像素的意思）



void RGB_SetColor(uint8_t LedId,RGB_Color_TypeDef Color);//给一个LED装载24个颜色数据码（0码和1码）
void Reset_Load(void); //该函数用于将数组最后24个数据变为0，代表RESET_code
void RGB_SendArray(void);          //发送最终数组
void RGB_RED(uint16_t Pixel_Len);  //显示红灯
void RGB_GREEN(uint16_t Pixel_Len);//显示绿灯
void RGB_BLUE(uint16_t Pixel_Len); //显示蓝灯
void RGB_WHITE(uint16_t Pixel_Len);//显示白灯
 
void led_loop(void);

 
/*
功能：设定单个RGB LED的颜色，把结构体中RGB的24BIT转换为0码和1码
参数：LedId为LED序号，Color：定义的颜色结构体
*/
void RGB_SetColor(uint8_t LedId,RGB_Color_TypeDef Color);


/*
功能：最后一行装在7*24个0，输出24个周期占空比为0的PWM波，作为最后reset延时，这里总时长为24*7*1.2=201.6us > 200us(要求大于200us)
*/
void Reset_Load(void);

 
/*
功能：发送数组
参数：(&htim1)定时器1，(TIM_CHANNEL_2)通道2，((uint32_t *)Pixel_Buf)待发送数组，
			(Pixel_NUM+1)*24)发送个数，数组行列相乘
*/
void RGB_SendArray(void);
 
/*
功能：显示红色
参数：Pixel_Len为显示LED个数
*/
void RGB_RED(uint16_t Pixel_Len);
 
/*
功能：显示绿色
参数：Pixel_Len为显示LED个数
*/
void RGB_GREEN(uint16_t Pixel_Len);
 
/*
功能：显示蓝色
参数：Pixel_Len为显示LED个数
*/
void RGB_BLUE(uint16_t Pixel_Len);
 
/*
功能：显示白色
参数：Pixel_Len为显示LED个数
*/
void RGB_WHITE(uint16_t Pixel_Len);
 
//也可以继续添加其他颜色，和颜色变化函数等

/*******************************************************************************/
/*									添加部分									   */

//显示指定颜色
static void rgb_show(uint32_t Pixel_Len, RGB_Color_TypeDef rgb);

//颜色循环转换
static RGB_Color_TypeDef Wheel(uint8_t WheelPos);

//彩虹呼吸灯
extern void rainbow(uint8_t wait);

//彩虹灯旋转
extern void rainbowCycle(uint8_t wait);


static uint8_t rainbow_change_flag = 1;
void led_loop(void);


#endif
