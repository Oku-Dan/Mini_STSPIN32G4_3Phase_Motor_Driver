#include "system_interface.h"
#include "main.h"
#include "stm32g431xx.h"
#include "stm32g4xx_hal_spi.h"
#include <math.h>
#include <string.h>

#define ADC1_BUFFER_SIZE 4
#define ADC2_BUFFER_SIZE 4
volatile uint16_t adc1_buffer[ADC1_BUFFER_SIZE];
volatile uint16_t adc2_buffer[ADC2_BUFFER_SIZE];
volatile uint16_t adc1_latch[ADC1_BUFFER_SIZE];
volatile uint16_t adc2_latch[ADC2_BUFFER_SIZE];

const static uint16_t spi_tx_buff = 0x3FFF;
volatile static uint16_t spi_rx_buff = 0x0000;
volatile static uint16_t angle_raw = 0;

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

uint16_t get_adc_buffer(ADC_CHANNEL channel)
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

uint16_t get_adc_latch(ADC_CHANNEL channel)
{
    switch (channel & ADC_INSTANCE_MASK) {
        case ADC1_MASK:
            return adc1_latch[channel & ADC_INDEX_MASK];
        case ADC2_MASK:
            return adc2_latch[channel & ADC_INDEX_MASK];
        default:
            return 0;
    }
}

float get_vref(void)
{
    const uint16_t vref_int = *((__IO uint16_t*)VREFINT_FACT_ADDR);
    return VREFINT_FACT_VOLTAGE * get_adc_latch(ADC_VREFINT) / vref_int;
}

float get_temperature(void)
{
    return 1.0f/(1.0f/25.0f+logf((4.7f/(get_adc_latch(ADC_TEMPERATURE) / 4096.0f) - 4.7f)/10.0f)/3400.0f);
}

float get_vbus(void)
{
    return get_adc_latch(ADC_VBUS) / 4096.0f * get_vref();
}

int _write(int file, char *ptr, int len)
{
    return HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ptr, len) == HAL_OK ? len : 0;
}

uint16_t get_angle_raw(void)
{
    return angle_raw;
}

float get_angle(void)
{
    return get_angle_raw() / (float)(0x3FFF) * 2.0f * (float)(M_PI);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1) {
        static uint16_t val = 0;
        float t = val / (float)0xFFFF * 2.0f * M_PI;
        val += 64;

        // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, TIM1_COUNT_MAX / 2 + 250 * sinf(t));
        // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, TIM1_COUNT_MAX / 2 + 250 * sinf(t + 2.0f * M_PI / 3.0f));
        // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, TIM1_COUNT_MAX / 2 + 250 * sinf(t + 4.0f * M_PI / 3.0f));

        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t*)&spi_tx_buff, (uint8_t*)&spi_rx_buff, 1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc == &hadc1) {
        memcpy((void*)adc1_latch, (void*)adc1_buffer, sizeof(adc1_latch));
    } else if (hadc == &hadc2) {
        memcpy((void*)adc2_latch, (void*)adc2_buffer, sizeof(adc2_latch));
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi == &hspi1) {
        angle_raw = spi_rx_buff & 0x3FFF;
        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
    }
}
