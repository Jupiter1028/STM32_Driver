//GPIO模拟IIC

//#include "i2c_gpio.h"   
#include <main.h>   
#include "delay.h" 
#include "oled.h" 
#include "oled_font.h" 
#include "math.h" 

//I2C接口
void I2C_Init(void){
  
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = SDA_PIN | SCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(I2C_PORT, SDA_PIN | SCL_PIN, GPIO_PIN_SET);
}


//SDA管脚设置为输出
void SDA_OUT(uint8_t bit){ 
		HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, (GPIO_PinState)bit);
}

//SCL管脚设置为输出
void SCL_OUT(uint8_t bit){ 
		HAL_GPIO_WritePin(I2C_PORT, SCL_PIN,(GPIO_PinState)bit);
}

void I2C_Start(void){
	SDA_OUT(1);
//	Delay_us(3);
	SCL_OUT(1);
//	Delay_us(3);
	SDA_OUT(0);
//	Delay_us(3);
	SCL_OUT(0);  
}

void I2C_Stop(void){
	SDA_OUT(0);
//	Delay_us(3);
	SCL_OUT(1);  
//	Delay_us(3);
	SDA_OUT(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void I2C_Write_Byte(uint8_t data){
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		SDA_OUT(data & (0x80 >> i));
		SCL_OUT(1);
//		Delay_us(3);
		SCL_OUT(0);
	}
	SCL_OUT(1);	//额外的一个时钟，不处理应答信号
//	Delay_us(3);
	SCL_OUT(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	I2C_Start();
	I2C_Write_Byte(0x78);		//从机地址
	I2C_Write_Byte(0x00);		//写命令
	I2C_Write_Byte(Command); 
	I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	I2C_Start();
	I2C_Write_Byte(0x78);		//从机地址
	I2C_Write_Byte(0x40);		//写数据
	I2C_Write_Byte(Data);
	I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置低4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置高4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}


/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
//  */
void OLED_Init(void)
{
//	Delay_ms(2000);
	
		uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//????
	{
		for (j = 0; j < 1000; j++);
	}
	
	I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}


void OLED_ShowPIC(uint8_t *PIC){ //显示全屏图片
	for(uint8_t m=0;m<8;m++){//
		OLED_WriteCommand(0xb0+m);
		OLED_WriteCommand(0x10); //起始列地址的高4位
		OLED_WriteCommand(0x02);	//起始列地址的低4位
		for(uint8_t i=0;i<128;i++){//送入128次图片显示内容
			OLED_WriteData(PIC[i+m*128]);
		}
	}
}



void OLED_ShowFloat(uint8_t Line, uint8_t Column, float value, uint8_t intlen, uint8_t diclen){ //显示浮点数
	uint8_t isless0 = 0;
	if(value < 0){
			OLED_ShowChar(Line,Column, '-');
			isless0 = 1 ;
	}	
	OLED_ShowNum(Line, Column+isless0,32,intlen);
	OLED_ShowChar(Line,Column+isless0 +intlen,'.');
	OLED_ShowNum(Line,Column+isless0 +intlen+1,(((float)value - (uint8_t)value) * pow(10,diclen)),diclen);
}

void OLED_ShowTemperature(uint8_t Line,float value){
	uint8_t intlen = 2,diclen = 4 ;
	OLED_ShowChineseChar(Line,1,13);
	OLED_ShowChineseChar(Line,2,14);	
	OLED_ShowChar(Line,5,':');	
		
	OLED_ShowFloat(Line,6,value,intlen,diclen);
	OLED_ShowChineseChar(Line,7,12);
}

void OLED_Showhumidity(uint8_t Line,uint8_t value){
	OLED_ShowChineseChar(Line,1,15);
	OLED_ShowChineseChar(Line,2,14);	
	OLED_ShowChar(Line,5,':');	
	OLED_ShowNum(Line,6,value,2);	
	OLED_ShowChar(Line,8,'%');
}


/**
  * @brief  OLED显示一个中文汉字
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~4
	* @param  order 显示字库里面第几个汉字
  * @retval 无
  */
void OLED_ShowChineseChar(uint8_t Line, uint8_t Column,uint8_t order)
{      	
	OLED_SetCursor((Line-1)*2,(Column-1)*16);		//设置光标位置在上半部分
	for (uint8_t  i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED_F16x16[order-1][i]);			//显示上半部分内容
	}
	
	OLED_SetCursor(Line*2-1,(Column-1)*16);	//设置光标位置在下半部分
	for (uint8_t  i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED_F16x16[order-1][i+16]);		//显示下半部分内容
	}
}



