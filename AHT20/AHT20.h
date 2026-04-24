#ifndef _AHT20_h_
#define _AHT20_h_

#include <main.h>   			//IO定义与初始化函数定义在main.c中,必须引用
#include "i2c.h"

#define aht_iic hi2c1 		//定义AHT20使用的IIC

#define AHT20_ADDRESS 0x70

void AHT20_Init(void);
void AHT20_ReadData(float *Temperature, float *Humidity);


#endif // _AHT20_h_

