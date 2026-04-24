/*!***************************************************
 * @file: BMI088.h
 * @brief: 用于配置BMX160
 * @author:   
 * @date: 2023/09/11
 * @note:	
 ****************************************************/
#include "bmx160.h"
#include "MahonyAHRS.h"
bmx160 BMX160;

#define BMX160_Enable      {nrf_gpio_pin_clear(SPI_BMX160_CSB);}//CSB1拉低
#define BMX160_Disable     {nrf_gpio_pin_set(SPI_BMX160_CSB);}

#define BMX160_Delay(times)  nrf_delay_ms(times);

/*****************位操作*****************/
/**
* @brief  提取16位二进制数据中一位，返回该位是0或1
* @param	DATA:原始数据
* @param	NUM：数据位置
* @retval 数据值
*/
int Extract_text(uint16_t *DATA, int NUM)
{
    int RE_NUM = 3;
    if (NUM < 0 || NUM > 15)
    {
        Uart_printf(0,"提取发生错误");
        return RE_NUM;
    }

    if (*DATA & (1 << NUM))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
* @brief  提取十六位二进制中的一段，转换为十进制
* @param	DATA:原始数据
* @param	NUM_Start：数据位置开始
* @param	NUM_Stop：数据位置停止
* @retval 数据值
*/
int Binary_To_Decimal(uint16_t *DATA, int NUM_Start, int NUM_Stop)
{
    //函数开启条件：开始和停止位置需要大于0小于15，并且停止位置大于开始位置
    if ((NUM_Start < 0 || NUM_Start > 15) || (NUM_Stop < 0 && NUM_Stop > 15) || (NUM_Start > NUM_Stop))
    {
        Uart_printf(0,"转换发送错误\r\n");
        return (-1);
    }
    //读取二进制位
    int NUM_B[16] = {0};
    for (int i = 0; i < ((NUM_Stop - NUM_Start) + 1); i++)
    {
        NUM_B[i] = Extract_text(DATA, NUM_Start + i);
    }
    //转换为十进制
    int NUM_D = 0;
    for (int i = 0; i < ((NUM_Stop - NUM_Start) + 1); i++)
    {
        NUM_D = NUM_D + (NUM_B[i] * pow(2, i));
    }
    return NUM_D;
}

/**
  * @brief 位操作
  * @param Reg:被修改的值
  * @param Bit_Start:开始修改的位置
  * @param Bit_Stop:结束修改的位置
  * @param Value:需要修改成的值(二进制表示)
  * @retval 返回修改完成后的变量
  */
uint16_t Bit_Modification(uint16_t Reg, int Bit_Start, int Bit_Stop, uint16_t Value)
{
    uint16_t X = 0xFFFF;
    for (int i = Bit_Start; i < Bit_Stop + 1; i++)
    {
        X &= ~(1 << i);
    }
    Reg &= X;
    Reg |= Value << Bit_Start;
    return Reg;
}



/**
  * @brief 根据校准策略自动校准设备
  * @param BMX160：目标设备
  * @retval 校准是否成功
  */
void BMX160_Auto_Calibration(void)
{
    //需要提前配置校准参数
    //配置FOC_CONF寄存器
    uint8_t DATA_FOC_CONF = {0};
    //陀螺仪快速偏移补偿
    DATA_FOC_CONF = Bit_Modification(DATA_FOC_CONF, 6, 6, BMX160_FOC_GYR);
    //加速度计X轴补偿目标值
    DATA_FOC_CONF = Bit_Modification(DATA_FOC_CONF, 4, 5, FOC_ACC_X);

    //加速度计Y轴补偿目标值
    DATA_FOC_CONF = Bit_Modification(DATA_FOC_CONF, 2, 3, FOC_ACC_Y);
    //加速度计Z轴补偿目标值
		DATA_FOC_CONF = Bit_Modification(DATA_FOC_CONF, 0, 1, FOC_ACC_Z);
   
    //补偿校准
    uint8_t DATA_OFFSET = {0};

    DATA_OFFSET = Bit_Modification(DATA_OFFSET, 6, 6, BMX160_off_ACC);

    DATA_OFFSET = Bit_Modification(DATA_OFFSET, 7, 7, BMX160_off_GYR);

    BMX160_Write_Reg(R_OFFSET_6, DATA_OFFSET);
		
    //发送校准命令
    BMX160_Write_Reg(R_CMD, BMX160_CMD_StartFoc);

    BMX160_Delay(100);		
}

/**
  * @brief 写数据到BMX160
  * @param Addr：BMX160寄存器地址
  * @param Val： 要写入的值
  * @retval 无
  */
void BMX160_Write_Reg(uint8_t Addr, uint8_t Val)
{
	BMX160_Enable;
	uint8_t data[2] = { Addr&0x7F,Val };
	SpiWriteReadData(data,2,NULL,0);
	BMX160_Disable;
}

/**
  * @brief 从BMX160寄存器读一个字节的数据
  * @param Addr：BMX160寄存器地址
  * @retval 读取到的数据
  */
uint8_t BMX160_Read_Reg(uint8_t Addr)
{
	static uint8_t val[2];
	BMX160_Enable;
	uint8_t txdata = Addr|0x80;       //  ADD[7]:R/W  1/0    写数据置0,读数据置1
	SpiWriteReadData(&txdata,1,val,2);
	BMX160_Disable;
  return val[1];
}

/**
	* @brief BMX160发送1个字节读取多个字节数据
  * @param Addr：读取数据的寄存器起始地址
  * @param Txlen：地址长度（字节）
  * @param Rxdata：读取到的数据保持地址
  * @param Rxlen：读取的数据长度（字节）
  * @retval 无
  */
void BMX160_Read_Data(uint8_t Addr,uint8_t Txlen,uint8_t * Rxdata,uint8_t Rxlen){
	BMX160_Enable;
	uint8_t txdata = Addr|0x80;
	SpiWriteReadData(&txdata,Txlen,NULL,0);
	SpiWriteReadData(&txdata,0,Rxdata,Rxlen);
	BMX160_Disable;
}

/**
  * @brief 配置磁力计采样策略(参考datasheet 2.4.3.1.3)
  * @param BMX160：目标设备
  * @param MAG_XY_Mode：XY轴采样
  * @param MAG_Z_Mode：Z轴采样
  * @param MAG_ODR：输出速率
  * @retval 配置结果
  */
void BMX160_MAG_Sampling_Mode(uint8_t mag_xy_mode, uint8_t mag_z_mode, uint8_t mag_range)
{
    //CMD发送0x19命令
    BMX160_Write_Reg(R_CMD, BMX160_CMD_PMU_MAG_Normal);   //设置磁力计为普通模式
		BMX160_Delay(1);

    //写入IF[0]，0x80
		BMX160_Write_Reg(R_MAG_IF_0, 0x80);			//参考文档添加
	
		//写入IF[3]，0x01
    BMX160_Write_Reg(R_MAG_IF_3, 0x01);

    //间接写入0x4B,MAG进入睡眠
    BMX160_Write_Reg(R_MAG_IF_2, 0x4B);

    //按照配置模式，写入预设
    if (mag_xy_mode == BMX160_MAG_Mode_LowPower)
    {
        //间接写入PEPXY
        BMX160_Write_Reg(R_MAG_IF_3, 0x01);
        BMX160_Write_Reg(R_MAG_IF_2, 0x51);
    }
    else if (mag_xy_mode == BMX160_MAG_Mode_Normal)
    {
        //间接写入PEPXY
        BMX160_Write_Reg(R_MAG_IF_3, 0x04);
				BMX160_Write_Reg(R_MAG_IF_2, 0x51);
    }
    else if (mag_xy_mode == BMX160_MAG_Mode_Strengthen)
    {
        //间接写入PEPXY
        BMX160_Write_Reg( R_MAG_IF_3, 0x07);
        BMX160_Write_Reg(R_MAG_IF_2, 0x51);
    }
    else if (mag_xy_mode == BMX160_MAG_Mode_HighPrecision)
    {
        //间接写入PEPXY
        BMX160_Write_Reg( R_MAG_IF_3, 0x17);
        BMX160_Write_Reg( R_MAG_IF_2, 0x51);
    }

    if (mag_z_mode == BMX160_MAG_Mode_LowPower)
    {
        //间接写入PEPZ
        BMX160_Write_Reg( R_MAG_IF_3, 0x02);
        BMX160_Write_Reg( R_MAG_IF_2, 0x52);
    }
    else if (mag_z_mode == BMX160_MAG_Mode_Normal)
    {
        //间接写入PEPZ
        BMX160_Write_Reg(R_MAG_IF_3, 0x0E);
        BMX160_Write_Reg(R_MAG_IF_2, 0x52);
    }
    else if (mag_z_mode == BMX160_MAG_Mode_Strengthen)
    {
        //间接写入PEPZ
        BMX160_Write_Reg(R_MAG_IF_3, 0x1A);
        BMX160_Write_Reg(R_MAG_IF_2, 0x52);
    }
    else if (mag_z_mode == BMX160_MAG_Mode_HighPrecision)
    {
        //间接写入PEPZ
        BMX160_Write_Reg(R_MAG_IF_3, 0x52);
        BMX160_Write_Reg(R_MAG_IF_2, 0x52);
    }
		BMX160_Write_Reg(R_MAG_IF_3, 0x02);  //参考文档添加
		
		BMX160_Write_Reg(R_MAG_IF_2, 0x4C);

    BMX160_Write_Reg(R_MAG_IF_1, 0x42);

    BMX160_Write_Reg(R_MAG_CONF,mag_range) ;  //0x44   0x05

		BMX160_Write_Reg(R_MAG_IF_0, 0x00);

    if (mag_xy_mode == BMX160_MAG_Mode_LowPower && mag_z_mode == BMX160_MAG_Mode_LowPower)
    {
        BMX160_Write_Reg(R_CMD, BMX160_CMD_PMU_MAG_LowPower);
    }
		
		BMX160_Write_Reg(R_CMD, BMX160_CMD_PMU_MAG_LowPower);
}

/**
  * @brief 获取传感器温度
  * @param BMX160：目标设备
  * @param DATA：温度值
  * @retval 获取是否成功
  */
uint8_t BMX160_Get_Temperature(float *DATA)
{
    uint8_t DATA_IN[2];
		BMX160_Read_Data(R_TEMPERATURE_0,1,DATA_IN,2);
    //Tem的第15位指示正负，其余指示数值
    uint16_t Tem = (DATA_IN[1] << 8) + (DATA_IN[0]);
    //将第0位到第14位转换为十进制，和分辨率相乘，加上0x0000时的23度
    *DATA = (Binary_To_Decimal(&Tem, 0, 14) * 0.001953125) + 23;
    //判断正负
    if (Extract_text(&Tem, 15) == 1)
    {
        *DATA = *DATA - 64.001953125f;
    }
    return BMX160_OK;
}


uint8_t BMX160_Init(void)
{
	BMX160_Write_Reg(R_CMD,BMX160_CMD_Softreset);					//BMX160复位   7e b6
	BMX160_Delay(100);
		
	uint8_t chip_id = BMX160_Read_Reg(R_CHIP_ID);         //t:80  r:D8
	
	while( chip_id != BMX160_CHIP_ID)   //读取芯片ID
	{
		chip_id = BMX160_Read_Reg(R_CHIP_ID);
	}
	//****************************************************加速度计参数配置****************************************************
	BMX160_Write_Reg(R_CMD, BMX160_CMD_PMU_ACC_Normal);		//加速度计设置为普通模式 	//t:7E 11  
	BMX160_Delay(10);
		
	while(BMX160_Read_Reg(R_ACC_CONF) != (BMX160_ACC_ODR | 0x20))	//ACC 输出速率设置/采样模式设置(普通)        //t:C0  r:0B
	{
			BMX160_Write_Reg(R_ACC_CONF,  (BMX160_ACC_ODR | 0x20) );    	//t:40|20 0B  				
			BMX160_Delay(5);
	}
	
	while(BMX160_Read_Reg(R_ACC_RANGE) != BMX160_ACC_RANG)	//ACC Rang +- 2g;			//t:c1 03	
	{
			BMX160_Write_Reg(R_ACC_RANGE, BMX160_ACC_RANG);    //t:41 03
			BMX160_Delay(5);
	}
	
	//*********************************************陀螺仪参数配置****************************************************
	BMX160_Write_Reg(R_CMD, BMX160_CMD_PMU_GYR_Normal);		//陀螺仪设置为普通模式
	BMX160_Delay(80);
	
	while(BMX160_Read_Reg(R_GYR_CONF) != (BMX160_GYR_ODR | 0x20 ))
	{
			BMX160_Write_Reg(R_GYR_CONF,  (BMX160_GYR_ODR | 0x20 ));     //GYRO Output data rate
			BMX160_Delay(5);
	}
	while(BMX160_Read_Reg(R_GYR_RANGE) != BMX160_GYR_RANGE)
	{
			BMX160_Write_Reg(R_GYR_RANGE, BMX160_GYR_RANGE);     //GYRO Range
			BMX160_Delay(5);
	}
	
	while(BMX160_Read_Reg(R_INT_OUT_CTRL) != BMX160_INT_MODE){	//中断管脚电气属性配置  
			BMX160_Write_Reg(R_INT_OUT_CTRL, BMX160_INT_MODE);    
			BMX160_Delay(5);
	}

	while(BMX160_Read_Reg(R_INT_LATCH) != 0x00)   //中断管脚电气属性配置  
	{
			BMX160_Write_Reg(R_INT_LATCH, 0x00);    
			BMX160_Delay(5);
	}
	
	while(BMX160_Read_Reg(R_INT_MAP_1) != 0x88){
		BMX160_Write_Reg(R_INT_MAP_1, 0x88);              //INT1、INT2都设置为数据就绪中断  
		BMX160_Delay(5); 
	}
	
	while(BMX160_Read_Reg(R_INT_EN_1 ) != 0x10)   	//使能数据就绪中断
	{
			BMX160_Write_Reg(R_INT_EN_1, 0x10);    
			BMX160_Delay(5);
	}
			
	//地磁仪参数配置
	BMX160_MAG_Sampling_Mode(MAG_XY_Mode,MAG_Z_Mode,BMX160_MAG_RANGE);
	
	BMX160_Auto_Calibration();      //校准
	
	return BMX160_OK;
}

/**
  * @brief 直接获取加速度计的三轴参数  
  * @param BMX160：目标设备
  * @param DATA_X：X轴参数
  * @param DATA_Y：Y轴参数
  * @param DATA_Z：Z轴参数
  * @retval 初始化结果（G值未乘常数9.8）
  */
uint8_t BMX160_Get_ACC(void)
{
		BMX160_Read_Data(R_DATA_14,1,BMX160.ACC.buff,6);

    if (BMX160_ACC_RANG == BMX160_ACC_RANG_2G)
    {
        BMX160.acc.ACC_X = BMX160.ACC.data[0] / 16384.0;           //16384LSB/G     
        BMX160.acc.ACC_Y = BMX160.ACC.data[1] / 16384.0;
        BMX160.acc.ACC_Z = BMX160.ACC.data[2] / 16384.0;

        if (BMX160.acc.ACC_X > 2)
        {
            BMX160.acc.ACC_X = BMX160.acc.ACC_X - 4;
        }
        if (BMX160.acc.ACC_Y > 2)
        {
            BMX160.acc.ACC_Y = BMX160.acc.ACC_Y - 4;
        }
        if (BMX160.acc.ACC_Z > 2)
        {
            BMX160.acc.ACC_Z = BMX160.acc.ACC_Z - 4;
        }
    }
    else if (BMX160_ACC_RANG == BMX160_ACC_RANG_4G)
    {
        BMX160.acc.ACC_X = BMX160.ACC.data[0] / 8192.0; 
        BMX160.acc.ACC_Y = BMX160.ACC.data[1] / 8192.0;
        BMX160.acc.ACC_Z = BMX160.ACC.data[2] / 8192.0;
        if (BMX160.acc.ACC_X > 4)
        {
            BMX160.acc.ACC_X = BMX160.acc.ACC_X - 8;
        }
        if (BMX160.acc.ACC_Y > 4)
        {
            BMX160.acc.ACC_Y = BMX160.acc.ACC_Y - 8;
        }
        if (BMX160.acc.ACC_Z > 4)
        {
            BMX160.acc.ACC_Z = BMX160.acc.ACC_Z - 8;
        }
    }
    else if (BMX160_ACC_RANG == BMX160_ACC_RANG_8G)
    {
        BMX160.acc.ACC_X = BMX160.ACC.data[0] / 4096.0;
        BMX160.acc.ACC_Y = BMX160.ACC.data[1] / 4096.0;
        BMX160.acc.ACC_Z = BMX160.ACC.data[2] / 4096.0;
        if (BMX160.acc.ACC_X > 8)
        {
            BMX160.acc.ACC_X = BMX160.acc.ACC_X - 16;
        }
        if (BMX160.acc.ACC_Y > 8)
        {
            BMX160.acc.ACC_Y = BMX160.acc.ACC_Y - 16;
        }
        if (BMX160.acc.ACC_Z > 8)
        {
            BMX160.acc.ACC_Z = BMX160.acc.ACC_Z - 16;
        }
    }
    else if (BMX160_ACC_RANG == BMX160_ACC_RANG_16G)
    {
        BMX160.acc.ACC_X = BMX160.ACC.data[0] / 2048.0;
        BMX160.acc.ACC_Y = BMX160.ACC.data[1] / 2048.0;
        BMX160.acc.ACC_Z = BMX160.ACC.data[2] / 2048.0;
        if (BMX160.acc.ACC_X > 16)
        {
            BMX160.acc.ACC_X = BMX160.acc.ACC_X - 32;
        }
        if (BMX160.acc.ACC_Y > 16)
        {
            BMX160.acc.ACC_Y = BMX160.acc.ACC_Y - 32;
        }
        if (BMX160.acc.ACC_Z > 16)
        {
            BMX160.acc.ACC_Z = BMX160.acc.ACC_Z - 32;
        }
    }		
    else
    {
        return BMX160_Error;
    }
		
//		BMX160.acc.ACC_X *= 9.8;
//		BMX160.acc.ACC_Y *= 9.8;
//		BMX160.acc.ACC_Z *= 9.8;
		
    return BMX160_OK;
}

/**
  * @brief 直接获取磁力计的三轴参数
  * @param BMX160：目标设备
  * @param DATA_X：X轴参数
  * @param DATA_Y：Y轴参数
  * @param DATA_Z：Z轴参数
  * @retval 初始化结果(°/S)
  */
void BMX160_Get_MAG(void)
{
		BMX160_Read_Data(R_DATA_0,1,BMX160.MAG.buff,6);

		BMX160.mag.MAG_X = BMX160.MAG.data[0] / 16.0;
		BMX160.mag.MAG_Y = BMX160.MAG.data[1] / 16.0;
		BMX160.mag.MAG_Z = BMX160.MAG.data[2] / 16.0;

    if (BMX160.mag.MAG_X > 2048)
    {
        BMX160.mag.MAG_X = BMX160.mag.MAG_X - 4096;
    }
    if (BMX160.mag.MAG_Y > 2048)
    {
        BMX160.mag.MAG_Y = BMX160.mag.MAG_Y - 4096;
    }
    if (BMX160.mag.MAG_Z > 2048)
    {
        BMX160.mag.MAG_Z = BMX160.mag.MAG_Z - 4096;
    }
}

/**
  * @brief 直接获取陀螺仪的三轴参数
  * @param BMX160：目标设备
  * @param DATA_X：X轴参数
  * @param DATA_Y：Y轴参数
  * @param DATA_Z：Z轴参数
  * @retval 初始化结果
  */
void BMX160_Get_GYR(void)
{
		BMX160_Read_Data(R_DATA_8,1,BMX160.GYRO.buff,6);
    if (BMX160_GYR_RANGE == BMX160_GYR_RANGE_2000)
    {
        BMX160.gyro.GYR_X = (BMX160.GYRO.data[0] / 16.3875);
        BMX160.gyro.GYR_Y = (BMX160.GYRO.data[1] / 16.3875);
        BMX160.gyro.GYR_Z = (BMX160.GYRO.data[2] / 16.3875);   //
        if (BMX160.gyro.GYR_X > 2000)
        {
            BMX160.gyro.GYR_X = BMX160.gyro.GYR_X - 4000;
        }
        if (BMX160.gyro.GYR_Y > 2000)
        {
            BMX160.gyro.GYR_Y = BMX160.gyro.GYR_Y - 4000;
        }
        if (BMX160.gyro.GYR_Z > 2000)
        {
            BMX160.gyro.GYR_Z = BMX160.gyro.GYR_Z - 4000;
        }
    }
    else if (BMX160_GYR_RANGE == BMX160_GYR_RANGE_1000)
    {
        BMX160.gyro.GYR_X = (BMX160.GYRO.data[0] / 32.7675);
        BMX160.gyro.GYR_Y = (BMX160.GYRO.data[1] / 32.7675);
        BMX160.gyro.GYR_Z = (BMX160.GYRO.data[2] / 32.7675);
        if (BMX160.gyro.GYR_X > 1000)
        {
            BMX160.gyro.GYR_X = BMX160.gyro.GYR_X - 2000;
        }
        if (BMX160.gyro.GYR_Y > 1000)
        {
            BMX160.gyro.GYR_Y = BMX160.gyro.GYR_Y - 2000;
        }
        if (BMX160.gyro.GYR_Z > 1000)
        {
            BMX160.gyro.GYR_Z = BMX160.gyro.GYR_Z - 2000;
        }
    }
    else if (BMX160_GYR_RANGE == BMX160_GYR_RANGE_500)
    {
        BMX160.gyro.GYR_X = (BMX160.GYRO.data[0] / 65.535);
        BMX160.gyro.GYR_Y = (BMX160.GYRO.data[1] / 65.535);
        BMX160.gyro.GYR_Z = (BMX160.GYRO.data[2] / 65.535);
        if (BMX160.gyro.GYR_X > 500)
        {
            BMX160.gyro.GYR_X = BMX160.gyro.GYR_X - 1000;
        }
        if (BMX160.gyro.GYR_Y > 500)
        {
            BMX160.gyro.GYR_Y = BMX160.gyro.GYR_Y - 1000;
        }
        if (BMX160.gyro.GYR_Z > 500)
        {
            BMX160.gyro.GYR_Z = BMX160.gyro.GYR_Z - 1000;
        }
    }
    else if (BMX160_GYR_RANGE == BMX160_GYR_RANGE_250)
    {
        BMX160.gyro.GYR_X = (BMX160.GYRO.data[0] / 131.07);
        BMX160.gyro.GYR_Y = (BMX160.GYRO.data[1] / 131.07);
        BMX160.gyro.GYR_Z = (BMX160.GYRO.data[2] / 131.07);
        if (BMX160.gyro.GYR_X > 250)
        {
            BMX160.gyro.GYR_X = BMX160.gyro.GYR_X - 500;
        }
        if (BMX160.gyro.GYR_Y > 250)
        {
            BMX160.gyro.GYR_Y = BMX160.gyro.GYR_Y - 500;
        }
        if (BMX160.gyro.GYR_Z > 250)
        {
            BMX160.gyro.GYR_Z = BMX160.gyro.GYR_Z - 500;
        }
    }
    else if (BMX160_GYR_RANGE == BMX160_GYR_RANGE_125)
    {
        BMX160.gyro.GYR_X = (BMX160.GYRO.data[0] / 142.14);
        BMX160.gyro.GYR_Y = (BMX160.GYRO.data[1] / 142.14);
        BMX160.gyro.GYR_Z = (BMX160.GYRO.data[2] / 142.14);

        if (BMX160.gyro.GYR_X > 125)
        {
            BMX160.gyro.GYR_X = BMX160.gyro.GYR_X - 250;
        }
        if (BMX160.gyro.GYR_Y > 125)
        {
            BMX160.gyro.GYR_Y = BMX160.gyro.GYR_Y - 250;
        }
        if (BMX160.gyro.GYR_Z > 125)
        {
            BMX160.gyro.GYR_Z = BMX160.gyro.GYR_Z - 250;
        }
    }
}

/**
  * @brief 直接获取BMX160的时间
  * @param 
  * @retval 
  */
void BMX160_Get_Time(void){
	BMX160.TIME.time[0] = BMX160_Read_Reg(R_SENSORTIME_0);
	BMX160.TIME.time[1] = BMX160_Read_Reg(R_SENSORTIME_1);
	BMX160.TIME.time[2] = BMX160_Read_Reg(R_SENSORTIME_2);
}

/**
  * @brief 直接读取步数
  * @param 
  * @retval 
  */
void BMX160_Get_Steps(void){
	BMX160.STEPS.buff[0] = BMX160_Read_Reg(R_STEP_CNT_0);
	BMX160.STEPS.buff[1] = BMX160_Read_Reg(R_STEP_CNT_1);
//	BMX160_Read_Data(R_STEP_CNT_0,1,BMX160.STEPS.buff,2); //读取步数数据
	Uart_printf(0,"steps:%d\n",BMX160.STEPS.data);	
}

/**
  * @brief 直接读取步数
  * @param 
  * @retval 
  */
void BMX160_Steps_Clear(void){
	BMX160_Write_Reg(R_CMD,0xB2); //步数清零
}

/**
  * @brief 记步模式设置
  * @param 
  * @retval 
  */
void BMX160_Set_Steps_Mode(void){
	if( BMX160_STEP_MODE == BMX160_STEP_MODE_NORMAL){
		BMX160_Write_Reg(R_STEP_CONF_0,0x15 );
		BMX160_Write_Reg(R_STEP_CONF_1,0x03| 0x08);
	}else
		if( BMX160_STEP_MODE == BMX160_STEP_MODE_SENSITIVE){
			BMX160_Write_Reg(R_STEP_CONF_0,0x2D );
			BMX160_Write_Reg(R_STEP_CONF_1,0x00 | 0x08);
		}else
			if( BMX160_STEP_MODE == BMX160_STEP_MODE_ROBUST){
				BMX160_Write_Reg(R_STEP_CONF_0,0x1D );
				BMX160_Write_Reg(R_STEP_CONF_1,0x07| 0x08);
			}
}

// 将四元数转换为欧拉角的函数
void quaternionToEuler(float q0,float q1,float q2,float q3,float* pitch,float* roll,float* yaw) {

    // 计算俯仰角（pitch）
    *pitch = asin(2.0f * (q0 * q2 - q1 * q3));

    // 计算滚动角（roll）
    *roll = atan2(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2));

    // 计算偏航角（yaw）
    *yaw = atan2(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3));
	
		
		*pitch *=  57.29577952383886f;
		*roll  *=  57.29577952383886f;
		*yaw 	 *=  57.29577952383886f;
}


