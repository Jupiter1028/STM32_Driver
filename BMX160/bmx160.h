#ifndef __BMX160_H
#define __BMX160_H

#include "spi.h"
#include "nrf_delay.h"
#include "TrackingPro_1V2.h"
#include "uart.h"
#include <math.h>

#define BMX160_OK 0
#define BMX160_Error 1
//寄存器地址
#define R_CHIP_ID 0X00       //芯片识别码
#define R_ERR_REG 0X02       //错误标志
#define R_PMU_STATUS 0X03    //当前功率模式
#define R_DATA_0 0X04        //磁力计X轴LSB
#define R_DATA_1 0X05        //磁力计X轴MSB
#define R_DATA_2 0X06        //磁力计Y轴LSB
#define R_DATA_3 0X07        //磁力计Y轴MSB
#define R_DATA_4 0X08        //磁力计Z轴LSB
#define R_DATA_5 0X09        //磁力计Z轴MSB
#define R_DATA_6 0X0A        //RHALL_LSB
#define R_DATA_7 0X0B        //RHALL_MSB
#define R_DATA_8 0X0C        //陀螺仪X轴LSB
#define R_DATA_9 0X0D        //陀螺仪X轴MSB
#define R_DATA_10 0X0E       //陀螺仪Y轴LSB
#define R_DATA_11 0X0F       //陀螺仪Y轴MSB
#define R_DATA_12 0X10       //陀螺仪Z轴LSB
#define R_DATA_13 0X11       //陀螺仪Z轴MSB
#define R_DATA_14 0X12       //加速度X轴LSB
#define R_DATA_15 0X13       //加速度X轴MSB
#define R_DATA_16 0X14       //加速度Y轴LSB
#define R_DATA_17 0X15       //加速度Y轴MSB
#define R_DATA_18 0X16       //加速度Z轴LSB
#define R_DATA_19 0X17       //加速度Z轴MSB
#define R_SENSORTIME_0 0X18  //传感器时间0：7
#define R_SENSORTIME_1 0X19  //传感器时间8-15
#define R_SENSORTIME_2 0X1A  //传感器时间16-23
#define R_STATUS 0X1B        //状态标志
#define R_INT_STATUS_0 0X1C  //中断状态标志
#define R_INT_STATUS_1 0X1D  //中断状态标志
#define R_INT_STATUS_2 0X1E  //中断状态标志
#define R_INT_STATUS_3 0X1F  //中断状态标志
#define R_TEMPERATURE_0 0X20 //传感器温度
#define R_TEMPERATURE_1 0X21 //传感器温度
#define R_FIFO_LENGTH_0 0X22 //FIFO数据读出状态
#define R_FIFO_LENGTH_1 0X23 //FIFO数据读出状态
#define R_FIFO_DATA 0X24     //FIFO 数据读出寄存器
#define R_ACC_CONF 0X40      //加速度数据速率、带宽和加速度传感器的读取模式
#define R_ACC_RANGE 0X41     //加速度g范围
#define R_GYR_CONF 0X42      //陀螺仪数据速率、带宽和加速度传感器的读取模式
#define R_GYR_RANGE 0X43     //角速度范围
#define R_MAG_CONF 0X44      //磁力计数据速率、带宽和加速度传感器的读取模式
#define R_FIFO_DOWNS 0X45    //FIFO 的加速度和陀螺仪数据的下采样率
#define R_FIFO_CONFIG_0 0X46 //读取或设置当前FIFO水印级别、设置 FIFO的不同操作模式
#define R_FIFO_CONFIG_1 0X47 //读取或设置当前FIFO水印级别、设置 FIFO的不同操作模式
#define R_MAG_IF_0 0X4C      //磁力计寻址的寄存器   
#define R_MAG_IF_1 0X4D      //磁力计寻址的寄存器		读取的地址
#define R_MAG_IF_2 0X4E      //磁力计寻址的寄存器		写入的地址
#define R_MAG_IF_3 0X4F      //磁力计寻址的寄存器		写入的数据
#define R_INT_EN_0 0X50      //控制中断引擎的开启
#define R_INT_EN_1 0X51      //控制中断引擎的开启
#define R_INT_EN_2 0X52      //控制中断引擎的开启
#define R_INT_OUT_CTRL 0X53  //中断引脚的电气定义
#define R_INT_LATCH 0X54     //中断复位位和中断模式选择
#define R_INT_MAP_0 0X55     //控制将哪些中断信号映射到INT1和INT2引脚
#define R_INT_MAP_1 0X56     //控制将哪些中断信号映射到INT1和INT2引脚
#define R_INT_MAP_2 0X57     //控制将哪些中断信号映射到INT1和INT2引脚
#define R_INT_DATA_0 0X58    //中断组的数据源定义
#define R_INT_DATA_1 0X59    //中断组的数据源定义
#define R_INT_LOWHIGH_0 0X5A //低G中断的延迟时间定义
#define R_INT_LOWHIGH_1 0X5B //包含low-g中断的阈值定义
#define R_INT_LOWHIGH_2 0X5C //低G中断模式的选择，低G中断迟滞的设置，以及高G中断迟滞的设置。
#define R_INT_LOWHIGH_3 0X5D //高g中断的延迟时间定义
#define R_INT_LOWHIGH_4 0X5E //高g中断的阈值定义
#define R_INTMOTION_0 0X5F   //对任意运动中断和慢速/无运动中断触发延迟所要评估的样本数的定义。
#define R_INTMOTION_1 0X60   //任意运动中断的阈值定义
#define R_INTMOTION_2 0X61   //慢速/无动作中断的阈值定义
#define R_INTMOTION_3 0X62   //慢动作/无动作配置
#define R_INT_TAP_0 0X63     //单击和双击中断的定时定义
#define R_INT_TAP_1 0X64     //单击和双击中断的阈值定义
#define R_INT_ORIENT_0 0X65  //方向中断的迟滞、阻塞和模式的定义
#define R_INT_ORIENT_1 0X66  //轴方向的定义，上/下屏蔽，以及方向中断的theta阻断角。
#define R_INT_FLAT_0 0X67    //平面中断的平坦阈值角度的定义
#define R_INT_FLAT_1 0X68    //平面中断保持时间和平面中断迟滞的定义
#define R_FOC_CONF 0X69      //加速度计和陀螺仪快速偏移补偿的配置设置
#define R_CONF 0X6A          //NVM编程配置
#define R_IF_CONF 0X6B       //三线SPI或四线SPI设置
#define R_PMU_TRIGGER 0X6C   //设置触发条件以改变陀螺仪电源模式
#define R_SELF_TEST 0X6D     //传感器自检配置和触发器的设置
#define R_NV_CONF 0X70       //数字接口的NVM设置
#define R_OFFSET_0 0X71      //加速度计和陀螺仪的偏移补偿值
#define R_OFFSET_1 0X72      //加速度计和陀螺仪的偏移补偿值
#define R_OFFSET_2 0X73      //加速度计和陀螺仪的偏移补偿值
#define R_OFFSET_3 0X74      //加速度计和陀螺仪的偏移补偿值
#define R_OFFSET_4 0X75      //加速度计和陀螺仪的偏移补偿值
#define R_OFFSET_5 0X76      //加速度计和陀螺仪的偏移补偿值
#define R_OFFSET_6 0X77      //加速度计和陀螺仪的偏移补偿值
#define R_STEP_CNT_0 0X78    //步数
#define R_STEP_CNT_1 0X79    //步数
#define R_STEP_CONF_0 0X7A   //步数检测器的配置
#define R_STEP_CONF_1 0X7B   //步数检测器的配置
#define R_CMD 0X7E           //命令寄存器触发操作，如软复位、NVM编程等

