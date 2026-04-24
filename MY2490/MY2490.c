#include "MY2490.h"

uint8_t PLAY[] = {0X7E,0X03,0X11,0X12,0XEF};
uint8_t PAUSE[] = {0X7E,0X03,0X12,0X11,0XEF};
uint8_t VOLUMEPLUS[] = {0X7E,0X03,0X15,0X16,0XEF};
uint8_t VOLUMEDOWN[] = {0X7E,0X03,0X16,0X15,0XEF};
uint8_t PLAYPAUSE[] = {0X7E,0X03,0X1C,0X1F,0XEF};
uint8_t PLAYNEXT[] = {0X7E,0X03,0X13,0X10,0XEF};
uint8_t PLAYPREVIOUS[] = {0X7E,0X03,0X14,0X17,0XEF};
uint8_t SETVOLUME[] = {0X7E,0X03,0X31,0X00,0X2C,0XEF};
uint8_t PLAYNUM[] = {0X7E,0X05,0X41,0X00,0X00,0X45,0XEF};

void AUDIO_ON(void){
	HAL_GPIO_WritePin(AUDIO_EN_GPIO_Port,AUDIO_EN_Pin,GPIO_PIN_SET);
}

void AUDIO_OFF(void){
	HAL_GPIO_WritePin(AUDIO_EN_GPIO_Port,AUDIO_EN_Pin,GPIO_PIN_RESET);
}

void MY2490_PLAY(void){
	HAL_UART_Transmit(&MY2490_UART,PLAY,sizeof(PLAY),1);
}

void MY2490_PAUSE(void){
	HAL_UART_Transmit(&MY2490_UART,PAUSE,sizeof(PAUSE),1);
}

void MY2490_VOLUMEPLUS(void){
	HAL_UART_Transmit(&MY2490_UART,VOLUMEPLUS,sizeof(VOLUMEPLUS),1);
}

void MY2490_VOLUMEDOWN(void){
	HAL_UART_Transmit(&MY2490_UART,VOLUMEDOWN,sizeof(VOLUMEDOWN),1);
}

void MY2490_PLAYPAUSE(void){
	HAL_UART_Transmit(&MY2490_UART,PLAYPAUSE,sizeof(PLAYPAUSE),1);
}

void MY2490_SETVOLUME(uint8_t value){
	SETVOLUME[3] = value;
	SETVOLUME[4] = SETVOLUME[1] ^ SETVOLUME[2] ^ SETVOLUME[3];
	HAL_UART_Transmit(&MY2490_UART,SETVOLUME,sizeof(SETVOLUME),1);
}

void MY2490_PLAYNUM(uint16_t num){
	PLAYNUM[3] = num >> 8;
	PLAYNUM[4] = (uint8_t)num;
	PLAYNUM[5] = PLAYNUM[1] ^ PLAYNUM[2] ^ PLAYNUM[3] ^ PLAYNUM[4];
	HAL_UART_Transmit(&MY2490_UART,PLAYNUM,sizeof(PLAYNUM),1);
}

void MY2490_PLAYPREVIOUS(void){
	HAL_UART_Transmit(&MY2490_UART,PLAYPREVIOUS,sizeof(PLAYPREVIOUS),1);
}

void MY2490_PLAYNEXT(void){
	AUDIO_ON();
	HAL_UART_Transmit(&MY2490_UART,PLAYNEXT,sizeof(PLAYNEXT),1);
}

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：uint8_t hour   ：小时
                uint8_t minute ：分钟
      返回值：  无
******************************************************************************/
void MY2490_TIME(uint8_t hour,uint8_t minute){
  MY2490_PLAYNUM(214);    //北京时间
  if( hour < 5){
    MY2490_PLAYNUM(102);    //凌晨
  }else
    if( hour > 5 && hour < 9){
      MY2490_PLAYNUM(103);    //早上
    }else
      if( hour > 8 && hour < 11){
        MY2490_PLAYNUM(104);    //上午
      }else
        if( hour == 12 ){
          MY2490_PLAYNUM(105);    //中午
        }else
          if( hour > 12 && hour < 19 ){
            MY2490_PLAYNUM(106);    //下午
          }else
            MY2490_PLAYNUM(107);    //晚上
  
	MY2490_PLAYNUM(hour);       //小时
  MY2490_PLAYNUM(100);        //点
  MY2490_PLAYNUM(minute);     //分钟
  MY2490_PLAYNUM(101);        //分
}





