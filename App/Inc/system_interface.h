#ifndef SYSTEM_INTERFACE_H
#define SYSTEM_INTERFACE_H

#include "main.h"

#define ADC1_MASK 0x10
#define ADC2_MASK 0x20
#define ADC3_MASK 0x30
#define ADC_RANK(X) (X - 1)
#define ADC_INDEX_MASK 0x0F
#define ADC_INSTANCE_MASK 0xF0

enum {
    ADC_SHUNT1      = ADC1_MASK | ADC_RANK(1),
    ADC_SHUNT3      = ADC1_MASK | ADC_RANK(2),
    ADC_TEMPERATURE = ADC1_MASK | ADC_RANK(3),
    ADC_VREFINT     = ADC1_MASK | ADC_RANK(4),
    ADC_SHUNT2      = ADC2_MASK | ADC_RANK(1),
    ADC_AIN1        = ADC2_MASK | ADC_RANK(2),
    ADC_AIN2        = ADC2_MASK | ADC_RANK(3),
    ADC_VBUS        = ADC2_MASK | ADC_RANK(4),
};

void delay(uint32_t);
void init_system(void);
uint16_t get_adc_buffer(uint8_t);
float get_vref(void);
float get_temperature(void);
float get_vbus(void);

#endif /* SYSTEM_INTERFACE_H */