//电源模式
#define BMX160_ACC_Normal_mode 0 //正常模式
#define BMX160_ACC_LPower_mode 1 //低功耗模式
//正常模式下
#define BMX160_ACC_Normal_Oversampling 0 //正常滤波器
#define BMX160_ACC_OSR2_Oversampling -1  //2倍过采样
#define BMX160_ACC_OSR4_Oversampling -2  //4倍过采样
//低功耗模式
#define BMX160_ACC_1_Undersampling 1     //1平均样本
#define BMX160_ACC_2_Undersampling 2     //2平均样本
#define BMX160_ACC_4_Undersampling 4     //4平均样本
#define BMX160_ACC_8_Undersampling 8     //8平均样本
#define BMX160_ACC_16_Undersampling 16   //16平均样本
#define BMX160_ACC_32_Undersampling 32   //32平均样本
#define BMX160_ACC_64_Undersampling 64   //64平均样本
#define BMX160_ACC_128_Undersampling 128 //128平均样本

//输出速率
#define BMX160_ACC_ODR_0_78125 0x1 //输出速率0.78125/HZ
#define BMX160_ACC_ODR_1_5625 0x2  //输出速率1.5625/HZ
#define BMX160_ACC_ODR_3_125 0x3   //输出速率3.125/HZ
#define BMX160_ACC_ODR_6_25 0x4    //输出速率6.25/HZ
#define BMX160_ACC_ODR_12_5 0x5    //输出速率12.5/HZ
#define BMX160_ACC_ODR_25 0x6      //输出速率25/HZ
#define BMX160_ACC_ODR_50 0x7      //输出速率50/HZ
#define BMX160_ACC_ODR_100 0x8     //输出速率100/HZ
#define BMX160_ACC_ODR_200 0x9     //输出速率200/HZ
#define BMX160_ACC_ODR_400 0xa     //输出速率400/HZ
#define BMX160_ACC_ODR_800 0xb     //输出速率800/HZ
#define BMX160_ACC_ODR_1600 0xc    //输出速率1600/HZ
#define BMX160_ACC_ODR		BMX160_ACC_ODR_100																		//加速度计输出速率

