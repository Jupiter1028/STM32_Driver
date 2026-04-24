#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED1 GPIO_PIN_1
#define LED2 GPIO_PIN_2
#define LED3 GPIO_PIN_3
#define LED4 GPIO_PIN_4
#define LED5 GPIO_PIN_5
#define LED6 GPIO_PIN_6
#define LED7 GPIO_PIN_7


void LED_ON(uint16_t LED);
void LED_OFF(uint16_t LED);
void LED_Ratio(uint8_t value);
void LED_Run1(uint16_t delay);
void LED_Run2(uint16_t delay);
void LED_Run3(uint16_t delay);
void LED_Bin(uint8_t value);

#endif
