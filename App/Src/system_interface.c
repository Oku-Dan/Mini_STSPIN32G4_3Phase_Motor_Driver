#include "system_interface.h"
#include "hardware_interface.h"
#include "main.h"
#include "math.h"

void init_system(void)
{
    init_hardware();
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
    return (float)__HAL_ADC_CALC_VREFANALOG_VOLTAGE(get_adc_latch(ADC_VREFINT), LL_ADC_RESOLUTION_12B) / 1000.0f;
}

float get_temperature(void)
{
    const uint16_t VREFANALOG = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(get_adc_latch(ADC_VREFINT), LL_ADC_RESOLUTION_12B);
    return __HAL_ADC_CALC_TEMPERATURE(VREFANALOG, get_adc_latch(ADC_TEMPERATURE), LL_ADC_RESOLUTION_12B);
}

float get_vbus(void)
{
    return get_adc_latch(ADC_VBUS) / (float)(0xFFF) * 11.0f * get_vref();
}

float get_angle_rad(void)
{
    return get_angle_14bit() / (float)(0x3FFF) * 2.0f * (float)(M_PI);
}

void delay(uint32_t ms)
{
    delay_ms(ms);
}

void set_periodic_callback(void (*func)(void)){
    timer_interrupt_callback = func;
}

void set_led(uint8_t state)
{
    set_led_gpio(state);
}

void set_voltage(float u, float v, float w){
    set_pwm_duty(TIM1_COUNT_MAX * u, TIM1_COUNT_MAX * v, TIM1_COUNT_MAX * w);
}