#define BMX160_CMD_PMU_ACC_Suspend 0x10     //加速度计暂停模式
#define BMX160_CMD_PMU_ACC_Normal 0x11      //加速度计普通模式
#define BMX160_CMD_PMU_ACC_LowPower 0x12    //加速度计低功耗模式
#define BMX160_CMD_PMU_GYR_Suspend 0x14     //陀螺仪计暂停模式
#define BMX160_CMD_PMU_GYR_Normal 0x15      //陀螺仪计普通模式
#define BMX160_CMD_PMU_GYR_FastStartUp 0x17 //陀螺仪计快速启动模式
#define BMX160_CMD_PMU_MAG_Suspend 0x18     //磁力计计暂停模式
#define BMX160_CMD_PMU_MAG_Normal 0x19      //磁力计计普通模式
#define BMX160_CMD_PMU_MAG_LowPower 0x1a    //磁力计计低功耗模式
#define BMX160_CMD_StartFoc 0x03            //加速度和陀螺仪的快速偏移校准
#define BMX160_CMD_ProgNvm 0xa0             //写入NVM
#define BMX160_CMD_FifoFlush 0xb0           //清除Fifo中所有数据
#define BMX160_CMD_IntReset 0xb1            //重置中断引擎
#define BMX160_CMD_Softreset 0xb6           //重启
#define BMX160_CMD_StepCntClr 0xb2          //步数计数器重置

#define BMX160_CHIP_ID 			0xD8          			//芯片ID
#define BMX160_ACC_RANG_2G  0x03          			//加速度计量程±2g
#define BMX160_ACC_RANG_4G  0x05          			//加速度计量程±4g
#define BMX160_ACC_RANG_8G  0x08          			//加速度计量程±8g
#define BMX160_ACC_RANG_16G 0x0C          			//加速度计量程±16g
#define BMX160_ACC_RANG  BMX160_ACC_RANG_2G    

#define BMX160_GYR_ODR_25      0x06             // 100/(2^(8-x))        100/2^2
#define BMX160_GYR_ODR_50      0x07							//输出速率50HZ
#define BMX160_GYR_ODR_100     0x08             //100/(2^(8-8)) = 100
#define BMX160_GYR_ODR_200     0x09
#define BMX160_GYR_ODR_400     0x0A             
#define BMX160_GYR_ODR_800     0x0B
#define BMX160_GYR_ODR_1600    0x0C
#define BMX160_GYR_ODR_3200    0x0D
#define BMX160_GYR_ODR    BMX160_GYR_ODR_100  																	//陀螺仪输出速率

#define BMX160_GYR_RANGE_125   0x04							//125/s
#define BMX160_GYR_RANGE_250   0x03
#define BMX160_GYR_RANGE_500   0x02
#define BMX160_GYR_RANGE_1000  0x01
#define BMX160_GYR_RANGE_2000  0x00							//2000/s
#define BMX160_GYR_RANGE 	BMX160_GYR_RANGE_2000

