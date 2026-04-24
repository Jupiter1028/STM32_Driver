#include "onewire.h"       
#include "delay.h"         //延时函数
#include "gpio.h"

/******************************************************************************************
* Function name ： Onewire_Init()
* Description : 初始化单总线
* Input Parameter ： 无
* Return ：无
* Others : 需要开启单总线IO对于的时钟
******************************************************************************************/
void Onewire_Init(void){
	__HAL_RCC_GPIOB_CLK_ENABLE();
	Onewire_IN();
}

/******************************************************************************************
* Function name ： Onewire_IN()
* Description : 设置单总线接口为(上拉)输入
* Input Parameter ： 无
* Return ：无
* Others : 需要定义相应的"Onewire_Pin"接口
******************************************************************************************/
void Onewire_IN(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin  = Onewire_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
  GPIO_InitStruct.Pull = GPIO_PULLUP ;            //上拉输入模式
  HAL_GPIO_Init(Onewire_Port, &GPIO_InitStruct);
 
}

/******************************************************************************************
* Function name ： Onewire_OUT()
* Description : 设置单总线接口为输出
* Input Parameter ： bit
*        @bit= 0 : 输出0
*        @bit= 1 : 输出1 
* Return ：无
* Others : 需要定义相应的"Onewire_Pin"接口
******************************************************************************************/
void Onewire_OUT(uint8_t bit){
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 if(bit == 1){
		  HAL_GPIO_WritePin(Onewire_Port, Onewire_Pin, GPIO_PIN_SET);	
		 
		  GPIO_InitStruct.Pin = Onewire_Pin;
		  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP  ;	
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	}else
		{	
		  HAL_GPIO_WritePin(Onewire_Port, Onewire_Pin, GPIO_PIN_RESET);
			
		  GPIO_InitStruct.Pin = Onewire_Pin;
		  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP  ;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  
			
		}
	  HAL_GPIO_Init(Onewire_Port, &GPIO_InitStruct);
}

/******************************************************************************************
* Function name ： OnewireReset()
* Description : 检测单总线设备
* Input Parameter ： 无
* Return ：
*        @ 0 : 无单总线设备
*        @ 1 : 检测到单总线设备
* Others : 
******************************************************************************************/
uint8_t OnewireReset(void)
{
	uint8_t data;
	Onewire_OUT(0);
	Delay_us(600);       // 拉低总线600us
	Onewire_IN();        //设置为上拉输入,等待从机拉低
	Delay_us(60);        //应答信号在等待15~60us后发送
	if(HAL_GPIO_ReadPin(Onewire_Port, Onewire_Pin)==0){ 
		Delay_us(20);	    //从机低脉冲在60~240us
		if(HAL_GPIO_ReadPin(Onewire_Port, Onewire_Pin)==0)
			data = 1;
	}else
		data = 0;
	return data ;
}

/******************************************************************************************
* Function name ： OnewireReadBIT()
* Description : 从单总线设备读取1Bit数据
* Input Parameter ： 无
* Return ：
*        @ 0 : 读取到0
*        @ 1 : 读取到1
* Others : 
******************************************************************************************/
uint8_t OnewireReadBIT(void){
	uint8_t temp;
	Onewire_OUT(0);
	Delay_us(2);
	Onewire_IN();
	Delay_us(12);
	if(HAL_GPIO_ReadPin(Onewire_Port, Onewire_Pin)==1){
		temp = 1;
	}
	if(HAL_GPIO_ReadPin(Onewire_Port, Onewire_Pin)==0){
		temp = 0;
	}
	Delay_us(50);  		 //读完一位延时10us	
	return temp;
}

/******************************************************************************************
* Function name ： OnewireReadByte()
* Description : 从单总线设备读取1Byte数据
* Input Parameter ： 无
* Return ：读取到的1Byte数据
* Others : 先读低位
******************************************************************************************/
uint8_t OnewireReadByte(void){
	uint8_t data =0,i;
	for( i=0;i<8;i++ ){
		data >>= 1;
			if( OnewireReadBIT() ==1 ){
				data |= 0x80 ;
			}else
				data |= 0 ;
	}
	return data;
}

/******************************************************************************************
* Function name ： OnewireReadBIT()
* Description : 往单总线上写入1Bit数据
* Input Parameter ： bit
*        @bit = 0 : 写0
*        @bit = 1 : 写1
* Return ：无
* Others : 先写低位
******************************************************************************************/
void OnewireWriteBIT(uint8_t bit){
	if( bit == 0){
		Onewire_OUT(0);   
		Delay_us(80);        //写0:60~120us
	}
	else
		if( bit == 1)
		{
			Onewire_OUT(0);  
			Delay_us(1);
			Onewire_OUT(1);  
			Delay_us(60);    //写1:拉低1us 释放:60~120us
		}
	Onewire_IN();
	return;
}

/******************************************************************************************
* Function name ： OnewireWriteByte(uint8_t data)
* Description : 往单总线上写入1Byte数据
* Input Parameter ： data
*        @data    : 需要写入的数据(1Byte)
* Return ：无
* Others : 先写低位
******************************************************************************************/
void OnewireWriteByte(uint8_t data){
	uint8_t bit,i;
	for(i = 0;i<8;i++){
		bit = data & 0x01 ;
		if(bit == 1){
			OnewireWriteBIT(1);
		}else
			OnewireWriteBIT(0);
		data >>= 1;
		Delay_us(10);
	}
}






