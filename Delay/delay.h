#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx_hal.h"

//void HAL_Delay_us_init(uint8_t SYSCLK);
void Delay_us(int16_t nus);
void Delay_ms(int16_t nms);




#endif
