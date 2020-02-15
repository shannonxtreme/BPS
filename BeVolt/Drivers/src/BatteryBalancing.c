#include "SPI.h"
#include "LTC6811.h"
#include "Voltage.h"
#include "BatteryBalancing.h"
#include "config.h"
#include "Current.h"

#define chargingTolerance 100

uint16_t minVoltage = 0;
uint8_t minIndex = 0;

void ReleaseChargeInit() {
	if (Current_CheckStatus() == 1){
		return;//If charging, return
	}
	minVoltage = Voltage_GetModuleMillivoltage(0);
	for (uint8_t i = 1; i < NUM_BATTERY_MODULES; i++){
		if(Voltage_GetModuleMillivoltage(i) < minVoltage) {
			minVoltage = Voltage_GetModuleMillivoltage(i);//Find minimum voltage
			minIndex = i;//Find module with minimum voltage
		}
	}	
}
	
void ReleaseCharge(cell_asic Minions[]){ 
	bool stillDischarging = false; 
	for (uint8_t k = 0; k < NUM_BATTERY_MODULES; k++) {
		uint16_t voltage = Voltage_GetModuleMillivoltage(k);//Get voltage of module
		if (voltage > minVoltage + chargingTolerance) {//Check to see if module is greater than min
			setDischarge(k, Minions);//Set discharge bit if module is too high
			stillDischarging = true;
		}
		else {//Clear discharge bit of module if it reaches minimum
			uint8_t ICIndex;
			uint8_t MNumber;
			getICNumber(k, &ICIndex, &MNumber);//Get module number and IC Board
			ClearDischargeBit(MNumber , 1, &Minions[ICIndex]);//Clear discharge bit
		}
	}
	// make all open circuits
	if (!stillDischarging) {
		clear_discharge(NUM_MINIONS, Minions);
	}
}

void ClearDischargeBit(int Cell, uint8_t total_ic, cell_asic ic[])
{
  for (int i=0; i<total_ic; i++)
  {
    if (Cell<9)
    {
      ic[i].config.tx_data[4] = ic[i].config.tx_data[4] & ~(1<<(Cell-1));
    }
    else if (Cell < 13)
    {
      ic[i].config.tx_data[5] = ic[i].config.tx_data[5] & ~(1<<(Cell-9));
    }
  }
}

void setDischarge(uint8_t i, cell_asic ic[]) {
	uint8_t ICNumber = 0;
	uint8_t ModuleNumber = 0;
	getICNumber(i, &ICNumber, &ModuleNumber);//Get IC and Module number
	LTC6811_set_discharge(ModuleNumber, 1, &ic[ICNumber]); //Set discharge bit 
}

void getICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber) {
	uint8_t total = 0;
	for(int m = 0; m < NUM_MINIONS; m++) {
		total += NUM_MODULES_PER_MINION[m];
		if(i < total) {
			*ICNumber = m;
			*ModuleNumber = i - total;
			return;
		}
	}
}
