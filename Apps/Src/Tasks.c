#include "common.h"
#include "os.h"
#include "Current.h"
#include "Voltage.h"
#include "Temperature.h"

void Tasks_Init(void) {

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

void Task_CANBus(void *p_arg) {
    (void)p_arg;

    while(1) {
        // Todo:
    }
}

void Task_LogInfo(void *p_arg) {
    (void)p_arg;
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
`   (void)p_arg;

    while(1) {
        // Todo: Do something cool or be lame.
    }
}