void Export_Quaternion_Data(void)
{
	Uart_printf(0,"Quaternion:%.6f, %.6f, %.6f,%.6f \n",q0, q1, q2 , q3);
}

#include "bmp280.h"
void Export_EulerAngles(void){
	quaternionToEuler(q0,q1,q2,q3,&BMX160.eularangles.Pitch,&BMX160.eularangles.Roll,&BMX160.eularangles.Yaw);
	Uart_printf(0,"Euler:%.6f,%.6f,%.6f,%.3f\n",BMX160.eularangles.Roll,BMX160.eularangles.Pitch,BMX160.eularangles.Yaw,bmp280_data.altitude_float);
}

void Export_Data(uint8_t Out_Euler,uint8_t Out_altitude){
	if( Out_altitude == 1 && Out_Euler == 1){
		quaternionToEuler(q0,q1,q2,q3,&BMX160.eularangles.Pitch,&BMX160.eularangles.Roll,&BMX160.eularangles.Yaw);
		Uart_printf(0,"Euler:%.6f,%.6f,%.6f,%d\n",BMX160.eularangles.Roll,BMX160.eularangles.Pitch,BMX160.eularangles.Yaw,
//		BMX160.Temperature,bmp280_data.altitude_float);
		uncomp_data.uncomp_press);
	}
	if( Out_Euler == 1 && Out_altitude==0){
		quaternionToEuler(q0,q1,q2,q3,&BMX160.eularangles.Pitch,&BMX160.eularangles.Roll,&BMX160.eularangles.Yaw);
		Uart_printf(0,"EulerAngles:%.6f,%.6f,%.6f\n",BMX160.eularangles.Roll,BMX160.eularangles.Pitch,BMX160.eularangles.Yaw,BMX160.Temperature);
	}
	if( Out_altitude == 1 && Out_Euler == 0){
		Uart_printf(0,"altitude:%f\n",bmp280_data.altitude_float);
	}
}

