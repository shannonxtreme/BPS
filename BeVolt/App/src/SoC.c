/** main.c
 * Program for UTSVT BeVolt's Battery Protection System SOC
 * @authors Garrett Wong, Sijin Woo
 * @lastRevised 1/13/2019
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "ADC.h"

#define MAX_CHARGE 1000*1000																								// In amp-hours (Ah)
uint32_t fixedPoint_SoC;																										// % of how much charge is left in battery with .01 resolution
float float_SoC;																														// float vers of SoC



/** Soc_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm.
 * Info found on reference sheet stm32f413 page 535
 */
void SoC_Init(void){ 
	// TODO: Initilize timer. 32 bit timer..
	
	/* Timer is used to find the timeDelta */

	/* Initializes Timer2 to */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 					//Enable TIM clock	
	TIM_TimeBaseInitTypeDef Init_TIM2;								 		//make struct
	
	Init_TIM2.TIM_Prescaler = 1;				
	Init_TIM2.TIM_CounterMode = TIM_CounterMode_Down;
	Init_TIM2.TIM_Period = 0xFFFF-1;											// 65535
	Init_TIM2.TIM_ClockDivision = TIM_CKD_DIV1;	
	Init_TIM2.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &Init_TIM2);										//call function
	TIM_Cmd(TIM2,ENABLE);																	//enable counter	
	
	// Grab from EEPROM what is the current SoC; For now, going to set it to 100.00%
	fixedPoint_SoC = 10000;
	float_SoC = 100.00;
}

/** SoC_Calculate
 * Calculates new SoC depending on the current reading
 * @param current reading from hall effect sensors. Fixed point of 0.0001 (1.500 Amps = 15000)
 * not a constant samping. Add on however much from the previous
 */
void SoC_Calculate(int32_t amps){ 
	// TODO: Coloumb counting algorithm.
	// Accumulator summation
	// need to find out how to raise a flag or access count register

	RCC_ClocksTypeDef RCC_Clocks;
	/* Get system clocks */
	RCC_GetClocksFreq(&RCC_Clocks);
	
		
	uint32_t counter = TIM2->CNT;											//find current value of up counter
	TIM2->CNT = 0;																		//set counter to zero to count up again
	
	float timeElapsed = (0xFFFF - counter)/60;
	timeElapsed /= 60;
	timeElapsed /= RCC_Clocks.SYSCLK_Frequency;  			// time in hours
	
	float float_amps = amps * .0001;
	
	float AhCollected = timeElapsed * float_amps; 	// 1 * .0001 * 10 to get resolution of .01
	
	float_SoC = (AhCollected * 100)/MAX_CHARGE + float_SoC;
	fixedPoint_SoC = float_SoC * 100;
}

/** SoC_Calibrate
 * Calibrates the SoC. Whenever the BPS trips, the SoC should recalibrate. If an undervoltage
 * fault occurs, the SoC calibrates to 0% and whenever there is an overvoltage, the SoC calibrates
 * to 100%.
 * @param voltage fault type. 0 if under voltage, 1 if over voltage
 */
void SoC_Calibrate(int32_t faultType){
	// TODO: depending on fault type, the accumulator should be set to 0% or 100%
	if (!(faultType == 0 || faultType == 1)) // ERROR
	if (faultType == 0) {
		fixedPoint_SoC = 0;
		float_SoC = 0;
	}
	if (faultType == 1) {
		fixedPoint_SoC = 10000;
		float_SoC = 100.00;
	}
}

/** SoC_GetPercent
 * Gets the percentage of charge left in the battery pack
 * @return fixed point percentage. Resolution = 0.01 (45.55% = 4555)
 */
uint32_t SoC_GetPercent(void){
	// TODO: returns percentage of charge
	return fixedPoint_SoC;
}

/** SoC_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator new value. Resolution = 0.0001.
 */
void SoC_SetAccum(int32_t accum){
	fixedPoint_SoC = accum/10; 
	float_SoC = accum/10000;
}

