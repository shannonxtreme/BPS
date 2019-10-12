/** ADC.c
 * Program for UTSVT BeVolt's Battery Protection System ADC initialization
 */
 
#ifndef ADC_H
#define ADC_H
#include <stdint.h>

typedef enum { 
	HIGH_PRECISION,
	LOW_PRECISION
} CurrentSensor;

void ADC_InitHilo(void);

uint32_t ADC_ReadHigh(void);

uint32_t ADC_ReadLow(void);

uint32_t ADC_Conversion(uint32_t ADC_Reading, CurrentSensor s);


#endif
