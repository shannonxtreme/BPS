/** ADC.c
 * Program for UTSVT BeVolt's Battery Protection System ADC initialization
 */

#ifndef ADC_H
#define ADC_H
#include <stdint.h>

void ADC_InitHilo(void);

uint16_t ADC_ReadHigh(void);

uint16_t ADC_ReadLow(void);

#endif
