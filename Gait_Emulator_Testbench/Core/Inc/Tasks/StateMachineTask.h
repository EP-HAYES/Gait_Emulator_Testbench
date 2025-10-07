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

typedef enum
{
    Sleep,
    Boot,
    Idle,
    Test,
    Run,
    Error

}SystemStates;

typedef struct
{
    SystemStates State;

}SystemData;

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* End of file ---------------------------------------------------------------*/

#endif /* INC_TASKS_STATEMACHINETASK_H_ */
