/*
 * SensorSamplingTask.c
 *
 *  Created on: Oct 4, 2025
 *      Author: EP HAYES
 */


/* Includes ------------------------------------------------------------------*/

#include "SensorSamplingTask.h"
#include "StateMachineTask.h"

/* Private define ------------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/

extern SystemData Status;

/* Private function prototypes -----------------------------------------------*/

void SensorSamplingTask_Reset(void)
    {
    memset(&SensorRead, 0, sizeof(SensorRead));
    }

void SensorSamplingTask_Init(void)
    {
    SensorSamplingTask_Reset();

    }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR( SampleSensorsHandle, 				//Notify and unblock sampling task
			    ENABLE_SAMPLING,
			    eSetBits,
			    pxHigherPriorityTaskWoken );
    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
    }

void SensorSamplingTask(void const * argument)
    {
    uint32_t Notification;
    for(;;)
	{
	xTaskNotifyWait( (uint32_t)0,						//Unblock sensor sampling task for notifications
				    UINT32_MAX,
				    &Notification,
				    pdMS_TO_TICKS(SAMPLINGTASK_TIMEOUT) );
		if (Notification & ENABLE_SAMPLING) 					//Data from UART notification
		    {
		    if (Status.State == Test)
			{

			}
		    else if (Status.State == Run)
			{

			}
		    }
		else
		    {
		    vTaskSuspend(NULL);
		    }
	}
    }

void GetSensorData(float* Data)
    {
    memcpy(Data, &SensorRead, sizeof(SensorRead));
    if (memcmp(Data, &SensorRead, sizeof(SensorRead)) == 0)
	{
	return HAL_OK;
	}
    else
	{
	return HAL_ERROR;
	}
    }
/* End of file ---------------------------------------------------------------*/
