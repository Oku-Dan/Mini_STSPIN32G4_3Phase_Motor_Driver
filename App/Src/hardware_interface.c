#include "hardware_interface.h"
#include "main.h"
#include "string.h"

#define I2C_DRIVER_ADDR         0x8E

#define I2C_DRIVER_REG_POWMNG   0x01
#define I2C_DRIVER_REG_LOGIC    0x02
#define I2C_DRIVER_REG_READY    0x07
#define I2C_DRIVER_REG_NFAULT   0x08
#define I2C_DRIVER_REG_CLEAR    0x09
#define I2C_DRIVER_REG_STBY     0x0A
#define I2C_DRIVER_REG_LOCK     0x0B
#define I2C_DRIVER_REG_RESET    0x09
#define I2C_DRIVER_REG_STATUS   0x80

#define I2C_DRIVER_CLEAR_FAULT  0xFF
#define I2C_DRIVER_STATUS_LOCK  0x80
#define I2C_DRIVER_STATUS_RESET 0x08
#define I2C_DRIVER_STATUS_VDSP  0x04
#define I2C_DRIVER_STATUS_THSD  0x02
#define I2C_DRIVER_STATUS_UVLO  0x01

volatile uint16_t adc1_buffer[ADC1_BUFFER_SIZE];
volatile uint16_t adc2_buffer[ADC2_BUFFER_SIZE];
volatile uint16_t adc1_latch[ADC1_BUFFER_SIZE];
volatile uint16_t adc2_latch[ADC2_BUFFER_SIZE];

volatile uint16_t spi_tx_buff = 0x3FFF;
volatile uint16_t spi_rx_buff = 0x0000;

void (*timer_interrupt_callback)(void) = NULL;

void init_hardware(void)
{
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 500);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  HAL_OPAMP_Start(&hopamp1);
  HAL_OPAMP_Start(&hopamp2);
  HAL_OPAMP_Start(&hopamp3);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  
  HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
  
  uint8_t status;
  do{
	  if(HAL_I2C_Mem_Read(&hi2c3,I2C_DRIVER_ADDR,I2C_DRIVER_REG_STATUS,1,&status,1,HAL_MAX_DELAY) != HAL_OK) Error_Handler();
	  HAL_Delay(10);
  }while(status & I2C_DRIVER_STATUS_UVLO);

  uint8_t fault_clear = I2C_DRIVER_CLEAR_FAULT;
  if(HAL_I2C_Mem_Write(&hi2c3,I2C_DRIVER_ADDR,I2C_DRIVER_REG_CLEAR,1,&fault_clear,1,HAL_MAX_DELAY) != HAL_OK) Error_Handler();

  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

  HAL_Delay(10);

  adc1_latch[ADC1_BUFFER_SIZE - 1] = 0xFFFF;
  adc2_latch[ADC2_BUFFER_SIZE - 1] = 0xFFFF;

  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_buffer, ADC1_BUFFER_SIZE);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_buffer, ADC2_BUFFER_SIZE);

  while(adc1_latch[ADC1_BUFFER_SIZE - 1] == 0xFFFF || adc2_latch[ADC2_BUFFER_SIZE - 1] == 0xFFFF);

  HAL_TIM_Base_Start_IT(&htim1);
}

void delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}

void set_pwm_duty(uint16_t duty_u, uint16_t duty_v, uint16_t duty_w){
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty_u);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty_v);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty_w);
}

uint16_t get_angle_14bit(void)
{
    return spi_rx_buff & 0x3FFF;
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
        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1) {
        if (timer_interrupt_callback) {
            timer_interrupt_callback();
        }
        
        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t*)&spi_tx_buff, (uint8_t*)&spi_rx_buff, 1);
        
    }
}

int _write(int file, char *ptr, int len)
{
    return HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ptr, len) == HAL_OK ? len : 0;
}

void set_led_gpio(uint8_t state)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}