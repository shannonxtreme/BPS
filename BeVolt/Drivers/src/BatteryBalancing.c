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
			minVoltage = Voltage_GetModuleMillivoltage(i);//Find minimum voltage and store in minVoltage
			minIndex = i;//Find module with minimum voltage and store in minIndex
		}
	}
}
	
void ReleaseCharge(cell_asic Minions[]){ 
	int m = 1;
	for (uint8_t k = 0; k < NUM_BATTERY_MODULES; k++) {
		int n = 8; //Number of modules that will be set
		if (k == 24){
		n = 7; // Last minion board will have 7 modules
		}
		if (k%8 == 0){
		m++;//if we get through 8 modules, go to next minion board
		}
		LTC6811_set_discharge(n, k, &Minions[m]);
		}
	}
