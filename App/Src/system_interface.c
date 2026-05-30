#include "system_interface.h"
#include <math.h>

#define ADC1_BUFFER_SIZE 4
#define ADC2_BUFFER_SIZE 4
volatile uint16_t adc1_buffer[ADC1_BUFFER_SIZE];
volatile uint16_t adc2_buffer[ADC2_BUFFER_SIZE];

#define VREFINT_FACT_ADDR (0x1FFF75AA)
#define VREFINT_FACT_VOLTAGE (3.3f)

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

void init_system(void)
{
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_buffer, ADC1_BUFFER_SIZE);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_buffer, ADC2_BUFFER_SIZE);
}

uint16_t get_adc_buffer(uint8_t channel)
{
    switch (channel & ADC_INSTANCE_MASK) {
        case ADC1_MASK:
            return adc1_buffer[channel & ADC_INDEX_MASK];
        case ADC2_MASK:
            return adc2_buffer[channel & ADC_INDEX_MASK];
        default:
            return 0;
    }
}

float get_vref(void)
{
    const uint16_t vref_int = *((__IO uint16_t*)VREFINT_FACT_ADDR);
    return VREFINT_FACT_VOLTAGE * get_adc_buffer(ADC_VREFINT) / vref_int;
}

float get_temperature(void)
{
    return 1.0f/(1.0f/25.0f+logf((4.7f/(get_adc_buffer(ADC_TEMPERATURE) / 4096.0f) - 4.7f)/10.0f)/3400.0f);
}

float get_vbus(void)
{
    return get_adc_buffer(ADC_VBUS) / 4096.0f * get_vref();
}

int _write(int file, char *ptr, int len)
{
    return HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ptr, len) == HAL_OK ? len : 0;
}