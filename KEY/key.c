#include "key.h"
#include "delay.h"

void KEY1_Init(void){ //微动开关的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);       
  GPIO_InitStructure.GPIO_Pin = KEY_LEFT | KEY_DOWN; //选择端口号（0~15或all）                        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //选择IO接口工作方式 //下拉电阻       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	
	GPIO_Init(KEYPORT1,&GPIO_InitStructure);
}

void KEY_Init(void){ //微动开关的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);       
  GPIO_InitStructure.GPIO_Pin = KEY_UP | KEY_RIGHT |  KEY_LEFT | KEY_DOWN; //选择端口号（0~15或all）                        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //选择IO接口工作方式 //下拉电阻       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	
	GPIO_Init(KEYPORT0,&GPIO_InitStructure);	
	KEY1_Init();
}



uint8_t ReadKEYBit(uint16_t KEY){
	uint8_t KEYBit = 0 ;
	if(KEY==KEY_UP)
	{
		KEYBit = GPIO_ReadInputDataBit(KEYPORT0,KEY_UP);
	}
	
	if(KEY==KEY_RIGHT)
	{
		KEYBit = GPIO_ReadInputDataBit(KEYPORT0,KEY_RIGHT);
	}
	
	if(KEY==KEY_DOWN)
	{
		KEYBit = GPIO_ReadInputDataBit(KEYPORT1,KEY_DOWN);
	}
	
	if(KEY==KEY_LEFT)
	{
		KEYBit = GPIO_ReadInputDataBit(KEYPORT1,KEY_LEFT);
	}
	
	return KEYBit;
	
}

//获取按键代码有问题
uint8_t GetKey(){	
	uint8_t key = 0;
	
		if ( ReadKEYBit(KEY_UP)==1){
			delay_ms(20);
			while(ReadKEYBit(KEY_UP)==1)
				delay_ms(20);
			key = 1;	
				}
		if ( ReadKEYBit(KEY_DOWN)==1){
			delay_ms(20);
			while(ReadKEYBit(KEY_DOWN)==1)
				delay_ms(20);
			key = 2;	
				}
		if ( ReadKEYBit(KEY_LEFT)==1){
			delay_ms(20);
			while(ReadKEYBit(KEY_LEFT)==1)
				delay_ms(20);
			key = 3;	
				}
		if ( ReadKEYBit(KEY_RIGHT)==1){
			delay_ms(20);
			while(ReadKEYBit(KEY_RIGHT)==1)
				delay_ms(20);
			key = 4;	
				}
				
		return key;				
}
 

/*
选择IO接口工作方式：
GPIO_Mode_AIN 模拟输入
GPIO_Mode_IN_FLOATING 浮空输入
GPIO_Mode_IPD 下拉输入
GPIO_Mode_IPU 上拉输入
GPIO_Mode_Out_PP 推挽输出
GPIO_Mode_Out_OD 开漏输出
GPIO_Mode_AF_PP 复用推挽输出
GPIO_Mode_AF_OD 复用开漏输出
*/
