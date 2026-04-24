#include "key.h"
#include "stdio.h"

//////////////////////////////////////////////////////////////////////////////////  
KeyTypedef Key;
volatile uint16_t	 nSleepTime;//判断系统超时(睡眠时间)
volatile uint8_t	 nkey_flag;//按键标志

static uint32_t s_nTime=0; //按键处理计时
static uint8_t s_nflag=0;	//按键处理判断标志

static uint32_t s_on_Time=0;	//计时



///按键标志与按键值初始化
void KeyInit(void)
{
    Key.nState = 0;
    Key.nValue = 0;
}


///////////////////////////////////////////
//函数：TaskKeyScan
//功能：按键扫描
//参数：无
//返回：无
/////////////////////////////////////
// 按键扫描
// return:按键值
void TaskKeyScan(void)
{
    uint8_t nTmp = 0;
	

    // 检测按键
	
    if( KeyRead == 1)	
    {
        nTmp = KEY_POW;  //KEY_POW==2
    }else;


    
    switch(Key.nState)
    {
    case 0: 
        if(nTmp)
					{
            Key.nState = 1;
            Key.nLastValue = nTmp;	
            Key.nTime = nTimeBase;
					}
				else
					{
						Key.nValue = 0;	
					}
        break;

    case 1:		
        if(nTmp == Key.nLastValue)
					{
							if((nTimeBase - Key.nTime >= 30 ) || (((int32_t)(nTimeBase - Key.nTime) < 0)&&( Key.nTime+nTimeBase>= 30 )))// 消抖30ms
							{
								 if(nTmp == Key.nLastValue)
									{
										Key.nState = 2;	
										Key.nTime = nTimeBase;
										Key.nkey_state=1; 
									//	nSleepTime = 0;
									}
								else 
									{
										Key.nState = 0;
										Key.nValue = 0;	
									}
							}
					}
        else
					{
						Key.nState = 0;
						Key.nValue = 0;	
					}

        break;

    case 2:		

       if(((nTimeBase - Key.nTime >= 500 ) || (((int32_t)(nTimeBase - Key.nTime) < 0)&&(Key.nTime+nTimeBase>= 500 ))))// 长按 > 1S
			 {
          	if(nTmp == KEY_POW)
            {
              Key.nValue = KEY_POW;			
							Key.nState=3;
							Key.nTime = nTimeBase;
            }
						else
							{
									Key.nState = 0;
									Key.nValue = 0;	
							}					
        }
			 
			
				if(((nTimeBase - Key.nTime >= 1000 ) || (((int32_t)(nTimeBase - Key.nTime) < 0)&&(Key.nTime+nTimeBase>= 1000 ))))// 长按 > 1S
				{
					if(Key.nKeyMutex == 0)
					{
							if(nTmp == KEY_POW)	
								{
										Key.nValue = KEY_ONE_S;		
										Key.nKeyMutex = 1;
												
									//	Key.nState=3;
								//		Key.nTime = nTimeBase;
									return;																		
								}  
							else
								{
										Key.nState = 0;
										Key.nValue = 0;	
								}		
						}							
				}
						
					
			
					
				if(Key.nKeyMutex == 1)
				{
					if(((nTimeBase - Key.nTime >= 2300 ) || (((int32_t)(nTimeBase - Key.nTime) < 0)&&(Key.nTime+nTimeBase>= 2300 ))))// 长按 > 3S
						{
							if(nTmp == KEY_POW)	
								{
										Key.nValue = KEY_POW_LONG;										
										Key.nState=3;
										Key.nTime = nTimeBase;
										Key.nKeyMutex = 0;
																		
								}  
							else
							{
									Key.nValue = KEY_START;	
									Key.nState = 0;
								Key.nKeyMutex = 0;
								//	Key.nValue = 0;	
						
							}				
						}
						else
						{
								if(nTmp == 0)	
								{			
									Key.nValue = KEY_START;	
							
									Key.nState = 0;
									Key.nKeyMutex = 0;
							
								
								}		
						}
					}
				
			
				break;
					
			case 3:		
				
				Key.nValue = 0;	
				if(nTmp == 0)	
        {
				
					if(nTimeBase - Key.nTime >= 100 ) 
					{
						if(nTmp == 0)
						{
							Key.nState = 0;						
					//	return;
						}
					}
				}
				
				break;

			default:
					Key.nState = 0;
					break;
			
		}
				
		switch(Key.nkey_state)		
			{		
				case 1:		
					s_on_Time=nTimeBase;
					Key.nkey_state=2;
					break;
				case 2: 
						if(nTimeBase - s_on_Time>= 50  )
						{
							s_nTime=0;
						  Key.nkey_state=0;
						}
						
						break;			

			default:
				Key.nkey_state=0;
					break;		
			}		
			
}




