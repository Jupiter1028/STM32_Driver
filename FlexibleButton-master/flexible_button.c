/**
 * @File:    flexible_button.c
 * @Author:  MurphyZhao
 * @Date:    2018-09-29
 * 
*/

#include "flexible_button.h"
#include "String.h"
#include "usart.h"
#include "stdio.h"
#include "hal_oled.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "buzzer.h"
#include "cmsis_os.h"

#ifndef NULL
#define NULL 0
#endif

//定义
#define EVENT_SET_AND_EXEC_CB(btn, evt)                                        \
    do                                                                         \
    {                                                                          \
        btn->event = evt;                                                      \
        if(btn->cb)                                                            \
            btn->cb((flex_button_t*)btn);                                      \
    } while(0)

/**
 * 按键按下
 * 
 * 1: 按下
 * 0: 未按下
*/
#define BTN_IS_PRESSED(i) (g_btn_status_reg & (1 << i))

   
    
typedef uint32_t btn_type_t;

static flex_button_t *btn_head = NULL;
extern  uint16_t TEMP ;
extern osSemaphoreId myBinarySem01Handle;    
extern osSemaphoreId myCountingSem01Handle; 
extern osTimerId myTimer01Handle;
extern osTimerId myTimer02Handle;
/**
 * g_logic_level
 * 
 * 按下按钮的逻辑电平
 * 每个bit代表一个按钮
 * 
 * 第一个注册按钮，按下按钮的逻辑级别为g_logic_level的低位。
*/
btn_type_t g_logic_level = (btn_type_t)0;

/**
 * g_btn_status_reg
 * 
 * 所有按钮的状态寄存器，每一位记录按钮的按下状态。
 * 
 * 首次注册按钮，按钮的按下状态为在g_btn_status_reg的低位。
 * 
*/
btn_type_t g_btn_status_reg = (btn_type_t)0;

static uint8_t button_cnt = 0;

/**
 * @brief 注册一个用户按钮
 * 
 * @param按钮:按钮结构实例
 * @return已注册的键数，错误时为-1
*/
int32_t flex_button_register(flex_button_t *button)
{
    flex_button_t *curr = btn_head;
    
    if (!button || (button_cnt > sizeof(btn_type_t) * 8))
    {
        return -1;
    }

    while (curr)
    {
        if(curr == button)
        {
            return -1;  /* already exist. */
        }
        curr = curr->next;
    }

    /**
     * 第一个注册按钮在“链接列表”的末尾。
     * btn_head指向“链表”的头。
    */
    button->next = btn_head;
    button->status = FLEX_BTN_STAGE_DEFAULT;
    button->event = FLEX_BTN_PRESS_NONE;
    button->scan_cnt = 0;
    button->click_cnt = 0;
    button->max_multiple_clicks_interval = MAX_MULTIPLE_CLICKS_INTERVAL;
    btn_head = button;

    /**
     * First registered button, the logic level of the button pressed is *第一个注册按钮，按下按钮的逻辑级别为g_logic_level的低位。
     * at the low bit of g_logic_level.
    */
    g_logic_level |= (button->pressed_logic_level << button_cnt);
    button_cnt ++;

    return button_cnt;
}

/**
 * @brief  在一个扫描周期内读取所有键值
 * 
 * @param void
 * @return none
*/
static void flex_button_read(void)
{
    uint8_t i;
    flex_button_t* target;

    //第一个注册的按钮，按钮值位于raw_data */的低位 
    btn_type_t raw_data = 0;

    for(target = btn_head, i = button_cnt - 1;
        (target != NULL) && (target->usr_button_read != NULL);
        target = target->next, i--)
    {
        raw_data = raw_data | ((target->usr_button_read)(target) << i);
    }

    g_btn_status_reg = (~raw_data) ^ g_logic_level;
}

