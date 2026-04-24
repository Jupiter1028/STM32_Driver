#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//#define KEY1 PAin(0)// PA0
//#define KEY2 PAin(1)// PA1

#define KEYPORT0	GPIOA	//定义IO接口组
#define KEYPORT1	GPIOB	//定义IO接口组
#define KEY_UP	  GPIO_Pin_11	//定义IO接口   GPIO-A
#define KEY_RIGHT	GPIO_Pin_12	//定义IO接口   GPIO-A
#define KEY_LEFT	GPIO_Pin_8	//定义IO接口   GPIO-B
#define KEY_DOWN	GPIO_Pin_9	//定义IO接口   GPIO-B


void KEY_Init(void);//初始化
uint8_t GetKey(void);

		 				    
#endif
