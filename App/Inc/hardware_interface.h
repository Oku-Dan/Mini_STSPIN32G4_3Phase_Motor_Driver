#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include "stdint.h"

#define ADC1_BUFFER_SIZE 4
#define ADC2_BUFFER_SIZE 4

void init_hardware(void);
void delay_ms(uint32_t ms);
uint16_t get_angle_14bit(void);
void set_led_gpio(uint8_t state);
void set_pwm_duty(uint16_t duty_u, uint16_t duty_v, uint16_t duty_w);

extern volatile uint16_t adc1_buffer[ADC1_BUFFER_SIZE];
extern volatile uint16_t adc2_buffer[ADC2_BUFFER_SIZE];
extern volatile uint16_t adc1_latch[ADC1_BUFFER_SIZE];
extern volatile uint16_t adc2_latch[ADC2_BUFFER_SIZE];
extern volatile uint16_t spi_tx_buff;
extern volatile uint16_t spi_rx_buff;
extern void (*timer_interrupt_callback)(void);

#endif /* HARDWARE_INTERFACE_H */