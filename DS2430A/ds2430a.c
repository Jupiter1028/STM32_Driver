#include "ds2430a.h"


/******************************************************************************************
* Function name ：OnewireReadROM
* Description : 单总线设备ROM信息
* Input Parameter ：ROM_Address  64bitROM数据地址
* Return ：无
* Others : 返回数据直接写入64bitROM数据地址(仅针对1个单总线设备)
******************************************************************************************/
void OnewireReadROM(uint8_t ROM_Address[8]){
	if( OnewireReset() == 1)
	{
		Delay_ms(1);
		OnewireWriteByte(0x33);       //Read ROM[33h]指令
		Delay_us(60);
		ROM_Address[0] = OnewireReadByte();	
		for(uint8_t i = 0;i<6 ;i++ ){
			ROM_Address[1+i] = OnewireReadByte();
		}
		ROM_Address[7] = OnewireReadByte();
//		printf("family:%x \r\n",ROM_Address[0] );
//		printf("ROM: \r\n " );
//		for(uint8_t j = 0;j<6 ;j++ ){
//			printf("%x ",ROM_Address[j+1] );
//		}
//		printf(" \r\n");
//		printf("DS2430_CRC:%x \r\n",ROM_Address[7] );
//	}else
//		printf("Onewire Devices read Error \r\n" );
  }
}

/******************************************************************************************
* Function name ：OnewireMatchROM()
* Description : 根据ROM匹配单总线设备
* Input Parameter ：ROM_Address  64bitROM数据地址
* Return ：无
* Others : 
******************************************************************************************/
void OnewireMatchROM(uint8_t ROM_Address[8]){
	if( OnewireReset() == 1)
	{
		OnewireWriteByte(0x55);       
		Delay_us(60);
    OnewireWriteByte(ROM_Address[0]);        
		for(uint8_t i = 0;i<6 ;i++ ){
			OnewireWriteByte( ROM_Address[1+i] );
		}
		OnewireWriteByte( ROM_Address[7] );
  }
}

/******************************************************************************************
* Function name ：OnewireSkipROM()
* Description : 跳过ROM直接执行后续读写操作
* Input Parameter ：无
* Return ：无
* Others : 仅支持单个单总线设备
******************************************************************************************/
void OnewireSkipROM(void){
//  uint8_t state = 0;
	if( OnewireReset() == 1)
	{
		Delay_ms(1);
		OnewireWriteByte(0xcc);       //SKIP ROM[CCh] OK
//    printf("Skip ROM success \r\n" );
   }
//  else
//		printf("Onewire Devices read Error \r\n" );
}



/******************************************************************************************
* Function name ：OnewireSearchROM()
* Description : 搜索单总线设备
* Input Parameter ：无
* Return ：
* Others : 该函数待实现
******************************************************************************************/
//搜索设备  暂时不用
void OnewireSearchROM(void);

/******************************************************************************************
* Function name ：ReadStatusRegister(void)
* Description : 读取单总线设备的状态寄存器
* Input Parameter ：无
* Return ：
*       @1:解锁状态
*       @0:锁定状态
* Others : 仅支持单个单总线设备
******************************************************************************************/
uint8_t ReadStatusRegister(void){
	uint8_t state = 0;
	if( OnewireReset() == 1){
		Delay_ms(1);
		OnewireWriteByte(0x66);           //读取寄存器状态
		Delay_us(100);
		OnewireWriteByte(0x00);           //确认字节 
		state = OnewireReadByte();
		if( ( state % 3)!=0 ){             //低位为00锁定
//			printf(" StatusRegister Locked \r\n" );
			return 0;
		}else
//			printf("StatusRegister:%X\r\n", state);
			return 1;
	}else
//		printf("No devices \r\n");
		return 0;
}

