/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NC_Pin GPIO_PIN_2
#define NC_GPIO_Port GPIOE
#define NCE3_Pin GPIO_PIN_3
#define NCE3_GPIO_Port GPIOE
#define NCE4_Pin GPIO_PIN_4
#define NCE4_GPIO_Port GPIOE
#define NCE5_Pin GPIO_PIN_5
#define NCE5_GPIO_Port GPIOE
#define NCE6_Pin GPIO_PIN_6
#define NCE6_GPIO_Port GPIOE
#define NCF9_Pin GPIO_PIN_9
#define NCF9_GPIO_Port GPIOF
#define NCF10_Pin GPIO_PIN_10
#define NCF10_GPIO_Port GPIOF
#define NRST_Pin GPIO_PIN_10
#define NRST_GPIO_Port GPIOG
#define AIN1_Pin GPIO_PIN_0
#define AIN1_GPIO_Port GPIOC
#define VBUS_Pin GPIO_PIN_1
#define VBUS_GPIO_Port GPIOC
#define AIN2_Pin GPIO_PIN_2
#define AIN2_GPIO_Port GPIOC
#define NCF2_Pin GPIO_PIN_2
#define NCF2_GPIO_Port GPIOF
#define OPAMP1_VINP_Pin GPIO_PIN_1
#define OPAMP1_VINP_GPIO_Port GPIOA
#define OPAMP1_VOUT_Pin GPIO_PIN_2
#define OPAMP1_VOUT_GPIO_Port GPIOA
#define OPAMP1_VINM_Pin GPIO_PIN_3
#define OPAMP1_VINM_GPIO_Port GPIOA
#define WAKE_Pin GPIO_PIN_7
#define WAKE_GPIO_Port GPIOE
#define INL1_Pin GPIO_PIN_8
#define INL1_GPIO_Port GPIOE
#define INH1_Pin GPIO_PIN_9
#define INH1_GPIO_Port GPIOE
#define INL2_Pin GPIO_PIN_10
#define INL2_GPIO_Port GPIOE
#define INH2_Pin GPIO_PIN_11
#define INH2_GPIO_Port GPIOE
#define INL3_Pin GPIO_PIN_12
#define INL3_GPIO_Port GPIOE
#define INH3_Pin GPIO_PIN_13
#define INH3_GPIO_Port GPIOE
#define READY_Pin GPIO_PIN_14
#define READY_GPIO_Port GPIOE
#define NFAULT_Pin GPIO_PIN_15
#define NFAULT_GPIO_Port GPIOE
#define NCB11_Pin GPIO_PIN_11
#define NCB11_GPIO_Port GPIOB
#define NCB12_Pin GPIO_PIN_12
#define NCB12_GPIO_Port GPIOB
#define NCB13_Pin GPIO_PIN_13
#define NCB13_GPIO_Port GPIOB
#define NCB14_Pin GPIO_PIN_14
#define NCB14_GPIO_Port GPIOB
#define NCB15_Pin GPIO_PIN_15
#define NCB15_GPIO_Port GPIOB
#define NCD8_Pin GPIO_PIN_8
#define NCD8_GPIO_Port GPIOD
#define NCD9_Pin GPIO_PIN_9
#define NCD9_GPIO_Port GPIOD
#define NCD10_Pin GPIO_PIN_10
#define NCD10_GPIO_Port GPIOD
#define NCD11_Pin GPIO_PIN_11
#define NCD11_GPIO_Port GPIOD
#define NCD12_Pin GPIO_PIN_12
#define NCD12_GPIO_Port GPIOD
#define NCD13_Pin GPIO_PIN_13
#define NCD13_GPIO_Port GPIOD
#define NCD14_Pin GPIO_PIN_14
#define NCD14_GPIO_Port GPIOD
#define NCD15_Pin GPIO_PIN_15
#define NCD15_GPIO_Port GPIOD
#define NCC6_Pin GPIO_PIN_6
#define NCC6_GPIO_Port GPIOC
#define NCC7_Pin GPIO_PIN_7
#define NCC7_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define NSS_Pin GPIO_PIN_15
#define NSS_GPIO_Port GPIOA
#define NCC10_Pin GPIO_PIN_10
#define NCC10_GPIO_Port GPIOC
#define NCC11_Pin GPIO_PIN_11
#define NCC11_GPIO_Port GPIOC
#define NCC12_Pin GPIO_PIN_12
#define NCC12_GPIO_Port GPIOC
#define NCD0_Pin GPIO_PIN_0
#define NCD0_GPIO_Port GPIOD
#define NCD1_Pin GPIO_PIN_1
#define NCD1_GPIO_Port GPIOD
#define NCD3_Pin GPIO_PIN_3
#define NCD3_GPIO_Port GPIOD
#define NCD4_Pin GPIO_PIN_4
#define NCD4_GPIO_Port GPIOD
#define NCD5_Pin GPIO_PIN_5
#define NCD5_GPIO_Port GPIOD
#define NCD6_Pin GPIO_PIN_6
#define NCD6_GPIO_Port GPIOD
#define NCD7_Pin GPIO_PIN_7
#define NCD7_GPIO_Port GPIOD
#define SCK_X_Pin GPIO_PIN_3
#define SCK_X_GPIO_Port GPIOB
#define MISO_A_Pin GPIO_PIN_4
#define MISO_A_GPIO_Port GPIOB
#define MOSI_B_Pin GPIO_PIN_5
#define MOSI_B_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_6
#define LED_GPIO_Port GPIOB
#define NCE0_Pin GPIO_PIN_0
#define NCE0_GPIO_Port GPIOE
#define NCE1_Pin GPIO_PIN_1
#define NCE1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