void OLED_ClearPoint(uint8_t x){
	for(uint8_t i=0;i<8;i++){				//行循环
		OLED_SetCursor(i,x);
		OLED_WriteData(0x00);
	}
}

/**
  * @brief  OLED显示一个点,符合坐标轴特点  左下角为(0,0)
  * @param  Column 列位置，范围：1~128
	* @param  value  列值，	范围：0~64
  * @retval 无
  */
void OLED_ShowPoint(uint8_t x,uint16_t value){
	uint8_t a = value/8 ;				//显示在第几行
	uint8_t b = value%8;
	OLED_ClearPoint(x);
	switch(b){
		case 0:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(128);
			break;
		case 1:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(64);
			break;
		case 2:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(32);
			break;
		case 3:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(16);
			break;
		case 4:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(8);
			break;
		case 5:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(4);
			break;
		case 6:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(2);
			break;
		case 7:
			OLED_SetCursor(7 - a,x);
			OLED_WriteData(1);
			break;
	}
}



//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t I2C_Read_Byte(void)
{
	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
    SCL_OUT(0); 
    Delay_us(2);
		SCL_OUT(1);
    receive<<=1;
    if(HAL_GPIO_ReadPin(I2C_PORT,SDA_PIN))receive++;   
		Delay_us(1); 
   }					 
   return receive;
}


/*
  * @brief  //OLED显示一列点 
  * @param  Column 列位置，范围：1~128
	* @param  value  列值，	范围：0~64
	*/
void OLED_ShowColumn(uint8_t x,uint8_t value)
{
//		OLED_WriteData(0xFF);
//		OLED_WriteData(0xFE);
//		OLED_WriteData(0xFC);
//		OLED_WriteData(0xF8);
//		OLED_WriteData(0xF0);
//		OLED_WriteData(0xC0);
//		OLED_WriteData(0x80);
	
	uint8_t a = value/8;	//显示多少列满点
	uint8_t b = value%8-1;
	uint8_t i;
	
	
	//0X00填充(熄灭的行填充)
	for(i=0;i<7-a;i++){				//行循环
		OLED_SetCursor(i,x);
		OLED_WriteData(0x00);
	}
	//0XFF填充(点亮的行填充)
	for(i=0;i<a;i++){				//行循环
		OLED_SetCursor(7-i,x);
		OLED_WriteData(0xFF);
	}
	
	switch(b){
		case 1:
			OLED_SetCursor(7-i,x);
			OLED_WriteData(0x80);
			break;
		case 2:
			OLED_SetCursor(7-i,x);
			OLED_WriteData(0xC0);
			break;
		case 3:
			OLED_SetCursor(7-i,x);
			OLED_WriteData(0xF0);
			break;
		case 4:
			OLED_SetCursor(7-i,x);
			OLED_WriteData(0xF8);
			break;
		case 5:
			OLED_SetCursor(7-i,x);
			OLED_WriteData(0xFC);
			break;
		case 6:
			OLED_SetCursor(7-i,x);
			OLED_WriteData(0xFE);
			break;
		case 7:
			OLED_SetCursor(7-i,x);
			OLED_WriteData(0xFF);
			break;
	}
}


