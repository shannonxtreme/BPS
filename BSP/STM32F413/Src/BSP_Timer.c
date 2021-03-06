#include "BSP_Timer.h"
#include "stm32f4xx.h"

/**
 * @brief   Initialize the timer for time measurements.
 * @param   None
 * @return  None
 */
void BSP_Timer_Init(void) {
    /* Timer is used to find the timeDelta */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef Init_TIM2;
	
	Init_TIM2.TIM_Prescaler = 1;
	Init_TIM2.TIM_CounterMode = TIM_CounterMode_Down;
	Init_TIM2.TIM_Period = 0xFFFF-1;
	Init_TIM2.TIM_ClockDivision = TIM_CKD_DIV1;
	Init_TIM2.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &Init_TIM2);
}

/**
 * @brief   Starts the timer.
 * @param   None
 * @return  None
 */
void BSP_Timer_Start(void) {
    TIM_Cmd(TIM2,ENABLE);
}

/**
 * @brief   Gets the number of ticks that has elapsed since the last time this function was called.
 * @param   None
 * @return  Number of ticks
 */
uint32_t BSP_Timer_GetTicksElapsed(void) {
    /* Get system clocks */
	uint32_t counter = TIM2->CNT;       // find current value of up counter
	TIM2->CNT = 0;

    return counter;
}

/**
 * @brief   Gets the running frequency of the timer (time per tick)
 * @param   None
 * @return  frequency in Hz
 */
uint32_t BSP_Timer_GetRunFreq(void) {
    RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

    return RCC_Clocks.SYSCLK_Frequency;
}
