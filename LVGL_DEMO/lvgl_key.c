#include "lvgl_key.h"

//个人修改的部分代码
#define USER_BUTTON_PORT GPIOB
#define USER_BUTTON_0_PIN GPIO_PIN_12		//上 
#define USER_BUTTON_1_PIN GPIO_PIN_13		//下 
#define USER_BUTTON_2_PIN GPIO_PIN_14		//左
#define USER_BUTTON_3_PIN GPIO_PIN_15		//右

#define Read_Key0_State()   HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_0_PIN)
#define Read_Key1_State() 	HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_1_PIN)
#define Read_Key2_State()   HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_2_PIN)
#define Read_Key3_State() 	HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_3_PIN)

void MyKEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = UP_Pin|DOWN_Pin|LEFT_Pin|RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#include "usart.h"
uint32_t MyKEY_Read(void){
  uint32_t key_value = 0;
 
  if(Read_Key0_State() == 0){
    HAL_Delay(10);
    key_value = 1 ;
  }else
   if(Read_Key1_State() == 0){
    HAL_Delay(10);
    key_value = 2;
  }else
    if(Read_Key2_State() == 0){
    HAL_Delay(10);
    key_value = 3;
  }else
    if(Read_Key3_State() == 0){
    HAL_Delay(10);
    key_value = 4;
  }
  return key_value;
}

