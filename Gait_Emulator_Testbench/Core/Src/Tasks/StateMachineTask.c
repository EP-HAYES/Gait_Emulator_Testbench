/*
 * StateMachineTask.c
 *
 *  Created on: Oct 8, 2025
 *      Author: EP HAYES
 */


/* Includes ------------------------------------------------------------------*/

#include "StateMachineTask.h"
#include "CommTask.h"

/* Private define ------------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/

static TxDataPacket TxMsgWrite;
static TxDataPacket TxMsgRead;
static RxDataPacket CommData;
SystemData Status;
extern Errors Error;

/* Private function prototypes -----------------------------------------------*/

void StateMachineTask_Init(void)
    {
    memset(MachineData, 0, sizeof(MachineData));
    memset(TxBuffer, 0, sizeof(TxBuffer));
    memset(TaskData, 0, sizeof(TaskData));
    memset(Status, 0, sizeof(Status));						//Resets State Machine Error conditions and State variable
    }

void StateMachineTask(void const * argument)
    {
    uint32_t Notification;
    static float TestData[INPUT_DATA_LEN];
    static Acutuators Part;
    static TestCheck Position;
    for(;;)
	{
	xTaskNotifyWait( (uint32_t)0,						//Unblock state machine task for notifications
			    UINT32_MAX,
			    &Notification,
			    pdMS_TO_TICKS(MACHINETASK_TIMEOUT) );
	if (Notification & STATE_CHANGE) 					//Data from UART notification
	    {
	    if ((GetRxData(&CommData)) == HAL_OK)				//Check for failed data reception
		{
		ChangeState(CommData.Ping);
		if (CommData.Command[0] != '\0')				//Check for command
		    {
		    ProcessCommand(&CommData.Command, &TxMsgWrite.Condition);
		    }
		}
	    else
		{
		Error.Tasks.Comm_ERR = true;
		Error_Handler();
		}
	    }
	else if (Notification & SENSORS_SAMPLED) 				//Data from Sampling notification
	    {
	    if (GetSensorData(&TxMsgWrite.SensorData) != true)
		{
		Error.Tasks.Sens_ERR = true;
		Error_Handler();
		}
	    CheckSoftwareLimits();
	    }
	else
	    {
	    vTaskSuspend(SampleSensorsHandle);
	    ChangeState(Sleep);
	    }
	switch (Status.State)
	    {
	    case Sleep:
		if (Status.Flag.HardwareEnabled  == true)
		    {
		    DisableActuator(Act_All) ? (DisableSensor(Sens_All) && Status.Flag.HardwareEnabled == false) : Error_Handler();
		    }
		break;
	    case Boot:
		Status.Flag.HardwareEnabled ? (void)0 : (EnableSensor(Sens_All) && EnableActuator(Act_All) && Status.Flag.HardwareEnabled == true);
		if (Status.Flag.MachineTestGood == false)
		    {
		    Status.Flag.SensorsGood ? (Status.Flag.ActuatorsGood ? Status.Flag.MachineTestGood == true : ActuatorTest()) : SensorCheck();
		    }
		else
		    {
		    JogActuator(Act_All, Home) ? (PositionReached(Act_All, Home) && ChangeState(Idle)) : PositionMonitor();
		    }
		break;
	    case Idle:
		Status.Flag.ActuatorsEnabled ? (DisableActuator(Act_All) ? Status.Flag.ActuatorsEnabled == false : Error_Handler()) : PositionMonitor();
		break;
	    case Test:
		switch (Status.Test)
		    {
		    case Motor:
			if (Status.Flag.ActuatorsEnabled  == false)
			    {
			    Status.Check.TestStepNotLin ? Part = Step : Part = Lin;
			    EnableActuator(Part);
			    Status.Flag.ActuatorsEnabled == true;
			    }
			FeedInterpolator(&CommData.InputData, &TestData);
			InterpolatorReady(Part) ? (JogActuator(Part, &TestData) && PositionMonitor()) : (void)0;
			break;
		    case Controller:
			if (Status.Flag.ActuatorsEnabled  == false)
			    {
			    Status.Check.TestStepNotLin ? Part = Step : Part = Lin;
			    EnableActuator(Part);
			    Status.Flag.ActuatorsEnabled == true;
			    }
			FeedInterpolator(&CommData.InputData, &TestData);
			InterpolatorReady(Part) ? (PartController(Part, &TestData) && PositionMonitor()) : (void)0;
			break;
		    case TargetMove:
			if (Status.Flag.ActuatorsEnabled  == false)
			    {
			    Status.Check.TestStepNotLin ? Part = Step : Part = Lin;
			    EnableActuator(Part);
			    Position = GetTarget();
			    Status.Flag.ActuatorsEnabled == true;
			    }
			JogActuator(Part, Position) ? PositionReached(Part, Position) : PositionMonitor();
			break;
		    }
		break;
	    case Run:
		if (Status.Flag.ActuatorsEnabled  == false)
		    {
		    EnableActuator(Act_All);
		    Status.Flag.ActuatorsEnabled == true;
		    }
		if (Status.Flag.SystemHomed == false)
		    {
		    (Status.Check.Target == Home) ? Status.Flag.SystemHomed == true : (JogActuator(Act_All, Home) ? PositionReached(Act_All, Home) : PositionMonitor());
		    }
		else if (Status.Flag.GoalReached == false)
		    {
		    if (Status.Flag.HipPositionSet == false)
			{
			FeedInterpolator(&CommData.InputData, &TestData);
			InterpolatorReady(Lin) ? (JogActuator(Lin, &TestData) ? Status.Flag.HipPositionSet == true : PositionMonitor()) : (void)0;	//Weight based set point, no data
			}
		    else
			{
			FeedInterpolator(&CommData.InputData, &TestData);
			InterpolatorReady(Step) ? (JogActuator(Step, &TestData) ? Status.Flag.GoalReached == true : PositionMonitor()) : (void)0;
			}
		    }
		else
		    {
		    FeedInterpolator(&CommData.InputData, &TestData);
		    InterpolatorReady(Step) ? (JogActuator(Step, &TestData) && PositionMonitor()) : (void)0;
		    }
		break;
	    case Err:
		Status.Flag.ActuatorsEnabled ? (DisableActuator(Act_All) ? Status.Flag.ActuatorsEnabled == false : Error_Handler()) : (void)0;
		LogMachineError() ? (void)0 : (LogSensorError() ? (void)0 : LogTestError());
		break;
	    }
	if (BufferTxData(&TxMsgRead) == HAL_OK)
	    {
	    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
	    xTaskNotify(SpinVcpHandle, COMM_TX, eSetBits);
	    }
	else
	    {
	    Error.Tasks.TxQ_ERR = true;
	    Error_Handler();
	    }
	}
    }

//State Machine Task -> Communication Task, memory safe data transfer function
void GetTxData(TxDataPacket* Destination)
    {
    memcpy(Destination, &TxMsgRead, sizeof(TxDataPacket));
    if (memcmp(Destination, &TxMsgRead, sizeof(TxDataPacket)) == 0)
	{
	return HAL_OK;
	}
    else
	{
	return HAL_ERROR;
	}
    }

void BufferTxData(TxDataPacket* Destination)
    {
    memcpy(Destination, &TxMsgWrite, sizeof(TxDataPacket));
    if (memcmp(Destination, &TxMsgWrite, sizeof(TxDataPacket)) == 0)
	{
	return HAL_OK;
	}
    else
	{
	return HAL_ERROR;
	}
    }


/* End of file ---------------------------------------------------------------*/
