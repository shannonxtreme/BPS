/* SysTick.c
 * SysTick file that accesses the system ticker for 
 * clock and delay funcitonality
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* SysTick_Handler
 * Called every us; updates the usTicks counter
 */
void SysTick_Handler(void);

/* DelayInit
 * Initializes the core clock configuration
 */
void DelayInit(void);

/* DelayUs
 * Delays 'us' amount of us and then continues
 */
void DelayUs(uint32_t us);

/* DelayMs
 * Delay 'ms' amount of ms and then continues
 */
void DelayMs(uint32_t ms);

#endif 
