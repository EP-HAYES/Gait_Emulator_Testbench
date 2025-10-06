/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define COMM_TX (1 << 2)
#define COMM_RX (1 << 1)
#define SENSORS_SAMPLED (1 << 0)
#define STATE_CHANGE (1 << 3)
#define DATA_READY (1 << 4)
#define ENABLE_SAMPLING (1 << 5)
#define EM_STOP_Pin GPIO_PIN_13
#define EM_STOP_GPIO_Port GPIOC
#define EM_STOP_EXTI_IRQn EXTI15_10_IRQn
#define STEP_EN_Pin GPIO_PIN_15
#define STEP_EN_GPIO_Port GPIOC
#define STG_DATA_Pin GPIO_PIN_2
#define STG_DATA_GPIO_Port GPIOC
#define LIMIT_Pin GPIO_PIN_3
#define LIMIT_GPIO_Port GPIOC
#define LIMIT_EXTI_IRQn EXTI3_IRQn
#define HIGHSIDE1_Pin GPIO_PIN_0
#define HIGHSIDE1_GPIO_Port GPIOA
#define HIGHSIDE2_Pin GPIO_PIN_1
#define HIGHSIDE2_GPIO_Port GPIOA
#define LED_PIN_Pin GPIO_PIN_5
#define LED_PIN_GPIO_Port GPIOA
#define AMPS_Pin GPIO_PIN_6
#define AMPS_GPIO_Port GPIOA
#define POT_Pin GPIO_PIN_7
#define POT_GPIO_Port GPIOA
#define LOWSIDE2_Pin GPIO_PIN_4
#define LOWSIDE2_GPIO_Port GPIOC
#define STG_SCL_Pin GPIO_PIN_0
#define STG_SCL_GPIO_Port GPIOB
#define STEP_ENCA_Pin GPIO_PIN_1
#define STEP_ENCA_GPIO_Port GPIOB
#define SPD_INC_Pin GPIO_PIN_12
#define SPD_INC_GPIO_Port GPIOB
#define LOWSIDE1_Pin GPIO_PIN_14
#define LOWSIDE1_GPIO_Port GPIOB
#define STEP_ENCB_Pin GPIO_PIN_6
#define STEP_ENCB_GPIO_Port GPIOC
#define SPD_ENC_Pin GPIO_PIN_9
#define SPD_ENC_GPIO_Port GPIOC
#define INC_PLUS_Pin GPIO_PIN_11
#define INC_PLUS_GPIO_Port GPIOA
#define INC_MINUS_Pin GPIO_PIN_12
#define INC_MINUS_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SPD_MINUS_Pin GPIO_PIN_6
#define SPD_MINUS_GPIO_Port GPIOB
#define STEP_PULSE_Pin GPIO_PIN_7
#define STEP_PULSE_GPIO_Port GPIOB
#define IMU_SCL_Pin GPIO_PIN_8
#define IMU_SCL_GPIO_Port GPIOB
#define IMU_SDA_Pin GPIO_PIN_9
#define IMU_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
