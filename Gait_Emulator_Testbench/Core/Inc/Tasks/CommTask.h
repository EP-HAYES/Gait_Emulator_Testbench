/*
 * CommTask.h
 *
 *  Created on: Oct 4, 2025
 *      Author: EP HAYES
 */

#ifndef INC_TASKS_COMMTASK_H_
#define INC_TASKS_COMMTASK_H_

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "main.h"

/* Private define ------------------------------------------------------------*/

#define WATCHDOG_TIMEOUT	14
#define COMMTASK_TIMEOUT	20
#define RX_RESEND		(char)"RX_RESEND"

typedef struct __attribute__((packed, aligned(4)))
{
    uint8_t Ping;								//Ping is state enum integer
    char Command[11];
    float InputData[2];
										//Received data parsing struct
}RxDataPacket;									//Total struct size 20 bytes

typedef struct __attribute__((packed, aligned(4)))
{
    uint8_t PingAck;
    char Condition[11];
    float SensorData[9];
    float ActuatorData[4];
										//Transmission data packaging struct
}TxDataPacket;									//Total struct size 64 bytes

typedef struct __attribute__((packed, aligned(4)))
{
    bool isAlive;
    bool PingRx;
    bool InternalCommError;
    TickType_t Freq;


}VcpConnectData;

extern osMessageQId CommRxHandle;
extern osMessageQId CommTxHandle;


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

void CommTask_Reset(void);
void CommTask_Init(void);
void CommTask(void const * argument);
void CommDisconnected(void const * argument);
const RxDataPacket* GetRxData(void);

/* End of file ---------------------------------------------------------------*/

#endif /* INC_TASKS_COMMTASK_H_ */
