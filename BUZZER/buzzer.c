#include "buzzer.h"
#include "delay.h" 


void BUZZER_Init(void){ //蜂鸣器的接口初始化
  GPIO_InitTypeDef GPIO_InitStruct = {0};

   __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = BUZZER_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUZZERPORT, &GPIO_InitStruct);
}

void BUZZER_BEEP1(void){ //蜂鸣器响一声
//	for(uint16_t  i=0;i<200;i++){
//		HAL_GPIO_WritePin(BUZZERPORT,BUZZER_PIN,GPIO_PIN_RESET );//蜂鸣器接口输出0
//		Delay_us(500); //延时		
//		HAL_GPIO_WritePin(BUZZERPORT,BUZZER_PIN,GPIO_PIN_SET );//蜂鸣器接口输出高电平1
//		Delay_us(300); //延时		
//	}
		HAL_GPIO_WritePin(BUZZERPORT,BUZZER_PIN,GPIO_PIN_SET );//蜂鸣器接口输出高电平1
		Delay_us(3000); //延时		
    HAL_GPIO_WritePin(BUZZERPORT,BUZZER_PIN,GPIO_PIN_RESET );//蜂鸣器接口输出高电平1
}

void BUZZER_BEEP(uint8_t times,uint8_t delay){ //蜂鸣器响多声
	for(uint16_t i=0;i<times;i++){
		BUZZER_BEEP1();
		HAL_Delay(2000);
	}
	
//		do
//			{
//				BUZZER_BEEP1();
//				times -- ;
//				HAL_Delay(delay);
//			}
//			while(times>0);
}

uint16_t music1[78]={ //音乐1的数据表（奇数是音调，偶数是长度）
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
440,1500,
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
784,1500,
659,750,
698,375,
784,375,
880,750,
784,375,
698,375,
659,750,
587,750,
659,750,
523,375,
494,375,
440,750,
440,375,
494,375,
523,750,
523,750,
494,750,
392,750,
440,3000
};

void MIDI_PLAY(void){ //MIDI音乐
	uint16_t i,e;
	for(i=0;i<39;i++){
		for(e=0;e<music1[i*2]*music1[i*2+1]/1000;e++){
			HAL_GPIO_WritePin(BUZZERPORT,BUZZER_PIN,GPIO_PIN_RESET ); //蜂鸣器接口输出0
			Delay_us(500000/music1[i*2]); //延时		
			HAL_GPIO_WritePin(BUZZERPORT,BUZZER_PIN,GPIO_PIN_SET ); //蜂鸣器接口输出高电平1
			Delay_us(500000/music1[i*2]); //延时	
		}	
	}
}
/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/


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






