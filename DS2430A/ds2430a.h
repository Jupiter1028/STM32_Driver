#ifndef __DS2430A_H
#define __DS2430A_H

#include "main.h"     
#include "delay.h"
#include "onewire.h"  

void OnewireReadROM(uint8_t ROM_Address[8]);
void OnewireMatchROM(uint8_t ROM_Address[8]);
void OnewireSkipROM(void); 
uint8_t ReadStatusRegister(void);
void ReadScratchpad(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t main_data[8]);
void WriteScratchpad(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t new_data[8]);
void ReadMemoryAllData(uint8_t ROM_data[8]);
void ReadMemory(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t ROM_data[8]);
void ReadApplicaitionRegister(uint8_t APPregAddr,uint8_t APPReg_data[8]);
void WriteApplicaitionRegister(uint8_t APP_data[8]);
void CopyScratchpad(void);
void WriteMemory(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t new_data[32]);

//搜索设备  暂时不用
//void OnewireSearchROM(void);
#endif

