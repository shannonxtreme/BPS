#include "LTC6811.h"
#include "LTC6811.h"
#include "Voltage.h"

void ReleaseChargeInit(void);
/*
Function wil return index with value of minimum voltage and module with minimum voltage

*/

void ReleaseCharge(cell_asic Minions[]);

void setDischarge(uint8_t i, cell_asic ic[]);
	
void getICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber);