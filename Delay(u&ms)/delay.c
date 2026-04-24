#include "delay.h"                  // Device header
      
/*
Systick功能实现us延时，参数SYSCLK为系统时钟
*/
//uint32_t fac_us;
// 
//void HAL_Delay_us_init(uint8_t SYSCLK)
//{
//     fac_us=SYSCLK; 
//}
 
//void Delay_us(uint32_t nus)
//{
//    uint32_t ticks;
//    uint32_t told,tnow,tcnt=0;
//    uint32_t reload=SysTick->LOAD;
//    ticks=nus*fac_us; 
//    told=SysTick->VAL; 
//    while(1)
//    {
//        tnow=SysTick->VAL;
//        if(tnow!=told)
//        {
//            if(tnow<told)tcnt+=told-tnow;
//            else tcnt+=reload-tnow+told;
//            told=tnow;
//            if(tcnt>=ticks)break; 
//        }
//    };
//}

//void Delay_us(uint16_t nus) 
//{
//  int32_t temp; 
////  SysTick->LOAD = nus*9; //72MHz
//  SysTick->LOAD = nus*8;   //64M/8   每8次为1us
//  SysTick->VAL=0X00;
//  SysTick->CTRL=0X01;
//  do 
//  { 
//    temp=SysTick->CTRL;
//  }
//  while((temp&0x01)&&(!(temp&(1<<16))));
//     
//  SysTick->CTRL=0x00; 
//  SysTick->VAL =0X00; 
//}

void Delay_ms(uint16_t nms) {
	for(uint8_t i = 0; i < 10;i++)
	Delay_us(99*nms); 
}


 /*
 for循环实现延时us
 */
void Delay_us(uint16_t nus)
{
   uint32_t Delay = 42 ;
   do
   {
    __NOP();
    }
     while (Delay --);
}
 









