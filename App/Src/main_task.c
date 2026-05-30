#include "main_task.h"

#include "main.h"
#include "stdio.h"

#define ADC1_BUFFER_SIZE 4
#define ADC2_BUFFER_SIZE 4
static volatile uint16_t adc1_buffer[ADC1_BUFFER_SIZE];
static volatile uint16_t adc2_buffer[ADC2_BUFFER_SIZE];

void main_task(void)
{
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 500);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  HAL_OPAMP_Start(&hopamp1);
  HAL_OPAMP_Start(&hopamp2);
  HAL_OPAMP_Start(&hopamp3);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);

  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_buffer, ADC1_BUFFER_SIZE);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_buffer, ADC2_BUFFER_SIZE);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3);

  while (1)
  {
    printf("[%4u, %4u, %4u, %4u]", adc1_buffer[0], adc1_buffer[1], adc1_buffer[2], adc1_buffer[3]);
    printf("[%4u, %4u, %4u, %4u]", adc2_buffer[0], adc2_buffer[1], adc2_buffer[2], adc2_buffer[3]);
    printf("\r\n");
    HAL_Delay(100);
  }
}

int _write(int file, char *ptr, int len)
{
    return HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ptr, len) == HAL_OK ? len : 0;
}