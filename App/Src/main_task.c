#include "main_task.h"

void main_task(void)
{
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 500);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  while (1)
  {
    HAL_Delay(500);
  }
}