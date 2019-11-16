/** Contactor.h
 * Sets Contactor on or off
 * Checks if flag is high or low. If high, return 1, if low, return 0.
 */

#include <stdint.h>
#include "config.h"

#ifndef CONTACTOR_H__
#define CONTACTOR_H__

// Initiliazes GPIOA_Pin_6
void Contactor_Init(void);


// Closes contactor, GPIO_Pin_6 = 1
void Contactor_On(void);

 
//Opens contactor, GPIO_Pin_6 = 0
void Contactor_Off(void);

 
//Outputs: flag status (0 or 1)
FunctionalState Contactor_Flag(void);

#endif
