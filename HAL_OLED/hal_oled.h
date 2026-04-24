//GPIO模拟IIC
#ifndef __HAL_OLED_H
#define __HAL_OLED_H

#include <main.h>   			//IO定义与初始化函数定义在main.c中,必须引用
#include "i2c.h"


#define OLED_ADDR 0x78   //OLED地址(禁止修改)
#define OLED_COM 	0x00   //OLED指令(禁止修改)
#define OLED_DAT 	0x40   //OLED数据(禁止修改)

#define oled_iic hi2c1 		//定义OLED使用的IIC

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float value, uint8_t intlen, uint8_t diclen);
void OLED_ShowTemperature(uint8_t Line,float value);
void OLED_Showhumidity(uint8_t Line,uint8_t value);

void OLED_SetCursor(uint8_t Y, uint8_t X);//设置光标位置
void OLED_WriteData(uint8_t Data);
void OLED_ShowPIC(uint8_t *PIC); //显示全屏图片
void OLED_ShowChineseChar(uint8_t Line, uint8_t Column, uint8_t order); //显示1个汉字
void OLED_ShowIcon(uint8_t Line, uint8_t Column, uint8_t order); //显示1个icon

void OLED_ShowPoints(uint8_t Columu,uint16_t value);//显示一个点
void OLED_ShowPixel(uint8_t x,uint16_t y,uint8_t value);//显示一个像素
void OLED_FullPixel(uint8_t x,uint8_t value);//填充像素
void OLED_ShowWave(uint8_t* address);//显示波形
void OLED_ShowColumn(uint8_t x,uint8_t value);


#endif
