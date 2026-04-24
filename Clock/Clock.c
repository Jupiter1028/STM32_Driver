#include "Clock.h"

AlarmClock alarm_clock_list[10]; //定义一个时钟列表最多10个时钟

void AlarmClockInit(void){
  for(uint8_t i=0;i<10;i++){
    alarm_clock_list[i].music = 0xFFFF;
  }
}

/******************************************************************************
函数说明：添加个性化闹钟
入口数据：AlarmClock
          uint8_t year   年;0代表1900年   
          uint8_t month  月份
          uint8_t day    日
          uint8_t week   周的第几天
          uint8_t hour   小时
          uint8_t minute 分钟
          uint16_t music  提醒铃声
        *不需要设置的参数输入0xFF,
          例 year = 0xFF,month=0xFF,day=0xFF,week = 0x1F（0001 1111）
             hour = 8,minute = 0,代表每周一到周五早上8点的闹钟                                      
返回值： 0:设置成功   -1:设置失败
******************************************************************************/
uint8_t AddAlarmClock(AlarmClock alarmclock){
  for(uint8_t i=0;i<10;i++){
    if(alarm_clock_list[i].music == 0xFFFF){
      alarm_clock_list[i].year   = alarmclock.year;
      alarm_clock_list[i].month  = alarmclock.month;
      alarm_clock_list[i].day    = alarmclock.day;
      alarm_clock_list[i].week   = alarmclock.week;
      alarm_clock_list[i].hour   = alarmclock.hour;
      alarm_clock_list[i].minute = alarmclock.minute;
      return 0;
    }else
      return -1;
  }
}























