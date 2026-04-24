#ifndef __ONEWIRE_H
#define __ONEWIRE_H
#include "main.h"    

#define Onewire_Pin GPIO_PIN_13            //单总线设备的接口
#define Onewire_Port GPIOC

void Onewire_IN(void);
void Onewire_OUT(uint8_t bit);

void Onewire_Init(void);
uint8_t OnewireReset(void);
void OnewireWriteBIT(uint8_t bit);
void OnewireWriteByte(uint8_t data);
	
uint8_t OnewireReadByte(void);
uint8_t OnewireReadBIT(void);

#endif
