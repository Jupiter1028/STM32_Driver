#ifndef __MY2490_H__
#define __MY2490_H__

#include "main.h"   
#include "usart.h"

#define MY2490_UART huart3

void MY2490_PLAY(void);
void MY2490_PAUSE(void);
void MY2490_VOLUMEPLUS(void);
void MY2490_VOLUMEDOWN(void);
void MY2490_PLAYPAUSE(void);
void MY2490_SETVOLUME(uint8_t value);
void MY2490_PLAYNUM(uint16_t num);
void MY2490_PLAYPREVIOUS(void);
void MY2490_PLAYNEXT(void);
void MY2490_TIME(uint8_t hour,uint8_t minute);

void AUDIO_ON(void);
void AUDIO_OFF(void);
#endif /* __MY2490_H__*/
