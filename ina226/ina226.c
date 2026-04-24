#include "ina226.h"
#include "i2c.h"

/*
**************************************************
* 说明：读取BUS电压，并转换为浮点数据
**************************************************
*/
float INA226_GetBusV(void) 
{
	uint16_t regData;
	float fVoltage;
  regData = INA226_GetBusVReg();
	fVoltage = regData * 0.00125f;/*电压的LSB = 1.25mV*/
	return fVoltage;
}
/*
**************************************************
* 说明：读取电流，并转换为浮点数据
**************************************************
*/
float INA226_GetCurrent() 
{
	uint16_t regData;
	float fCurrent;
	regData = INA226_GetCurrentReg();
	if(regData >= 0x8000)	regData = 0;
	fCurrent = regData * 0.0002f;/*电流的LSB = 0.2mA，由用户配置*/
	return fCurrent;
}
/*
**************************************************
* 说明：读取功率，并转换为浮点数据
**************************************************
*/
float INA226_GetPower() 
{
	uint16_t regData;
	float fPower;
	regData = INA226_GetPowerReg();
	fPower = regData * 0.005f;/*功率的LSB = 电流的LSB*25*/
	return fPower;
}

/*
**************************************************
* 说明：设置配置寄存器（00h）
**************************************************
*/
/*
	Configuration Register(00h)  						            	 		 0x0527 	  	0x45FF
	15   	1：Reset	 1:复位所有寄存器														0			     	 0			
	14:12	3: No Use 																						000		     	 100		
	11:9 	3：Averaging Mode  确定平均值样本数               		010   	     010  
	8:6		3: Bus Voltage Conversion Time  设置VBUS转换时间      100   	     111     
	5:3	 	3：Shunt Voltage Conversion Time  设置分路转换时间    100   	     111  
	2:0	 	3：Operating Mode            设置采样模式    	        111   	     111  
*/
uint8_t INA226_SetConfig(uint16_t ConfigWord) 
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_CONFIG;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, INA226_ADDRESS, SentTable, 3, INA226_I2C_TIMEOUT);
}

/*
**************************************************
* 说明：读取配置寄存器（00h）
**************************************************
*/
uint16_t INA226_GetConfig() 
{
    uint8_t SentTable[1] = {INA226_CONFIG};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

/*
**************************************************
* 说明：读取配置寄存器（01h）
* 
**************************************************
*/
uint16_t INA226_GetShuntV() 
{
    uint8_t SentTable[1] = {INA226_SHUNTV};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

/*
**************************************************
* 说明：读取总线电压寄存器（02h）
* 			用于存储最近的母线电压读数VBUS。
**************************************************
*/
uint16_t INA226_GetBusVReg() 
{
    uint8_t SentTable[1] = {INA226_BUSV};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

/*
**************************************************
* 说明：设置校准寄存器（05h）
* 			这个寄存器为设备提供了存在的电流互感器电阻的值，该电流互感器电阻用于产生测量的差分电压。
*				选用0.01Ω电阻。根据电压降寄存器（01h）的满量程为81.92mV可算得最大测量电流为81.92 / 0.01 = 8192mA。
*				我们计算Current_LSB，根据公式3，此时的Maximum Expected Current为8192
*				Current_LSB = 8192 / 32768 = 0.25mA，再根据公式1向校准寄存器写入的值（CAL）为
*				(0.00512 / (0.25 * 0.01))* 1000 = 2048 = 0x0800
————————————————
版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。                       
原文链接：https://blog.csdn.net/qq_52280273/article/details/136370199
**************************************************
*/
/*
	Calibration Register(05h)  						            	 		 0x0527 	  		0x45FF
	15   	1：Reset	 1:复位所有寄存器														0			     	 0			
	14:12	3: No Use 																						000		     	 100		
	11:9 	3：Averaging Mode  确定平均值样本数               		010   	     010  
	8:6		3: Bus Voltage Conversion Time  设置VBUS转换时间      100   	     111     
	5:3	 	3：Shunt Voltage Conversion Time  设置分路转换时间    100   	     111  
	2:0	 	3：Operating Mode            设置采样模式    	        111   	     111  
*/
uint8_t INA226_SetCalibrationReg(uint16_t ConfigWord) 
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_CALIB;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, INA226_ADDRESS, SentTable, 3, INA226_I2C_TIMEOUT);
}

uint16_t INA226_GetCalibrationReg() 
{
    uint8_t SentTable[1] = {INA226_CALIB};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

uint16_t INA226_GetPowerReg() 
{
    uint8_t SentTable[1] = {INA226_POWER};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

uint16_t INA226_GetCurrentReg() 
{
    uint8_t SentTable[1] = {INA226_CURRENT};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

uint16_t INA226_GetManufID() 
{
    uint8_t SentTable[1] = {INA226_MANUF_ID};
    uint8_t ReceivedTable[2];
	
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

uint16_t INA226_GetDieID() 
{
    uint8_t SentTable[1] = {INA226_DIE_ID};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

uint8_t INA226_SetMaskEnable(uint16_t ConfigWord) 
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_MASK;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, INA226_ADDRESS, SentTable, 3, INA226_I2C_TIMEOUT);
}

uint16_t INA226_GetMaskEnable() 
{
    uint8_t SentTable[1] = {INA226_MASK};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}

uint8_t INA226_SetAlertLimit(uint16_t ConfigWord) 
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_ALERTL;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, INA226_ADDRESS, SentTable, 3, INA226_I2C_TIMEOUT);
}

uint16_t INA226_GetAlertLimit() 
{
    uint8_t SentTable[1] = {INA226_ALERTL};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT,INA226_ADDRESS, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT,INA226_ADDRESS, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK) return 0xFF;
    else return ((uint16_t)ReceivedTable[0]<<8 | ReceivedTable[1]);
}


