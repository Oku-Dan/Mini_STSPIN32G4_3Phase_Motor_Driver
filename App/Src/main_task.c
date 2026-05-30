#include "main_task.h"

#include "main.h"
#include "stdio.h"
#include "hardware_interface.h"
#include "system_interface.h"
#define ADC1_BUFFER_SIZE 4
#define ADC2_BUFFER_SIZE 4
static volatile uint16_t adc1_buffer[ADC1_BUFFER_SIZE];
static volatile uint16_t adc2_buffer[ADC2_BUFFER_SIZE];

void main_task(void)
{
  init_hardware();
  init_system();

  while (1)
  {
    // printf("[%4u, %4u, %4u, %4u]", adc1_buffer[0], adc1_buffer[1], adc1_buffer[2], adc1_buffer[3]);
    // printf("[%4u, %4u, %4u, %4u]", adc2_buffer[0], adc2_buffer[1], adc2_buffer[2], adc2_buffer[3]);
    printf("[%4u, %4u, %4u] ", get_adc_buffer(ADC_SHUNT1), get_adc_buffer(ADC_SHUNT2), get_adc_buffer(ADC_SHUNT3));
    printf("%f, %f, %f", get_vref(), get_temperature(), get_vbus() * 11.0f);
    printf("\r\n");
    delay(100);
  }
}