//磁力计模式
#define BMX160_MAG_Mode_LowPower 0      //低功耗模式
#define BMX160_MAG_Mode_Normal 1        //普通模式
#define BMX160_MAG_Mode_Strengthen 2    //增强模式
#define BMX160_MAG_Mode_HighPrecision 3 //高精度模式

#define MAG_XY_Mode BMX160_MAG_Mode_HighPrecision  //设置磁力计XY轴模式
#define MAG_Z_Mode  BMX160_MAG_Mode_HighPrecision  //设置磁力计Z轴模式

//输出范围
#define BMX160_MAG_RANGE_0_78125  0x01
#define BMX160_MAG_RANGE_1_5625 	0x02
#define BMX160_MAG_RANGE_3_125 		0x03
#define BMX160_MAG_RANGE_6_25 		0x04
#define BMX160_MAG_RANGE_12_5 		0x05
#define BMX160_MAG_RANGE_25 			0x06
#define BMX160_MAG_RANGE_50 			0x07
#define BMX160_MAG_RANGE_100 			0x08
#define BMX160_MAG_RANGE_200 			0x09
#define BMX160_MAG_RANGE_400 			0x0A
#define BMX160_MAG_RANGE_800 			0x0B
#define BMX160_MAG_RANGE 			    BMX160_MAG_RANGE_200

#define BMX160_FOC_GYR_ON  0x1          //开启陀螺仪三轴快速偏移补偿
#define BMX160_FOC_GYR_OFF 0x0          //关闭陀螺仪三轴快速偏移补偿
#define BMX160_FOC_GYR BMX160_FOC_GYR_ON          //陀螺仪三轴快速偏移补偿

#define BMX160_FOC_ACC_Recoup_Ban 0x00  //加速度计禁用补偿目标值
#define BMX160_FOC_ACC_Recoup_P_1g 0x01 //加速度计补偿目标值1g
#define BMX160_FOC_ACC_Recoup_N_1g 0x02 //加速度计补偿目标值-1g
#define BMX160_FOC_ACC_Recoup_0g 0x03   //加速度计补偿目标值0g
#define BMX160_FOC_ACC_Recoup BMX160_FOC_ACC_Recoup_0g   		//加速度计补偿目标值0g

#define FOC_ACC_X		BMX160_FOC_ACC_Recoup_0g
#define FOC_ACC_Y		BMX160_FOC_ACC_Recoup_0g
#define FOC_ACC_Z		BMX160_FOC_ACC_Recoup_P_1g

#define BMX160_off_ACC_ON 0x1           //开启加速度计的偏移补偿
#define BMX160_off_ACC_OFF 0x0          //关闭加速度计的偏移补偿
#define BMX160_off_ACC BMX160_off_ACC_ON

#define BMX160_off_GYR_ON 0x1           //开启加陀螺仪的偏移补偿
#define BMX160_off_GYR_OFF 0x0          //关闭加陀螺仪的偏移补偿
#define BMX160_off_GYR	BMX160_off_GYR_ON

#define BMX160_STEP_MODE_NORMAL		 0x00 				//普通模式适合大多数应用
#define BMX160_STEP_MODE_SENSITIVE 0x01	  //敏感模式，适合体重轻、身材矮小的人
#define BMX160_STEP_MODE_ROBUST    0x02			//鲁棒模式（检测到多误报可以使用）
#define BMX160_STEP_MODE	BMX160_STEP_MODE_SENSITIVE

//										BIT          				 0								1
//										 7						INT2_DISENABLE			INT2_ENABLE				INT2使能
//										 6						INT2_MODE_PP				INT2_MODE_OD			推挽/开漏
//										 5						INT2_LEV_LOW				INT2_LEV_HIGH			低/高 有效
//										 4						INT2_EDGE_TIGGER		INT2_EDGE_LEVEL		边沿/电平
//										 3						INT1_DISENABLE			INT1_ENABLE
//										 2						INT1_MODE_PP				INT1_MODE_OD	
//										 1						INT1_LEV_LOW				INT1_LEV_HIGH	
//										 0						INT1_EDGE_TIGGER		INT1_EDGE_LEVEL

#define BMX160_INT_MODE 0x99           //中断模式设置   	使能INT1&2 开漏,低 边沿有效

