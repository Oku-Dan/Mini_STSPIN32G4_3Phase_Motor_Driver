#include "main_task.h"

#include "main.h"
#include "stdio.h"
#include "system_interface.h"
#include "motor.h"
#include "typedef.h"

void update_motor_controller(void);

static Motor_HandleTypeDef motor_handle = {
    .object = &(Motor_InitTypeDef){
        .pole_pares = 7,
        .angle_offset = 3.7288f,
        .parameter = &(Motor_CharactaeristicTypeDef){
            .R = 0.5f,
            .Lq = 0.001f,
            .Ld = 0.001f,
            .Ke = 0.1f,
        },
    },
    .controller = &(Motor_ControllerTypeDef){
        .dt = 0.0001f, // 10 kHz control loop
    },
};

static volatile Voltage_TypeDef output_voltage;

void main_task(void)
{
  MotorInit(&motor_handle);

  set_periodic_callback(update_motor_controller);
  init_system();

  while (1)
  {
    static uint8_t led_state = 0;
    led_state = !led_state;
    set_led(led_state);
    
    printf("Vref: %.2f V, Vbus: %.2f V, Temp: %.2f C, Angle: %.2f rad\n", get_vref(), get_vbus(), get_temperature(), get_angle_rad());
    // printf("%f %f %f Angle: %f rad\n", output_voltage.u, output_voltage.v, output_voltage.w, get_angle_rad());
    delay(100);

    if(HAL_GetTick() > 10000) {
      if(HAL_GetTick() < 11000) {
        // MotorSetVoltage(&motor_handle, 0.0f, (11000 - HAL_GetTick()) / 1000.0f * 0.3f);
      }else {
        MotorSetStop(&motor_handle);
      }
    }
  }
}

void update_motor_controller(void)
{
  float mech_angle = get_angle_rad();
  float mech_speed = 0.0f; // Placeholder for speed calculation
  float current_u = get_adc_latch(ADC_SHUNT1) / 4096.0f * get_vref() / 0.001f; // Assuming 1 mOhm shunt resistor
  float current_v = get_adc_latch(ADC_SHUNT2) / 4096.0f * get_vref() / 0.001f; // Assuming 1 mOhm shunt resistor
  float current_w = get_adc_latch(ADC_SHUNT3) / 4096.0f * get_vref() / 0.001f; // Assuming 1 mOhm shunt resistor

  output_voltage = MotorUpdateController(&motor_handle, mech_angle, mech_speed, current_u, current_v, current_w);

  set_voltage(output_voltage.u, output_voltage.v, output_voltage.w);
}