/**
 * @brief 处理所有关键事件在一个扫描周期。 必须在'flex_button_read' API之后使用
 * 
 * @param void
 * @return激活按钮计数
 *
*/
static uint8_t flex_button_process(void)
{
    uint8_t i;
    uint8_t active_btn_cnt = 0;
    flex_button_t* target;    //按键事件
    
    for (target = btn_head, i = button_cnt - 1; target != NULL; target = target->next, i--)
    {
      if (target->status > FLEX_BTN_STAGE_DEFAULT)    //如果按键的状态的状态为默认，默认:0;点击：1；双击：2； 多击：3
        {
            target->scan_cnt ++;                      //记录扫描次数，按键按下是开始从零计数
            if (target->scan_cnt >= ((1 << (sizeof(target->scan_cnt) * 8)) - 1))
            {
                target->scan_cnt = target->long_hold_start_tick;  //长按保持事件触发的起始 tick
            }
        }

        switch (target->status)       //用于记录当前按键的状态，用于内部状态机
        {
        case FLEX_BTN_STAGE_DEFAULT: /* 如果按键的状态的状态为默认，默认:0;点击：1；双击：2； 多击：3 */
            if (BTN_IS_PRESSED(i)) /* 按键按下 */
            {
                target->scan_cnt = 0;    
                target->click_cnt = 0;

                EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_DOWN);

                /* swtich to button down stage */
                target->status = FLEX_BTN_STAGE_DOWN;
            }
            else
            {
                target->event = FLEX_BTN_PRESS_NONE;
            }
            break;

        case FLEX_BTN_STAGE_DOWN: /* stage: button down */
            if (BTN_IS_PRESSED(i)) /* is pressed */
            {
                if (target->click_cnt > 0) /* multiple click */
                {
                    if (target->scan_cnt > target->max_multiple_clicks_interval)
                    {
                        EVENT_SET_AND_EXEC_CB(target, 
                            target->click_cnt < FLEX_BTN_PRESS_REPEAT_CLICK ? 
                                target->click_cnt :
                                FLEX_BTN_PRESS_REPEAT_CLICK);

                        /* swtich to button down stage */
                        target->status = FLEX_BTN_STAGE_DOWN;
                        target->scan_cnt = 0;
                        target->click_cnt = 0;
                    }
                }
                else if (target->scan_cnt >= target->long_hold_start_tick)
                {
                    if (target->event != FLEX_BTN_PRESS_LONG_HOLD)
                    {
                        EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_HOLD);
                    }
                }
                else if (target->scan_cnt >= target->long_press_start_tick)
                {
                    if (target->event != FLEX_BTN_PRESS_LONG_START)
                    {
                        EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_START);
                    }
                }
                else if (target->scan_cnt >= target->short_press_start_tick)
                {
                    if (target->event != FLEX_BTN_PRESS_SHORT_START)
                    {
                        EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_SHORT_START);
                    }
                }
            }
            else /* button up */
            {
                if (target->scan_cnt >= target->long_hold_start_tick)
                {
                    EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_HOLD_UP);
                    target->status = FLEX_BTN_STAGE_DEFAULT;
                }
                else if (target->scan_cnt >= target->long_press_start_tick)
                {
                    EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_UP);
                    target->status = FLEX_BTN_STAGE_DEFAULT;
                }
                else if (target->scan_cnt >= target->short_press_start_tick)
                {
                    EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_SHORT_UP);
                    target->status = FLEX_BTN_STAGE_DEFAULT;
                }
                else
                {
                    /* swtich to multiple click stage */
                    target->status = FLEX_BTN_STAGE_MULTIPLE_CLICK;
                    target->click_cnt ++;
                }
            }
            break;

        case FLEX_BTN_STAGE_MULTIPLE_CLICK: /* stage: multiple click */
            if (BTN_IS_PRESSED(i)) /* is pressed */
            {
                /* swtich to button down stage */
                target->status = FLEX_BTN_STAGE_DOWN;
                target->scan_cnt = 0;
            }
            else
            {
                if (target->scan_cnt > target->max_multiple_clicks_interval)
                {
                    EVENT_SET_AND_EXEC_CB(target, 
                        target->click_cnt < FLEX_BTN_PRESS_REPEAT_CLICK ? 
                            target->click_cnt :
                            FLEX_BTN_PRESS_REPEAT_CLICK);

                    /* swtich to default stage */
                    target->status = FLEX_BTN_STAGE_DEFAULT;
                }
            }
            break;
        }
        
        if (target->status > FLEX_BTN_STAGE_DEFAULT)
        {
            active_btn_cnt ++;
        }
    }
    
    return active_btn_cnt;
}

/**
 * flex_button_event_read
 * 
 * @brief获取指定按钮的按钮事件。
 * 
 * @param 按钮:按钮结构实例
 * @return 返回按钮事件      
*/
flex_button_event_t flex_button_event_read(flex_button_t* button)
{
    return (flex_button_event_t)(button->event);
}

/**
 * flex_button_scan
 * 
 * @brief启动键扫描。
 * 需要在指定时间内周期性调用。采样周期:5 - 20ms
 * @param void
 * @return Activated button count
*/
uint8_t flex_button_scan(void)
{
    flex_button_read();
    return flex_button_process();
}


