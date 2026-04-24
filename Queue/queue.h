#ifndef _QUEUE_H
#define _QUEUE_H

#include "main.h"

typedef uint8_t qdata;   
typedef uint8_t qsize;

#define QUEUE_MAX_SIZE 128            // 指令接收缓冲区大小，根据需要调整，尽量设置大一些

#define CMD_HEAD 0XEE                                                  //帧头
#define CMD_TAIL 0XFFFCFFFF                                            //帧尾


typedef struct _QUEUE                                             
{                                                                 
    qsize _head;                                                       //队列头
    qsize _tail;                                                       //队列尾
    qdata _data[QUEUE_MAX_SIZE];                                       //队列数据缓存区
}QUEUE;                                                           

//消息结构体，可以添加内容
typedef struct
{
    uint8_t    cmd_head;                    //帧头

    uint8_t    cmd_type;                    //命令类型
    uint8_t    data1;                       //数据1
		uint8_t    data2;                       //数据2
    uint8_t    data3;                       //数据3
		uint8_t    data4;                       //数据4
  	uint8_t    data5;                       //数据5
    uint8_t    data6;                       //数据5

    uint8_t    cmd_tail[4];                   //帧尾
	
}CTRL_MSG,*PCTRL_MSG;


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
//从队列中取一个数据
extern void queue_pop(qdata* _data);

//获取队列中有效数据个数
extern qsize queue_size(void);

//取一条命令出来
extern qsize queue_find_cmd(qdata *cmd,qsize buf_len);


#endif

