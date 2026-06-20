#include "main_task.h"

#include "main.h"
#include "stdio.h"
#include "hardware_interface.h"
#include "system_interface.h"
#include "motor_control.h"

Motor_HandleTypeDef motor;

float MC_Get_Mechanical_Angle(Motor_HandleTypeDef *motor){
    return get_angle();
}

void main_task(void)
{
  init_hardware();
  init_system();
  
  motor.Init.pole_pares = 7;
  if(MC_Motor_Init(&motor) != MC_OK)Error_Handler();

  while (1)
  {
    // uint8_t status;
    // HAL_I2C_Mem_Read(&hi2c3,0x8E,0x80,1,&status,1,HAL_MAX_DELAY);
    // printf("[%4u, %4u, %4u, %4u]", adc1_buffer[0], adc1_buffer[1], adc1_buffer[2], adc1_buffer[3]);
    // printf("[%4u, %4u, %4u, %4u]", adc2_buffer[0], adc2_buffer[1], adc2_buffer[2], adc2_buffer[3]);
    // printf("[%4u, %4u, %4u] ", get_adc_latch(ADC_SHUNT1), get_adc_latch(ADC_SHUNT2), get_adc_latch(ADC_SHUNT3));
    // printf("%f, %f, %f, %02X, %f", get_vref(), get_temperature(), get_vbus() * 11.0f, status, MC_Get_Mechanical_Angle(&motor));
    printf("%f 0x%04X", MC_Get_Mechanical_Angle(&motor), get_angle_raw());
    printf("\r\n");
    delay(20);
  }
}