///////////////////////////////////////////
//函数：KeyProcOn
//功能：关机状态下按键处理
//参数：无
//返回：无
/////////////////////////////////////
// 按键处理
void KeyProcOn(void)
{

		uint32_t pwoer_time,pwoer_b_time;	
	
    switch(Key.nValue)
    {
    case KEY_POW:
				s_nflag=1;
				s_nTime=nTimeBase;
        break;
    default:
        break;
    }	
		
		
			switch(s_nflag)		
			{			
				case 1:		
			
			
			printf("\r\n检测电源A"); //打印读到串口
		
					while(((nTimeBase - s_nTime) < 350))  
					{
									pwoer_time=nTimeBase - s_nTime;
				//		printf("pwoer_time=%d\r\n",pwoer_time); //打印读到串口
		
					}
		
					if(pwoer_time<=300)
					{			
								pwoer_time=0;		
								s_nTime=nTimeBase;	
								s_nflag=5;
					}
					else if(pwoer_time>=300)
							{	
								pwoer_time=0;
									printf("\r\n电源A检测错误");						
						    s_nflag=4;
								s_nTime=nTimeBase;
								
 							}
							else;
								
					break;

				case 2:		
					s_nTime=nTimeBase;
					s_nflag=3;

					break;

				case 3: 
						if(nTimeBase - s_nTime>= 50  )
						{
							printf("\r\n MENU_F1_POWER"); //打印读到串口
							s_nTime=0;
						  s_nflag=0;
						}			
						
						break;	
						
				case 4: 
				
						if(((nTimeBase - s_nTime >= 5000 ) || (((int32_t)(nTimeBase - s_nTime) < 0)&&( s_nTime+nTimeBase>= 5000 ))))
						{
						

							
							s_nTime=0;
						  s_nflag=0;
						}						
						break;	
				
			case 5: 	
				do
					{
						pwoer_b_time=nTimeBase - s_nTime;
						printf("pwoer_time=%d\r\n",pwoer_time); //打印读到串口
		
					}while(((nTimeBase - s_nTime) < 350));  
			
					if(pwoer_b_time<=300)
					{			
						pwoer_b_time=0;		
						printf("\r\n检测电源B");		
									
								s_nflag=2;
					}
					else if(pwoer_b_time>=300)
							{	
								pwoer_b_time=0;
								printf("\r\n电源B检测错误");				
						    s_nflag=4;
								s_nTime=nTimeBase;
							
 							}
							else;
			break;					
						
			
	/////////////////////////////////////////////////////								
			default:
				s_nflag=0;
					break;		
			}   

}



///////////////////////////////////////////
//函数：KeyProcOff
//功能：开机状态下按键处理
//参数：无
//返回：无
/////////////////////////////////////
// 按键处理
void KeyProcOff(void)
{
	
		uint32_t powera_time=0,powerb_time=0;
	
    switch(Key.nValue)
    {
		case KEY_ONE_S:			
			printf("\r\nKEY_ONE_S");		
	//		RGB_LED_ALL_OFF();	//关闭所有灯
		//RGB_LED_Write_24Bits_Reset(0xff,0x58,0x10);
		//	RGBColorToColor(0x00ff00,0,10);
		//	RGB_LED_ALL_OFF();	
	
		
				 break;
		case KEY_START:	
			printf("\r\n KEY_START");
				break;

    case KEY_POW_LONG: //长按3S关机键处理		
				printf("\r\n KEY_POW_LONG");		
				s_nTime=nTimeBase;			
				s_nflag=1;			
        break;

    default:
	
        break;
    }
		
			switch(s_nflag)		
			{
					case 1:		
					while(((nTimeBase - s_nTime) < 350))  
					{
								powera_time=nTimeBase - s_nTime;
						printf("\r\n 检测12A电源输出关闭");	
							
					}
					
					if(powera_time<300)
					{
							powera_time=0;	
											
									s_nflag=7;
						
									#ifdef  DEBUG	
													printf("power close\r\n"); //打印读到串口
									#endif
					}
					else if(powera_time>=300)
							{	
								powera_time=0;				
								s_nTime=nTimeBase;
						   s_nflag=6;
							#ifdef  DEBUG	
											printf("power close ERR\r\n"); //打印读到串口
							#endif
						}else;

					break;
							
				case 2:		
					s_nTime=nTimeBase;
					s_nflag=3;

					break;

				case 3: 
						if(nTimeBase - s_nTime>= 50)
						{
							s_nTime=0;
						  s_nflag=0;
						}
						
						break;	
									
				case 5:		
					s_nTime=nTimeBase;
					s_nflag=3;
					
					break;
				
				case 6: 
						if(((nTimeBase - s_nTime >= 5000 ) || (((int32_t)(nTimeBase - s_nTime) < 0)&&(s_nTime+nTimeBase>= 5000 ))))
						{				
							//delay_us(10);						
							s_nTime=0;
						  s_nflag=0;
						}						
						break;	
						
				case 7:	
					do
					{
							powerb_time=nTimeBase - s_nTime;
						
					}while(((nTimeBase - s_nTime) < 350));  
					
					if(powerb_time<300)
					{
						powerb_time=0;
											
									s_nflag=2;
						printf("\r\n 关闭电源B后进入状态2");	
									#ifdef  DEBUG	
													printf("power close\r\n"); //打印读到串口
									#endif
					}
					else if(powerb_time>=300)
							{	
								powerb_time=0;	
						
								s_nTime=nTimeBase;
						   s_nflag=6;
							#ifdef  DEBUG	
											printf("power close ERR\r\n"); //打印读到串口
							#endif
						}else;

					  
							break;	
			default:
				s_nflag=0;
					break;		
			}
		
}
















