/*
 * SensorSamplingTask.h
 *
 *  Created on: Oct 4, 2025
 *      Author: EP HAYES
 */

#ifndef INC_TASKS_SENSORSAMPLINGTASK_H_
#define INC_TASKS_SENSORSAMPLINGTASK_H_

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "main.h"

/* Private define ------------------------------------------------------------*/

#define SAMPLINGTASK_TIMEOUT 2

/* Private variables ---------------------------------------------------------*/

static float SensorBuffer[9];
static float SensorRead[9];

/* Private function prototypes -----------------------------------------------*/

void SensorSamplingTask_Reset(void);
void SensorSamplingTask_Init(void);
void SensorSamplingTask(void const * argument);
void GetSensorData(float* Data);

/* End of file ---------------------------------------------------------------*/

#endif /* INC_TASKS_SENSORSAMPLINGTASK_H_ */
