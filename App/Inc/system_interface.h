#ifndef SYSTEM_INTERFACE_H
#define SYSTEM_INTERFACE_H

#include <stdint.h>
#include "main.h"

#define ADC1_MASK 0x10
#define ADC2_MASK 0x20
#define ADC3_MASK 0x30
#define ADC_RANK(X) (X - 1)
#define ADC_INDEX_MASK 0x0F
#define ADC_INSTANCE_MASK 0xF0

typedef enum {
    ADC_SHUNT1      = ADC1_MASK | ADC_RANK(1),
    ADC_SHUNT3      = ADC1_MASK | ADC_RANK(2),
    ADC_TEMPERATURE = ADC1_MASK | ADC_RANK(3),
    ADC_VREFINT     = ADC1_MASK | ADC_RANK(4),
    ADC_SHUNT2      = ADC2_MASK | ADC_RANK(1),
    ADC_AIN1        = ADC2_MASK | ADC_RANK(2),
    ADC_AIN2        = ADC2_MASK | ADC_RANK(3),
    ADC_VBUS        = ADC2_MASK | ADC_RANK(4),
} ADC_CHANNEL;

void init_system(void);
void delay(uint32_t);
uint16_t get_adc_buffer(ADC_CHANNEL);
uint16_t get_adc_latch(ADC_CHANNEL);
float get_vref(void);
float get_temperature(void);
float get_vbus(void);
float get_angle_rad(void);
void set_periodic_callback(void (*func)(void));
void set_led(uint8_t state);
void set_voltage(float u, float v, float w);

#endif /* SYSTEM_INTERFACE_H */