#define GG 9.80665f
#define MPU_ACCE_M_S_2 	0.0005985504150390625f	//(2 * GG / 32768.0) //单位换算，将LSB化为m/(s^2)，GG前面的"X"需根据量程修改																																						
#define MPU_GYRO_TO_DPS 0.06103515625f		//(2000 / 32768.0)     //单位换算，将LSB化为°/S(度每秒)
#define MPU_GYRO_TO_RDS 0.00106526443603169529841533860381f				//单位换算，将LSB化为rad/S(弧度每秒)		0.06103515625f*0.01745
																																	//大疆:(AD/32768)*(2000*pi/180)      rad/s		0.00106526443603169529841533860381f
																																	//大疆:(AD/32768)*2000				 	     deg/s  

typedef struct accdata
{
    float ACC_X;      
    float ACC_Y;  
    float ACC_Z;  
} ACCdata;

typedef struct gyrdata
{
    float GYR_X;      
    float GYR_Y;  
    float GYR_Z;  
} GYRdata;

typedef struct magdata
{
    float MAG_X;      
    float MAG_Y;  
    float MAG_Z;  
} MAGdata;

typedef struct quaternion
{
    float Q1;      
    float Q2;  
    float Q3;  
	  float Q4;  
}Quaternion;

typedef struct eulerAngles
{
    float Yaw;      	//绕Z轴旋转
    float Pitch;  		//绕Y轴旋转
    float Roll;   		//绕X轴旋转
}EulerAngles;

struct _bmx160
{
	ACCdata acc;			//原始数据
  GYRdata gyro;
	MAGdata mag;
	
	ACCdata acc_f;		//滤波后的数据
  GYRdata gyro_f;
	MAGdata mag_f;
	
	float Angle_X_Final;   //最终角度估计值
	float Angle_Y_Final; 
	float Angle_Z_Final; 
	
	union
  {
    uint16_t data[3];   
    uint8_t buff[6];
  } ACC,GYRO,MAG;						//这里存放读取到的原始数据
	
	union
  {
		uint16_t data;
    uint8_t buff[2];   
  } STEPS;							//这里存放读取到的步数数据
	
	union
	{
		uint8_t time[3];
		uint32_t time_32;
	}TIME;

	Quaternion qua;						 //存储转换后的四元数数据	
	EulerAngles	eularangles;	 //存储转换后的欧拉角
  float Temperature;         //存储转换后温度数据
	uint8_t chip_id;
};

typedef struct _bmx160 bmx160;
extern bmx160 BMX160;

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160传感器初始化配置
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
uint8_t BMX160_Init(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160传感器主动读取加速度计数据
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
uint8_t BMX160_Get_ACC(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160传感器主动读取陀螺仪数据
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
void BMX160_Get_GYR(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160传感器主动读取地磁仪数据
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
void BMX160_Get_MAG(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160传感器内部时间读取
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
void BMX160_Get_Time(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160计步模式配置
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
void BMX160_Set_Steps_Mode(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160计步数据读取
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
void BMX160_Get_Steps(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160计步数据清零
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
void BMX160_Steps_Clear(void);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160内部温度读取
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
uint8_t BMX160_Get_Temperature(float *DATA);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160寄存器配置
 * 		@Addr:寄存器地址
 * 		@Val:寄存器参数 
 * @author:   
 * @date: 2023/09/25
 * @note:
 ****************************************************/
void BMX160_Write_Reg(uint8_t Addr, uint8_t Val);

/*!***************************************************
 * @file: bmx160.h
 * @brief: BMX160寄存器配置读取
 * 		@Addr:寄存器地址
 * @author: 
 * @return:寄存器参数  
 * @date: 2023/09/25
 * @note:
 ****************************************************/
uint8_t BMX160_Read_Reg(uint8_t Addr);

/****************************************************
 * @file: BMI088.c
 * @brief: 用于配置GPIO外部中断
 * @author:朱磊    
 * @date:2023/09/25
 * @note:	GPIO外部中断回调函数
 ****************************************************/
void  HAL_GPIO_EXTI_Callback(uint32_t GPIO_Pin, uint8_t action);

void Export_EulerAngles(void);
void IMU_Read(void);
void Export_Data(uint8_t Out_Euler,uint8_t Out_altitude);
#endif