/******************************************************************************************
* Function name ：ReadScratchpad(uint8_t start_address_nByte,uint8_t length_Byte)
* Description : 读取单总线设备的暂存器数据
* Input Parameter ：
*       @start_address_nByte:起始地址(0x00~0x1F)   
*       @length_Byte:数据长度(0~32)
*       @main_data[8]:数据读出的数据保存地址
* Return ：无
* Others : 仅支持单个单总线设备
******************************************************************************************/
void ReadScratchpad(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t main_data[8]){
//  printf("ReadScratchpad! \r\n");
//  uint8_t state = 0;
  OnewireSkipROM();
  Delay_us(100);
  OnewireWriteByte(0xAA);           //读暂存
	Delay_us(100);
	OnewireWriteByte(start_address_nByte);           //开始地址 
  for(uint8_t i= 0;i < 31 ; i++ ){
				main_data[start_address_nByte+i] =	OnewireReadByte();        //循环把全部数据读出
				} 
  OnewireReset();                   //读完复位
//  printf("ReadScratchpad_data: \r\n");
//  for(uint8_t j= 0;j < 32 ; j++ ){
//			printf("%x ",main_data[j] );        //循环把全部数据读出
//		} 
//  printf("\r\n");
}

/******************************************************************************************
* Function name ：WriteScratchpad(uint8_t start_address_nByte,uint8_t length_Byte)
* Description : 写数据到暂存器
* Input Parameter ：
*       @start_address_nByte:要写入的起始地址(0x00~0x1F)   
*       @length_Byte:数据长度(0~32)
*       @main_data[8]:要写入的数据
* Return ：无
* Others : 仅支持单个单总线设备
******************************************************************************************/
void WriteScratchpad(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t new_data[32]){
//  printf("WriteScratchpad! \r\n");
    OnewireSkipROM();
    Delay_us(100);
    OnewireWriteByte(0x0F);           //写暂存指令
    Delay_us(100);
    OnewireWriteByte(start_address_nByte);         
    Delay_us(100); 
    for(uint8_t i= 0;i < length_Byte ; i++ ){
			  OnewireWriteByte(new_data[start_address_nByte+i]);
		} 
    Delay_us(100);
}

/******************************************************************************************
* Function name ：WriteMemory(uint8_t start_address_nByte,uint8_t length_Byte)
* Description : 写数据到存储器
* Input Parameter ：
*       @start_address_nByte:要写入的起始地址(0x00~0x1F)   
*       @length_Byte:数据长度(0~32)
*       @main_data[8]:要写入的数据
* Return ：无
* Others : 仅支持单个单总线设备
******************************************************************************************/
void WriteMemory(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t new_data[32]){
    uint8_t scratch_data[32];
//  printf("WriteScratchpad! \r\n");
    WriteScratchpad(start_address_nByte,length_Byte,new_data);
    ReadScratchpad(start_address_nByte,32,scratch_data);
    Delay_us(100);

    OnewireSkipROM();
    Delay_us(100);
    OnewireWriteByte(0x55);           //复制暂存数据到Memory
    Delay_us(100);
    OnewireWriteByte(0xA5);           //确认写入
      
    Delay_ms(12);                     //等待10ms,以便数据写入     
}
/******************************************************************************************
* Function name ：ReadMemoryAllData(uint8_t ROM_data[8])
* Description : 读出存储器全部数据
* Input Parameter ：
*       @ROM_data[8]:读出的数据保存地址  
* Return ：无
* Others : 仅支持单个单总线设备
******************************************************************************************/
void ReadMemoryAllData(uint8_t ROM_data[8]){
//  uint8_t state = 0;
  OnewireSkipROM();
  Delay_us(100);
  OnewireWriteByte(0xF0);           //读Memory
	Delay_us(100);
	OnewireWriteByte(0x00);           //从0x00地址开始 
  Delay_ms(7);                      //等待数据写入暂存器
  for(uint8_t i= 0;i < 32 ; i++ ){
				ROM_data[i] =	OnewireReadByte();        //循环把全部数据读出
			} 
  OnewireReset();                   //读完复位
//  printf("main_data: \r\n");
//  for(uint8_t j= 0;j < 32 ; j++ ){
//			printf("%2X ",main_data[j] );        //循环把全部数据读出
//		} 
}