void IMU_Read(void)
{	
//	BMX160.gyro_f.GYR_X  = BMX160.gyro.GYR_X * 0.00106526443603169529841533860381f	; 	//陀螺仪AD值 转换成弧度/秒  
//	BMX160.gyro_f.GYR_Y  = BMX160.gyro.GYR_Y * 0.00106526443603169529841533860381f	;  	
//	BMX160.gyro_f.GYR_Z  = BMX160.gyro.GYR_Z * 0.00106526443603169529841533860381f	;  	
	
	BMX160.gyro_f.GYR_X  = BMX160.gyro.GYR_X * MPU_GYRO_TO_RDS	; 	//陀螺仪AD值 转换成弧度/秒  
	BMX160.gyro_f.GYR_Y  = BMX160.gyro.GYR_Y * MPU_GYRO_TO_RDS	;  	
	BMX160.gyro_f.GYR_Z  = BMX160.gyro.GYR_Z * MPU_GYRO_TO_RDS	;  	
		
	BMX160.acc_f.ACC_X = BMX160.acc.ACC_X * GG;			//加速度AD值 转换成 米/平方秒 
	BMX160.acc_f.ACC_Y = BMX160.acc.ACC_Y * GG;
	BMX160.acc_f.ACC_Z = BMX160.acc.ACC_Z * GG;
	
	BMX160.mag_f.MAG_X = BMX160.mag.MAG_X;
	BMX160.mag_f.MAG_Y = BMX160.mag.MAG_Y;
	BMX160.mag_f.MAG_Z = BMX160.mag.MAG_Z;


//	/*******************************以下代码仿照大疆例程修改******************************************/
//	//低通滤波
//		accel_fliter_1[0] = accel_fliter_2[0];
//		accel_fliter_2[0] = accel_fliter_3[0];

//		accel_fliter_3[0] = accel_fliter_2[0] * fliter_num[0] + accel_fliter_1[0] * fliter_num[1] + mympu->acc.origin[xx] * fliter_num[2];

//		accel_fliter_1[1] = accel_fliter_2[1];
//		accel_fliter_2[1] = accel_fliter_3[1];

//		accel_fliter_3[1] = accel_fliter_2[1] * fliter_num[0] + accel_fliter_1[1] * fliter_num[1] + mympu->acc.origin[yy] * fliter_num[2];

//		accel_fliter_1[2] = accel_fliter_2[2];
//		accel_fliter_2[2] = accel_fliter_3[2];

//		accel_fliter_3[2] = accel_fliter_2[2] * fliter_num[0] + accel_fliter_1[2] * fliter_num[1] + mympu->acc.origin[yy] * fliter_num[2];

		MahonyAHRSupdate(BMX160.gyro_f.GYR_X, BMX160.gyro_f.GYR_Y, BMX160.gyro_f.GYR_Z, BMX160.acc_f.ACC_X, BMX160.acc_f.ACC_Y, BMX160.acc_f.ACC_Z,
										BMX160.mag_f.MAG_X,BMX160.mag_f.MAG_Y,BMX160.mag_f.MAG_Z);
		quaternionToEuler(q0,q1,q2,q3,&BMX160.eularangles.Pitch,&BMX160.eularangles.Roll,&BMX160.eularangles.Yaw);
}

#include "nrf_log.h"
volatile uint8_t bmx160_acc,bmx160_gyro,bmx160_mag;
extern uint8_t bmp280_flag;
void  GPIO_EXTI_Callback(uint32_t GPIO_Pin, uint8_t action)
{
//		NRF_LOG_INFO("GPIO_EXTI_Callback");
//		Uart_printf(0,"GPIO_EXTI_Callback");
	if(action == GPIOTE_CONFIG_POLARITY_HiToLo){
    switch (GPIO_Pin)
    {
			case ACC_INT://ACC	对应INT1
					BMX160_Get_ACC();
					bmx160_acc = 1;
					if(bmp280_flag){bmp280_get_uncomp_data(&uncomp_data, &bmp280);}
					break;
			case GYRO_INT://GYRO		对应INT3
					BMX160_Get_GYR();
					BMX160_Get_MAG();
					bmx160_gyro = 1;
					break;
			default:
					break;
    }
	}
}


