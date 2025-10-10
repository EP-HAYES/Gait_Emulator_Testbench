/*
 * MachineManager.c
 *
 *  Created on: Oct 8, 2025
 *      Author: EP HAYES
 */


/* Includes ------------------------------------------------------------------*/

#include "MachineManager.h"
#include "StateMachineTask.h"

/* Private define ------------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/

extern SystemData Status;

/* Private function prototypes -----------------------------------------------*/

void ChangeState(uint8_t NewState)
    {

    }

void ProcessCommand(char* RxCommand, char* TxCondition)				//If you fuck with the buffer sizes this function will no longer work
    {
    if (strcmp(RxCommand, BOOT_CHECK ))
	{

	}
    else
	{

	}
    }


/* End of file ---------------------------------------------------------------*/
