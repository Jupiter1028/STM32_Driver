#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "main.h"

#define BUZZERPORT	GPIOB	//定义IO接口
#define BUZZER_PIN	GPIO_PIN_9	//定义IO接口


void BUZZER_Init(void);//初始化
void BUZZER_BEEP1(void);//响一声
void BUZZER_BEEP(uint8_t times,uint8_t delay);//响多声
void MIDI_PLAY(void);
		 				    
#endif
