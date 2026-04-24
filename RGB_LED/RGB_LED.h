#ifndef __RGB_LED_H
#define __RGB_LED_H

#include <main.h>  
#include "delay.h"   

#define RGB_PORT GPIOA
#define RGB_PIN GPIO_PIN_1


struct Color{
	uint8_t G;
	uint8_t R;
	uint8_t B;
};




void RGB_Init(void);
void RGB_RESET(void);
void RGB_SET(uint8_t bit);
void RGB_Set_Color(struct Color C);


#endif

