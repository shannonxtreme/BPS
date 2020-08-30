#include "common.h"
#include "os.h"
#include "Current.h"
#include "Voltage.h"
#include "Temperature.h"

<<<<<<< HEAD
/*******************************************************************************
*    Shared Resources
*******************************************************************************/

/**
 * @brief   Queue for pushing and popping CAN Messages
 */
OS_Q CANBus_MsgQ;

/**
 * Semaphores
 */
OS_SEM SafetyCheck_Sem4;
OS_SEM Fault_Sem4;

/**
 * Mutexes
 */


void Tasks_Init(void) {

    OS_ERR err;

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                4,
                &err);

    OSSemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0,
                &err);
=======
void Tasks_Init(void) {

>>>>>>> 38847878d903775b4e06ab5abd32972bff8cc132
}

void Task_FaultState(void *p_arg) {
    (void)p_arg;
}

void Task_CriticalState(void *p_arg) {
    (void)p_arg;
}

void Task_PetWDog(void *p_arg) {
    (void)p_arg;
}

void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    while(1) {
        // Update Voltage Measurements
        // if voltage NOT safe:
        


        // Update Open Wire Measurements

        // Update Temperature Measurements

        // Control Fans depending on
    }
}

void Task_AmperesMonitor(void *p_arg) {
    (void)p_arg;

    while(1) {
        // Update Amperes Measurements
    }
}

<<<<<<< HEAD
void Task_LogInfo(void *p_arg) {
    (void)p_arg;
}

void Task_CANBusConsumer(void *p_arg) {
=======
void Task_CANBus(void *p_arg) {
>>>>>>> 38847878d903775b4e06ab5abd32972bff8cc132
    (void)p_arg;

    while(1) {
        // Todo:
    }
}

<<<<<<< HEAD
void Task_MotorNotify(void *p_arg) {

}

void Task_BatteryBalance(void *p_arg) {

=======
void Task_LogInfo(void *p_arg) {
    (void)p_arg;
>>>>>>> 38847878d903775b4e06ab5abd32972bff8cc132
}

void Task_CLI(void *p_arg) {
    (void)p_arg;

    while(1) {
        // Todo: 
    }
}

void Task_BLE(void *p_arg) {
    (void)p_arg;

    while(1) {
        // Todo:
    }
}

void Task_Idle(void *p_arg) {
<<<<<<< HEAD
    (void)p_arg;
=======
`   (void)p_arg;
>>>>>>> 38847878d903775b4e06ab5abd32972bff8cc132

    while(1) {
        // Todo: Do something cool or be lame.
    }
}
