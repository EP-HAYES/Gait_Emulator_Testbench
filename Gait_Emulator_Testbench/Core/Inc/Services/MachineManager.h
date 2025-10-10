/*
 * MachineManager.h
 *
 *  Created on: Oct 8, 2025
 *      Author: EP HAYES
 */

#ifndef INC_SERVICES_MACHINEMANAGER_H_
#define INC_SERVICES_MACHINEMANAGER_H_

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "main.h"


/* Private define ------------------------------------------------------------*/

#define BOOT_CHECK	"BOOT?"
#define BOOT_GOOD	"BOOT_OK"
#define HOME_HIP	"HIP0"
#define HOME_LEG	"LEG0"
#define BOOT_GOOD	"BOOT_OK"
#define HOME_HIP	"HIP0"

/* Private variables ---------------------------------------------------------*/

typedef enum
{
    Act_All,
    Step,
    Lin,

}Actuators;

typedef enum
{
    Sens_All,
    Step_Enc,
    Leg_Accs,
    Lin_Amps,
    Lin_Pos,
    Lin_Stg,
    Lin_Lim,
    Inc_Acc,
    Spd_Enc

}Sensors;

/* Private function prototypes -----------------------------------------------*/

void ChangeState(uint8_t NewState);
void ProcessCommand(char* RxCommand, char* TxCondition);
bool DisableActuator(Actuators Actuator);
void DisableSensor(Sensors Sensor);
void SensorTest(void);								//Check all sensors reading non-zero, use error state for failure
void ActuatorTest(void);							//Check all actuators motion capabilities, no control loop testing, use error state for failure

/* End of file ---------------------------------------------------------------*/

#endif /* INC_SERVICES_MACHINEMANAGER_H_ */
