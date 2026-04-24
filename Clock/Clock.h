#ifndef _CLOCK_h_
#define _CLOCK_h_

#include <main.h>   			//IO定义与初始化函数定义在main.c中,必须引用
#include "rtc.h"

typedef struct _alarm_clock{
  uint8_t year;      
  uint8_t month;     
  uint8_t day;        
  uint8_t week;
  uint8_t hour;
  uint8_t minute;
  uint16_t music ;
}AlarmClock;



#endif //_CLOCK_h_

