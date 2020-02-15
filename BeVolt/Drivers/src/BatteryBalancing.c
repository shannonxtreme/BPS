#include "SPI.h"
#include "LTC6811.h"
#include "Voltage.h"
#include "BatteryBalancing.h"


	void BalanceBattery(){
	if (Voltage_CheckStatus() == UNDERVOLTAGE){ 
			ChargeMin();
		}
	if (Voltage_CheckStatus() == OVERVOLTAGE){ 
			ReleaseCharge();
		}
	}
	
	void ReleaseCharge(){
		
	}
	
	void ChargeMin(){
	}
	