/******************************************************************************************
* Function name ：ReadMemory(uint8_t start_address_nByte,uint8_t length_Byte)
* Description : 读出存储器部分数据
* Input Parameter ：
*       @start_address_nByte:要读的起始地址(0x00~0x1F)   
*       @length_Byte:数据长度(0~32)
*       @ROM_data[8]:读出的数据保存地址
* Return ：无
* Others : 仅支持单个单总线设备
******************************************************************************************/
void ReadMemory(uint8_t start_address_nByte,uint8_t length_Byte,uint8_t ROM_data[8]){
//  printf("ReadMemory! \r\n");
  OnewireSkipROM();
  Delay_us(100);
  OnewireWriteByte(0xF0);                                //读Memory
	Delay_us(100);
	OnewireWriteByte(0X00+start_address_nByte);           //开始地址 
  Delay_ms(7);                                           //等待数据写入暂存器
  for(uint8_t i= 0;i < length_Byte ; i++ ){
				ROM_data[start_address_nByte+i] =	OnewireReadByte();        //循环把全部数据读出
			} 
  OnewireReset();                                       //读完复位
//  printf("main_data: \r\n");
//  for(uint8_t j= 0;j < length_Byte ; j++ ){
//			printf("%2X ",main_data[start_address_nByte+j] );        //循环把全部数据输出
//		} 
}

/******************************************************************************************
* Function name ：ReadApplicaitionRegister(uint8_t APPregAddr)
* Description : 读DS2430A application寄存器数据(8Byte)
* Input Parameter ：
*       @APPregAddr:起始地址(默认为0x00)
*       @APPReg_data[8]:数据保存地址
* Return ：无
* Others : 仅支持单个单总线设备
******************************************************************************************/
void ReadApplicaitionRegister(uint8_t APPregAddr,uint8_t APPReg_data[8]){
	if( ReadStatusRegister() == 1){
		if( OnewireReset() == 1)
			{
				Delay_ms(1);
				OnewireWriteByte(0xC3);          //读Appliciation寄存器
				OnewireWriteByte(APPregAddr);			 //要读的地址
				Delay_us(60);
				for(uint8_t i= 0;i < 8 ; i++ ){
					APPReg_data[i] = OnewireReadByte();
				}
				//打印读到的信息
//				for(uint8_t j = 0;j<8 ;j++ ){
//						printf("ROM: %x ",App_reg[j] );
//				}
			}
		}
	return;
}

/******************************************************************************************
* Function name ：WriteApplicaitionRegister
* Description : 写DS2430A application寄存器数据(8Byte)
* Input Parameter ：
*       @APPReg_data[8]:要写入的数据
* Return ：无
* Others : 仅支持单个单总线设备,[由于此数据只能写入一次,所以此函数未验证]
******************************************************************************************/
void WriteApplicaitionRegister(uint8_t APP_data[8]){
//	if( OnewireReset() == 1)
//	{
//		Delay_ms(1);
//		OnewireWriteByte(0xF0);       
//		Delay_us(60);
//		family  = OnewireReadByte();	
//		for(uint8_t i = 0;i<6 ;i++ ){
//			APP_data[i] = OnewireReadByte();
//		}
//		DS2430_CRC = OnewireReadByte();
//	
//		printf("family:%x \r\n",family );
//		
//		for(uint8_t j = 0;j<6 ;j++ ){
//			printf("ROM: %x ",ROM[j] );
//		}
//		printf(" \r\n");
//		printf("DS2430_CRC:%x \r\n",DS2430_CRC );
//	}else
//		printf("Not Command \r\n" );
}

/******************************************************************************************
* Function name ：CopyScratchpad(void)
* Description : 把DS2430A_MAIN暂存器数据写入储存器
* Input Parameter ：无
* Return ：无
* Others : 仅支持单个单总线设备,写入数据前需要先把数据写入暂存器
******************************************************************************************/
void CopyScratchpad(void){
//  printf("CopyScratchpad! \r\n");
//  uint8_t state = 0;
  OnewireWriteByte(0x55);           //copy to Memory
	Delay_us(100);
	OnewireWriteByte(0xA5);           //确认写入
  Delay_ms(15);                     //等待数据写入Memory
  OnewireReset();                   //读完复位
}


