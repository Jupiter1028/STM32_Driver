#include "LED.h"
#include "main.h"

void LED_ON(uint16_t LED){
	HAL_GPIO_WritePin(GPIOA,LED,GPIO_PIN_RESET);
}

void LED_OFF(uint16_t LED){
	HAL_GPIO_WritePin(GPIOA,LED,GPIO_PIN_SET);
}

void LED_TOGGLE(uint16_t LED){
	HAL_GPIO_TogglePin(GPIOA,LED);
}

//OLED根据输入的数字亮灯
void LED_Ratio(uint8_t value){
	switch(value){
		case 0:
			LED_OFF(LED1|LED2|LED3|LED4|LED5|LED6|LED7);
			break;
		case 1:
			LED_ON(LED1);
			LED_OFF(LED2|LED3|LED4|LED5|LED6|LED7);
			break;
		case 2:
			LED_ON(LED1|LED2);
			LED_OFF(LED3|LED4|LED5|LED6|LED7);
			break;
		case 3:
			LED_ON(LED1|LED2|LED3);
			LED_OFF(LED4|LED5|LED6|LED7);
			break;
		case 4:
			LED_ON(LED1|LED2|LED3|LED4);
			LED_OFF(LED5|LED6|LED7);
			break;
		case 5:
			LED_ON(LED1|LED2|LED3|LED4|LED5);
			LED_OFF(LED6|LED7);
			break;
		case 6:
			LED_ON(LED1|LED2|LED3|LED4|LED5|LED6);
			LED_OFF(LED7);
			break;
		case 7:
			LED_ON(LED1|LED2|LED3|LED4|LED5|LED6|LED7);
			break;
		
	}
}

//跑马灯 3颗灯从左往右1次
void LED_Run1(uint16_t delay){
	HAL_Delay(delay);
	LED_ON(LED1);
	LED_OFF(LED2|LED3|LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED1|LED2);
	LED_OFF(LED3|LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED1|LED2|LED3);
	LED_OFF(LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED2|LED3|LED4);
	LED_OFF(LED1|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED3|LED4|LED5);
	LED_OFF(LED1|LED2|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED5|LED6|LED7);
	LED_OFF(LED1|LED2|LED3|LED4);
	HAL_Delay(delay);
	LED_ON(LED6|LED7);
	LED_OFF(LED1|LED2|LED3|LED4|LED5);
	HAL_Delay(delay);
	LED_ON(LED7);
	LED_OFF(LED1|LED2|LED3|LED4|LED5|LED6);	
}

//跑马灯 3颗灯左右来回
void LED_Run2(uint16_t delay){
	HAL_Delay(delay);
	LED_ON(LED1);
	LED_OFF(LED2|LED3|LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED1|LED2);
	LED_OFF(LED3|LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED1|LED2|LED3);
	LED_OFF(LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED2|LED3|LED4);
	LED_OFF(LED1|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED3|LED4|LED5);
	LED_OFF(LED1|LED2|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED5|LED6|LED7);
	LED_OFF(LED1|LED2|LED3|LED4);
	HAL_Delay(delay);
	LED_ON(LED6|LED7);
	LED_OFF(LED1|LED2|LED3|LED4|LED5);
	HAL_Delay(delay);
	LED_ON(LED7);
	LED_OFF(LED1|LED2|LED3|LED4|LED5|LED6);
	
	HAL_Delay(delay);
	LED_ON(LED7);
	LED_OFF(LED1|LED2|LED3|LED4|LED5|LED6);
	HAL_Delay(delay);
	LED_ON(LED6|LED7);
	LED_OFF(LED1|LED2|LED3|LED4|LED5);
	HAL_Delay(delay);
	LED_ON(LED5|LED6|LED7);
	LED_OFF(LED1|LED2|LED3|LED4);
	HAL_Delay(delay);
	LED_ON(LED4|LED5|LED6);
	LED_OFF(LED1|LED2|LED3|LED7);
	HAL_Delay(delay);
	LED_ON(LED3|LED4|LED5);
	LED_OFF(LED1|LED2|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED2|LED3|LED4);
	LED_OFF(LED1|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED1|LED2|LED3);
	LED_OFF(LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED1|LED2);
	LED_OFF(LED3|LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_ON(LED1);
	LED_OFF(LED2|LED3|LED4|LED5|LED6|LED7);
	HAL_Delay(delay);
	LED_OFF(LED1|LED2|LED3|LED4|LED5|LED6|LED7);
}


//LED以二进制亮灯
void LED_Bin(uint8_t value){
	uint8_t x = value%129;
	LED_OFF(LED1|LED2|LED3|LED4|LED5|LED6|LED7);
	uint8_t a,b,c,d,e,f,g,h;
	a = ( x & 0x80 );
	b = ( x & 0x40 );
	c = ( x & 0x20 );
	d = ( x & 0x10 );	
	e = ( x & 0x08 );
	f = ( x & 0x04 );
	g = ( x & 0x02 );
	h = ( x & 0x01 );
	
	if(a==128){LED_ON(LED1|LED2|LED3|LED4|LED5|LED6|LED7);}
	if(b==64){LED_ON(LED1);}
	if(c==32){LED_ON(LED2);}
	if(d==16){LED_ON(LED3);}
	if(e==8){LED_ON(LED4);}
	if(f==4){LED_ON(LED5);}
	if(g==2){LED_ON(LED6);}
	if(h==1){LED_ON(LED7);}
}





