//GPIO模拟IIC
#ifndef __OLED_H
#define __OLED_H

#include <main.h>   
#define OLED_addr 0x78   //OLED地址


//PB6     ------> I2C1_SCL
//PB7     ------> I2C1_SDA
#define SCL_PIN GPIO_PIN_6
#define SDA_PIN GPIO_PIN_7
#define I2C_PORT GPIOB

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);

void SDA_OUT(uint8_t bit);
void SCL_OUT(uint8_t bit);

void I2C_Write_Byte(uint8_t data);
uint8_t I2C_Read_Byte(void);

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

void OLED_ShowPoint(uint8_t Columu,uint16_t value);//显示一个点
void OLED_ShowWave(uint8_t* address);//显示波形
void OLED_ShowColumn(uint8_t Columu,uint8_t value);//显示一个列

#endif
