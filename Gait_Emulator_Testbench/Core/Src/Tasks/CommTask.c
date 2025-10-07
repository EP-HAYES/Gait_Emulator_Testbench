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
static RxDataPacket RxMsg;
static RxDataPacket TaskBuffer;
static RxDataPacket TaskMsg;
static TxDataPacket TxMsg;
static VcpConnectData CommStats;

/* Private function prototypes -----------------------------------------------*/

// Reset all communication task variables
void CommTask_Reset(void)
    {
    memset(RxBuffer, 0, sizeof(RxBuffer));
    memset(RxMsg, 0, sizeof(RxMsg));
    memset(TxMsg, 0, sizeof(TxMsg));
    memset(TaskBuffer, 0, sizeof(TaskBuffer));
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
    memcpy(&RxMsg, RxBuffer, sizeof(RxMsg));					//Copy UART Received data buffer to data struct
    xQueueSendFromISR(CommRxHandle, &RxMsg, pxHigherPriorityTaskWoken);		//Push data struct to queue
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
	    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
	    if (xQueueReceive(CommRxHandle, &TaskBuffer, 0) == pdPASS)
		{
		memcpy(&TaskMsg, &TaskBuffer, sizeof(TaskBuffer));		//Buffer received data into shareable struct
		}
	    else
		{
		memset(TaskMsg, 0, sizeof(TaskMsg));
		CommStats.InternalCommError = true;
		}
	    xTaskNotify(StateMachineHandle, STATE_CHANGE, eSetBits);
	    taskYIELD();
	    }
	else if (Notification & COMM_TX)
	    {
	    if (CommStats.InternalCommError == true)
		{
		TxMsg.Condition = RX_RESEND;
		}
	    HAL_UART_Transmit_DMA(&huart2, TxMsg, Sizeof(TxMsg));
	    }
	else
	    {
	    osDelay(1000);
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
const RxDataPacket* GetRxData(void)
    {
    return &TaskMsg;
    }

/* End of file ---------------------------------------------------------------*/
