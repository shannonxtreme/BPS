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
		// voltage @ i
		//if greater - set discharge Bit;
		//else clearerr discharge bit
		uint16_t voltage = Voltage_GetModuleMillivoltage(k);
		if (voltage > minVoltage + chargingTolerance) {
			//insert discharging code
			stillDischarging = true;
		}
		else {
			//clear discharge bit
		}
	}
	// make all open circuits
	if (!stillDischarging) {
		clear_discharge(NUM_MINIONS, Minions);
	}
}
