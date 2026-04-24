#ifndef __KEY_H
#define __KEY_H	 

#include "gpio.h"			
#include "main.h"		
//////////////////////////////////////////////////////////////////////////////////


typedef enum
{
    KEY_MODE = 1,
		KEY_POW,							
		KEY_POW_LONG,
    KEY_ONE_S,
    KEY_START,
    KEY_MEM,
    KEY_MENU,
	
}KeyValueTypedef;

typedef struct
{
    uint8_t  nState;	
    uint8_t  nLastValue;	
    uint8_t  nValue;	
    uint32_t  nTime;	
	volatile	uint8_t  nKeyMutex ;	
	uint8_t nkey_state;	
	
}KeyTypedef;

//------------------- global var ---------------------
extern KeyTypedef Key;
extern volatile uint8_t	 nkey_flag;


#define KeyRead  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) //¶ÁÈ¡°´¼ü0

uint8_t KEY_Scan(uint8_t mode);  	
uint8_t Check_KEY(void);    
void TaskKeyScan(void);
//void KeyProc(void);
void KeyProcOff(void);
void KeyProcOn(void);
void KeyInit(void);
#endif
