/*
 * CommTask.c
 *
 *  Created on: Oct 4, 2025
 *      Author: EP HAYES
 */


/* Includes ------------------------------------------------------------------*/

#include  "CommTask.h"

/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static RxBuffer[sizeof(RxDataPacket)];
static RxDataPacket RxMsgRead;
static RxDataPacket RxMsgWrite;
static RxDataPacket TaskMsg;
static TxDataPacket TxMsg;
static VcpConnectData CommStats;

/* Private function prototypes -----------------------------------------------*/

// Reset all communication task variables
void CommTask_Reset(void)
    {
    memset(RxBuffer, 0, sizeof(RxBuffer));
    memset(RxMsgRead, 0, sizeof(RxMsgRead));
    memset(TxMsg, 0, sizeof(TxMsg));
    memset(RxMsgWrite, 0, sizeof(RxMsgWrite));
    memset(CommStats, 0, sizeof(CommStats));
    }

// Initialize all variables & peripherals required by the communication task and associated functions.
void CommTask_Init(void)
    {
    HAL_UART_Receive_DMA(&huart2, RxBuffer, Sizeof(RxDataPacket));
    CommTask_Reset();
//    ...
    }

// Parse UART data into message received struct and notify communication task.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
    {
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    memcpy(&RxMsgWrite, RxBuffer, sizeof(RxMsgWrite));				//Copy UART Received data buffer to data struct
    if (xTimerIsTimerActive(CommWatchdogHandle) == pdFALSE)
	{
	xTimerChangePeriodFromISR( CommWatchdogHandle,				//Start a watch dog timer to monitor the connection
				      pdMS_TO_TICKS(WATCHDOG_TIMEOUT),
				      pxHigherPriorityTaskWoken );
	CommStats.isAlive = true;						//Signal UART connection alive
	}
    else
	{
	CommStats.Freq = configTICK_RATE_HZ/xTimerGetPeriod(CommWatchdogHandle);
	xTimerResetFromISR(CommWatchdogHandle, pxHigherPriorityTaskWoken);	//Reset watch dog timer if connection persists
	CommStats.PingRx = true;						//Signal ping received to monitor connection speed
	}
    vTaskNotifyGiveFromISR( SpinVcpHandle, 					//Notify and unblock communication task
			    COMM_RX,
			    eSetBits,
			    pxHigherPriorityTaskWoken );
    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
    }

// TODO
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
    {
    //HAL_UART_Receive_DMA(huart, RxBuffer, Sizeof(RxDataPacket));
    CommStats.PingRx = false;
    }

// Main Communication Task for gait emulator test bench. Links PC UI to STM controller
void CommTask(void const * argument)
    {
    uint32_t Notification;
    for(;;)
	{
	xTaskNotifyWait( (uint32_t)0,						//Unblock communication task for notifications or no connection timeout
			    UINT32_MAX,
			    &Notification,
			    pdMS_TO_TICKS(COMMTASK_TIMEOUT) );
	if (Notification & COMM_RX)
	    {
	    if (BufferRxData(&RxMsgRead) == HAL_OK)				//Double buffer received data into shareable struct
		{
		if (eTaskGetState(StateMachineHandle) == eSuspended)
		    {
		    vTaskResume(StateMachineHandle);
		    }
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		xTaskNotify(StateMachineHandle, STATE_CHANGE, eSetBits);
		taskYIELD();
		}
	    else
		{
		Status.ErrCondition.Null_ERR = true;
		Error_Handler();
		}
	    }
	else if (Notification & COMM_TX)
	    {
	    if (GetTxData(&TxMsg) == HAL_OK)
		{
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&TxMsg, Sizeof(TxMsg));
		}
	    else
		{
		Status.ErrCondition.Null_ERR = true;
		Error_Handler();
		}
	    }
	else
	    {
	    osDelay(500);							//Wait for all tasks to detect failed communication
	    if (eTaskGetState(SampleSensorsHandle) == eSuspended) 		//Check that the state machine task has stopped the sampling task
		{
		vTaskSuspend(StateMachineHandle);				//Stop state machine task
		}
	    }

	}
    }

//Watch dog timer callback for UART communication timeout
void CommDisconnected(void const * argument)
    {
    CommTask_Reset();
//   TODO
    }


//Communication Task -> State Machine Task, memory safe data transfer function
void GetRxData(RxDataPacket* Destination)
    {
    memcpy(Destination, &RxMsgRead, sizeof(RxDataPacket));
    if (memcmp(Destination, &RxMsgRead, sizeof(RxDataPacket)) == 0)
	{
	return HAL_OK;
	}
    else
	{
	return HAL_ERROR;
	}
    }

//UART DMA transfer callback -> Communication Task, memory safe data transfer function
void BufferRxData(RxDataPacket* Destination)
    {
    memcpy(Destination, &RxMsgWrite, sizeof(RxDataPacket));
    if (memcmp(Destination, &RxMsgWrite, sizeof(RxDataPacket)) == 0)
	{
	return HAL_OK;
	}
    else
	{
	return HAL_ERROR;
	}
    }

/* End of file ---------------------------------------------------------------*/
