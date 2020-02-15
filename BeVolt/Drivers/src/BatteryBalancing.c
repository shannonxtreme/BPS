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
	ReleaseCharge();
}
	
void ReleaseCharge(){ 
		uint8_t currVoltage;
		for (uint8_t k = 0; k < NUM_BATTERY_MODULES; k++) {
			// voltage @ i
			//if greater - set discharge Bit;
			//else clearerr discharge bit
		}
	// make all open circuits
	}