//事件处理代码
//static void btn_cb(flex_button_t *btn)
static void btn_cb(void *arg)
{
  flex_button_t *btn = (flex_button_t *)arg;
  switch(btn->id){
    case 0:
//       printf("btn_0_cb\n");
        switch (btn->event)
        {
            case FLEX_BTN_PRESS_DOWN:
//                printf("btn_0 [按下]\n");
                if(OLED_SHOW){OLED_ShowString(4,1,"Btn0 down");}
                break;
            case FLEX_BTN_PRESS_CLICK:
                printf("btn_0 [点击事件]\n");
                if(OLED_SHOW){OLED_ShowString(4,1,"Btn0 click");}
                TEMP ++ ;
                //获取计数型信号量
                if(xSemaphoreTake(myCountingSem01Handle,10) == pdPASS ){
                    printf("获取计数信号量成功\n");
                  }else{
                  printf("获取计数信号量失败\n");
                }
                break;
            case FLEX_BTN_PRESS_DOUBLE_CLICK:
                printf("btn_0 [双击事件]\n");
                if(OLED_SHOW){OLED_ShowString(4,1,"Btn0 double click");}
                TEMP += 2 ;
                break;
            case FLEX_BTN_PRESS_SHORT_START:
                printf("btn_0 [短按开始事件]\n");
                if(OLED_SHOW){OLED_ShowString(4,1,"Btn0 press short");}
                TEMP += 5 ;
                break;
            case FLEX_BTN_PRESS_SHORT_UP:
                printf("btn_0 [短按抬起事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_START:
                printf("btn_0 [长按开始事件]\n");
                
                if(OLED_SHOW){OLED_ShowString(4,1,"Btn0 press long");}
                break;
            case FLEX_BTN_PRESS_LONG_UP:
                printf("btn_0 [长按抬起事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_HOLD:
                printf("btn_0 [长按保持事件]\n");
                
                break;
            case FLEX_BTN_PRESS_LONG_HOLD_UP:
                printf("btn_0 [长按保持抬起事件]\n");
                
                //释放二值信号量
                if( xSemaphoreGive(myBinarySem01Handle) != pdTRUE ){    //当释放信号量，信号量计数值（资源数）加一
                  printf("释放二值信号量成功\n");
                }else{
                  printf("释放二值信号量失败\n");
                }
                break;
            case FLEX_BTN_PRESS_REPEAT_CLICK:
                TEMP += (btn->click_cnt);
                printf("btn_0 [连击%d次]\n",btn->click_cnt);
                break;
        }
        break;
    case 1:
//      printf("btn_1_cb\n");
        switch (btn->event)
        {
            case FLEX_BTN_PRESS_DOWN:
//                printf("btn_1 [按下]\n");
                break;
            case FLEX_BTN_PRESS_CLICK:
                printf("btn_1 [点击事件]\n");
                TEMP -- ;
                //释放计数型信号量
                if(xSemaphoreGive(myCountingSem01Handle) == pdPASS){
                    printf("释放计数信号量成功\n");
                  }else{
                  printf("释放计数信号量失败\n");
                }
                break;
            case FLEX_BTN_PRESS_DOUBLE_CLICK:
                printf("btn_1 [双击事件]\n");
                TEMP -= 2 ;
                break;
            case FLEX_BTN_PRESS_SHORT_START:
                printf("btn_1 [短按开始事件]\n");
                TEMP -= 5 ;
                break;
            case FLEX_BTN_PRESS_SHORT_UP:
                printf("btn_1 [短按抬起事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_START:
                printf("btn_1 [长按开始事件]\n");
                TEMP -= 10 ;
                break;
            case FLEX_BTN_PRESS_LONG_UP:
                printf("btn_1 [长按抬起事件]\n");

                break;
            case FLEX_BTN_PRESS_LONG_HOLD:
                printf("btn_1 [长按保持事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_HOLD_UP:
                printf("btn_1 [长按保持抬起事件]\n");
                //释放二值信号量
                if( xSemaphoreTake(myBinarySem01Handle,0) != pdTRUE ){
                  printf("获取计数信号量成功\n");
                }else{
                  printf("获取计数信号量失败\n");
                }
                break;
            case FLEX_BTN_PRESS_REPEAT_CLICK:
                TEMP -= (btn->click_cnt);
                printf("btn_1 [连击%d次]\n",btn->click_cnt);
                break;
        }
        break;
  case 2:
//      printf("btn_2_cb\n");
        switch (btn->event)
        {
            case FLEX_BTN_PRESS_DOWN:
//                printf("btn_2 [按下]\n");
                
                break;
            case FLEX_BTN_PRESS_CLICK:
                printf("btn_2 [点击事件]\n");
                break;
            case FLEX_BTN_PRESS_DOUBLE_CLICK:
                printf("btn_2 [双击事件]\n");
                break;
            case FLEX_BTN_PRESS_SHORT_START:
                printf("btn_2 [短按开始事件]\n");
                break;
            case FLEX_BTN_PRESS_SHORT_UP:
                printf("btn_2 [短按抬起事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_START:
                printf("btn_2 [长按开始事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_UP:
                printf("btn_2 [长按抬起事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_HOLD:
                printf("btn_2 [长按保持事件]\n");
                break;
            case FLEX_BTN_PRESS_LONG_HOLD_UP:
                printf("btn_2 [长按保持抬起事件]\n");
                break;
        }
  }

}



//static void user_button_init(void)
void user_button_init(void)
{
    int i;
    
    memset(&user_button[0], 0x0, sizeof(user_button));

    for (i = 0; i < USER_BUTTON_MAX; i++)
    {
        user_button[i].id = i;   // 按键的ID号
        user_button[i].usr_button_read = common_btn_read; // 按键引脚电平读取函数
        user_button[i].cb = btn_cb;   // 事件回调函数
        user_button[i].pressed_logic_level = 0;  // 设置按键按下的逻辑电平
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500); // 设置短按事件触发的起始 tick
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(3000);  // 设置长按事件触发的起始 tick
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);   // 设置长按保持事件触发的起始 tick

        flex_button_register(&user_button[i]);   // 按键注册
    }
}

