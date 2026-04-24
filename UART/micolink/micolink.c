/*
	micolink数据解码程序
*/
#include "micolink.h"
#include "hal_oled.h" 
#include "stdio.h" 
/*
说明： 用户使用micolink_decode作为串口数据处理函数即可
*/

bool micolink_parse_char(MICOLINK_MSG_t* msg, uint8_t data);

/******************************************************************************
函数说明：micolink数据解码
入口数据：data 收到的数据
返回值： true 解码成功
	   false 解码失败
******************************************************************************/
bool micolink_decode(uint8_t data)
{
    static MICOLINK_MSG_t msg;
    if(micolink_parse_char(&msg, data) == false){
		return 0;
	}
    
    switch(msg.msg_id)
    {
        case MICOLINK_MSG_ID_RANGE_SENSOR:
        {
            MICOLINK_PAYLOAD_RANGE_SENSOR_t payload;
            memcpy(&payload, msg.payload, msg.len);

            /*
                此处可获取传感器数据:     
                距离        = payload.distance;
                强度        = payload.strength;
                距离状态    = payload.tof_status;
                光流速度x轴 = payload.flow_vel_x;
                光流速度y轴 = payload.flow_vel_y;
                光流质量    = payload.flow_quality;	
                光流状态    = payload.flow_status;
            */
			distance = payload.distance;
			strength = payload.strength;
			tof_status = payload.tof_status;
			flow_vel_x = payload.flow_vel_x;
			flow_vel_y = payload.flow_vel_y;
			flow_quality = payload.flow_quality;
			flow_quality = payload.flow_status;
			return true;
            break;
        } 
        default:
			return false;
            break;
    }
}


/******************************************************************************
函数说明：micolink数据包校验
入口数据：msg 要校验的数据
返回值： true 校验通过
	   false 校验未通过
******************************************************************************/
bool micolink_check_sum(MICOLINK_MSG_t* msg)
{
    uint8_t length = msg->len + 6;
    uint8_t temp[MICOLINK_MAX_LEN];
    uint8_t checksum = 0;

    memcpy(temp, msg, length);

    for(uint8_t i=0; i<length; i++)
    {
        checksum += temp[i];
    }
    if(checksum == msg->checksum)
        return true;
    else
        return false;
}

/******************************************************************************
函数说明：把数据转换为micolink数据包
入口数据：msg 数据包地址
		data 要转换的数据
返回值： true 转换成功
	   false 转换失败
******************************************************************************/
bool micolink_parse_char(MICOLINK_MSG_t* msg,uint8_t data)
{
    switch(msg->status){
		case 0:     //帧头
			if(data == MICOLINK_MSG_HEAD)
			{
				msg->head = data;
				msg->status++;
			}
			break;       
		case 1:     // 设备ID
			msg->dev_id = data;
			msg->status++;
			break;
		
		case 2:     // 系统ID
			msg->sys_id = data;
			msg->status++;
			break;  
		case 3:     // 消息ID
			msg->msg_id = data;
			msg->status++;
			break;  
		case 4:     // 包序列
			msg->seq = data;
			msg->status++;
			break; 
		case 5:     // 负载长度
			msg->len = data;
			if(msg->len == 0)
				msg->status += 2;
			else if(msg->len > MICOLINK_MAX_PAYLOAD_LEN)
				msg->status = 0;
			else
				msg->status++;
			break;
		case 6:     // 数据负载接收
			msg->payload[msg->payload_cnt++] = data;
			if(msg->payload_cnt == msg->len)
			{
				msg->payload_cnt = 0;
				msg->status++;
			}
			break;      
		case 7:     // 帧校验
			msg->checksum = data;
			msg->status = 0;
			if(micolink_check_sum(msg))
			{
				return true;
			}      
		default:
			msg->status = 0;
			msg->payload_cnt = 0;
			break;
    }
    return false;
}

