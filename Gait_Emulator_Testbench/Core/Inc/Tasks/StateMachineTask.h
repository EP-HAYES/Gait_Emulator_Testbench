/*
 * StateMachineTask.h
 *
 *  Created on: Oct 4, 2025
 *      Author: EP HAYES
 */

#ifndef INC_TASKS_STATEMACHINETASK_H_
#define INC_TASKS_STATEMACHINETASK_H_

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "main.h"

/* Private define ------------------------------------------------------------*/

#define MACHINETASK_TIMEOUT	30

/* Private variables ---------------------------------------------------------*/

typedef enum
{
    Sleep,
    Boot,
    Idle,
    Test,
    Run,
    Err

}SystemStates;

typedef enum
{
    Motor,
    Controller,
    TargetMove,

}TestOptions;

typedef enum
{
    Home,
    Min,
    Start,
    Mid,
    Lift,
    Goal,
    Max

}Positions;

typedef struct __attribute__((packed, aligned(4)))		//Sent from UI leave test if
{
    bool TestStepNotLin;
    bool TestLegNotHip;
    Positions Target ;

}TestCheck;

typedef struct __attribute__((packed, aligned(4)))
{
    float Inclination;
    float Speed;

}TreadmillData;

typedef struct __attribute__((packed, aligned(4)))		//***NB *** Write function to reset flags from state to state, probably in idle :)
{
    bool HardwareEnabled;
    bool ActuatorsEnabled;
    bool SystemHomed;
    bool MachineTestGood;
    bool SensorsGood;
    bool ActuatorsGood;
    bool GoalReached;
    bool HipPositionSet;
    bool TestRunning;

}HardwareFlags;

typedef struct __attribute__((packed, aligned(4)))
{
    SystemStates State;
    TestOptions Test;
    HardwareFlags Flag;
    TestCheck Check;

}SystemData;

typedef struct __attribute__((packed, aligned(4)))
{
    bool RxQ_ERR;
    bool TxQ_ERR;
    bool Comm_ERR;
    bool Sens_ERR;
    bool Tim_ERR;

}TaskErrors;

typedef struct __attribute__((packed, aligned(4)))
{
    bool I2C_ERR;
    bool Null_ERR;
    bool Sens_ERR;
    bool Stop_ERR;

}MachineErrors;


typedef struct __attribute__((packed, aligned(4)))
{
    TaskErrors Tasks;
    MachineErrors Machine;

}Errors;



/* Private function prototypes -----------------------------------------------*/

void StateMachineTask_Reset(void);
void StateMachineTask_Init(void);
void StateMachineTask(void const * argument);
void GetTxData(TxDataPacket* Destination);
void BufferTxData(TxDataPacket* Destination);

/* End of file ---------------------------------------------------------------*/

#endif /* INC_TASKS_STATEMACHINETASK_H_ */
