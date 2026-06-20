#include "hardware_interface.h"

#include "main.h"
#include "stm32g4xx_hal_tim.h"

void init_hardware(void)
{
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 500);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  HAL_OPAMP_Start(&hopamp1);
  HAL_OPAMP_Start(&hopamp2);
  HAL_OPAMP_Start(&hopamp3);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  
  HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
  
  uint8_t status;
  do{
	  if(HAL_I2C_Mem_Read(&hi2c3,0x8E,0x80,1,&status,1,HAL_MAX_DELAY) != HAL_OK) Error_Handler();
	  HAL_Delay(10);
  }while((status & 1U) != 0);

  uint8_t fault_clear = 0xFF;
  if(HAL_I2C_Mem_Write(&hi2c3,0x8E,0x09,1,&fault_clear,1,HAL_MAX_DELAY) != HAL_OK) Error_Handler();

  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	HAL_Delay(10);

  // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, TIM1_COUNT_MAX / 2);
  // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, TIM1_COUNT_MAX / 2);
  // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, TIM1_COUNT_MAX / 2);
  
}