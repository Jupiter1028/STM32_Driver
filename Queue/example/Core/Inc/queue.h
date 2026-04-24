/*! 
*  \file cmd_queue.h
*  \brief 串口指令队列
*  \version 1.0
*  \date 2012-2018
*  \copyright 广州大彩光电科技有限公司
*/

#ifndef _CMD_QUEUE
#define _CMD_QUEUE
#include "main.h"

typedef uint8_t qdata;
typedef uint8_t qsize;


typedef uint32_t uint32;
#define QUEUE_MAX_SIZE 512              // 指令接收缓冲区大小，根据需要调整，尽量设置大一些

#define CMD_HEAD 0XEE                                                  //帧头
#define CMD_TAIL 0XFFFCFFFF                                            //帧尾


typedef struct _QUEUE                                             
{                                                                 
    qsize _head;                                                       //队列头
    qsize _tail;                                                       //队列尾
    qdata _data[QUEUE_MAX_SIZE];                                       //队列数据缓存区
}QUEUE;                                                           


/*! 
*  \brief  清空指令数据
*/
extern void queue_reset(void);


/*! 
* \brief  添加指令数据
* \detial 串口接收的数据，通过此函数放入指令队列 
*  \param  _data 指令数据
*/
extern void queue_push(qdata _data);

/*! 
*  \brief  从指令队列中取出一条完整的指令
*  \param  cmd 指令接收缓存区
*  \param  buf_len 指令接收缓存区大小
*  \return  指令长度，0表示队列中无完整指令
*/
extern qsize queue_find_cmd(qdata *cmd,qsize buf_len);


//消息结构体，可以添加内容
typedef struct
{
    uint8_t    cmd_head;                    //帧头

    uint8_t    cmd_type;                    //命令类型(UPDATE_CONTROL)    
    uint8_t    ctrl_msg;                    //CtrlMsgType-指示消息的类型
		uint8_t    data;

    uint8_t  cmd_tail[4];                   //帧尾
	
}CTRL_MSG,*PCTRL_MSG;


#endif

