#include <main.h>   
//#include "delay.h" 
#include "hal_oled.h" 
#include "oled_font.h" 
#include "math.h" 


/**
  * @brief  OLED
  * @param  Data 
  * @retval 
  */
void OLED_WriteData(uint8_t Data)
{
	HAL_I2C_Mem_Write(&oled_iic,OLED_ADDR,OLED_DAT,1,&Data,1,10);
}

/**
  * @brief  OLED
  * @param  Command 
  * @retval 
  */
void OLED_WriteCommand(uint8_t Command)
{
  __disable_irq(); //
	HAL_I2C_Mem_Write(&oled_iic,OLED_ADDR,OLED_COM,1,&Command,1,10);

  __enable_irq();//
}


/**
  * @brief  OLED
  * @param  Y 0~7
  * @param  X 0~127
  * @retval 
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//Y
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//X4
	OLED_WriteCommand(0x00 | (X & 0x0F));			//X4
}

/**
  * @brief  OLED
  * @param  
  * @retval 
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
  * @brief  OLED
  * @param  Line 1~4
  * @param  Column 1~16
  * @param  Char ASCII
  * @retval 
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{   
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//
	}
}

/**
  * @brief  OLED
  * @param  Line 1~4
  * @param  Column 1~16
  * @param  String ASCII
  * @retval 
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
  * @brief  OLED
  * @retval XY
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
  * @brief  OLED
  * @param  Line 1~4
  * @param  Column 1~16
  * @param  Number 0~4294967295
  * @param  Length 1~10
  * @retval 
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
  * @brief  OLED
  * @param  Line 1~4
  * @param  Column 1~16
  * @param  Number -2147483648~2147483647
  * @param  Length 1~10
  * @retval 
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
  * @brief  OLED
  * @param  Line 1~4
  * @param  Column 1~16
  * @param  Number 0~0xFFFFFFFF
  * @param  Length 1~8
  * @retval 
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
  * @brief  OLED
  * @param  Line 1~4
  * @param  Column 1~16
  * @param  Number 0~1111 1111 1111 1111
  * @param  Length 1~16
  * @retval 
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
  * @brief  OLED
  * @param  
  * @retval 
//  */
void OLED_Init(void)
{
	HAL_Delay(200);
	
	OLED_WriteCommand(0xAE);	//
	
	OLED_WriteCommand(0xD5);	///
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//
	
	OLED_WriteCommand(0xA1);	//0xA1 0xA0
	
	OLED_WriteCommand(0xC8);	//0xC8 0xC0

	OLED_WriteCommand(0xDA);	//COM
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//VCOMH
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	///

	OLED_WriteCommand(0xA6);	///

	OLED_WriteCommand(0x8D);	//
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//
		
	OLED_Clear();				//OLED
}


void OLED_ShowPIC(uint8_t *PIC){ //
	for(uint8_t m=0;m<8;m++){//
		OLED_WriteCommand(0xb0+m);
		OLED_WriteCommand(0x10); //4
		OLED_WriteCommand(0x02);	//4
		for(uint8_t i=0;i<128;i++){//128
			OLED_WriteData(PIC[i+m*128]);
		}
	}
}



void OLED_ShowFloat(uint8_t Line, uint8_t Column, float value, uint8_t intlen, uint8_t diclen){ //
	uint8_t isless0 = 0;
	if(value < 0){
			OLED_ShowChar(Line,Column, '-');
			isless0 = 1 ;
	}	
	OLED_ShowNum(Line, Column+isless0,value,intlen);
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
  * @brief  OLED
  * @param  Line 1~4
  * @param  Column 1~4
	* @param  order 
  * @retval 
  */
void OLED_ShowChineseChar(uint8_t Line, uint8_t Column,uint8_t order)
{      	
	OLED_SetCursor((Line-1)*2,(Column-1)*16);		//
	for (uint8_t  i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED_F16x16[order-1][i]);			//
	}
	
	OLED_SetCursor(Line*2-1,(Column-1)*16);	//
	for (uint8_t  i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED_F16x16[order-1][i+16]);		//
	}
}

/**
  * @brief  OLED
  * @param  Line 1~128
  * @param  Column 1~64
  * @retval 
  */
void OLED_ShowPoints(uint8_t Columu,uint16_t value){
	uint8_t y = value; 		//
	uint8_t x = Columu ;		
	uint8_t num = 0x01;
	OLED_SetCursor(y,x);
	OLED_WriteData(num); //
}

/**
  * @brief  OLED
  * @param  Line 0~127
  * @param  Column 0~63
  * @retval 
  */
void OLED_ShowPixel(uint8_t x,uint16_t y,uint8_t value){
	OLED_SetCursor(y,x);
  switch(value){
    case 1:
      OLED_WriteData(0x01); //
      break;
    case 2:
      OLED_WriteData(0x02); //
      break;
    case 3:
      OLED_WriteData(0x04); //
      break;
    case 4:
      OLED_WriteData(0x08); //
      break;
    case 5:
      OLED_WriteData(0x10); //
      break;
    case 6:
      OLED_WriteData(0x20); //
      break;
    case 7:
      OLED_WriteData(0x40); //
      break;
    case 8:
      OLED_WriteData(0x80); //
      break;
  }	
}

/**
  * @brief  OLED
  * @param  Line 0~127
  * @param  Column 0~63
  * @retval 
  */
void OLED_FullPixel(uint8_t x,uint8_t value){
  uint8_t a = value/8 ;//
  for(uint8_t i=0;i<a;i++){
    OLED_SetCursor(i,x);
    OLED_WriteData(0x7F);
  }
  uint8_t b = value%8 ;//
	OLED_SetCursor(x,7-b);
  switch(value){
    case 1:
      OLED_WriteData(0x01); //
      break;
    case 2:
      OLED_WriteData(0x03); //
      break;
    case 3:
      OLED_WriteData(0x07); //
      break;
    case 4:
      OLED_WriteData(0x0F); //
      break;
    case 5:
      OLED_WriteData(0x1F); //
      break;
    case 6:
      OLED_WriteData(0x3F); //
      break;
    case 7:
      OLED_WriteData(0x7F); //
      break;
    case 8:
      OLED_WriteData(0xFF); //
      break;
  }	
}

/*
  * @brief  //OLED 
  * @param  Column 1~128
	* @param  value  	0~64
	*/
void OLED_ShowColumn(uint8_t x,uint8_t value)
{
	uint8_t a = value/8;	//
	uint8_t b = value%8-1;
	uint8_t i;
	
	
	//0X00()
	for(i=0;i<7-a;i++){				//
		OLED_SetCursor(i,x);
		OLED_WriteData(0x00);
	}
	//0XFF()
	for(i=0;i<a;i++){				//
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

////1ack=1ACKack=0nACK   
//uint8_t I2C_Read_Byte(void)
//{
//	unsigned char i,receive=0;
////	SDA_IN();//SDA
//  for(i=0;i<8;i++ )
//	{
//    SCL_OUT(0); 
//    Delay_us(2);
//		SCL_OUT(1);
//    receive<<=1;
//    if(HAL_GPIO_ReadPin(I2C_PORT,SDA_PIN))receive++;   
//		Delay_us(1); 
//   }					 
//   return receive;
//}


/**

*@brief
*@param hi2c:I2C_HandleTypeDefI2C
*@param DevAddress:7
*@param MemAddress
*@param MemAddSize
*@param pData
*@param Size
*@param	
*@retval HAL
  */
//HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)











