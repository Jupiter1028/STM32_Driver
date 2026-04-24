#include "AHT20.h"

/******************************************************************************
函数说明：温湿度传感器AHT20初始化函数
入口数据：无
返回值： 无
******************************************************************************/
void AHT20_Init(void)
{
   uint8_t readbuffer;//用于接收状态信息
   HAL_Delay(100);     //延时100ms，根据手册第一步的上电后要等待100ms
   HAL_I2C_Master_Receive(&aht_iic,AHT20_ADDRESS<<1 , &readbuffer, 1,10);
   if((readbuffer & 0x08) == 0x00)//根据手册，首先要看状态字的校准使能位Bit[3]是否为 1 
   {
     uint8_t sendbuffer[3]={0xBE,0x08,0x00};
     HAL_I2C_Master_Transmit(&hi2c1,AHT20_ADDRESS<<1,sendbuffer,3,10);
   }
}

/******************************************************************************
函数说明：读取温湿度的函数
入口数据：float *Temperature ：温度数据存放的地址
          float *Humidity    ：湿度数据存放的地址
返回值： 无
******************************************************************************/
void AHT20_ReadData(float *Temperature, float *Humidity)
{
   uint8_t sendbuffer[3] = {0xAC,0x33,0x00};
   uint8_t readbuffer[6];
 
   HAL_I2C_Master_Transmit(&aht_iic,AHT20_ADDRESS<<1,sendbuffer,3,10);
 
   HAL_Delay(80);               //等待80ms,等待测量完成
  
   HAL_I2C_Master_Receive(&aht_iic,AHT20_ADDRESS<<1,readbuffer,7,10);
   if((readbuffer[0] & 0x80) == 0x00)   //如果状态字Bit[7]为0，说明测量完成
   {
     uint32_t data = 0;
 
     data = ((uint32_t)readbuffer[3]>>4)+((uint32_t)readbuffer[2]<<4)+((uint32_t)readbuffer[1]<<12);
     *Humidity = data * 100.0f/ (1<<20);
    
     data = (((uint32_t)readbuffer[3] & 0X0F)<<16)+((uint32_t)readbuffer[4]<<8)+((uint32_t)readbuffer[5]);
     *Temperature = (data * 200.0f)/(1<<20) - 50;
   }
}




