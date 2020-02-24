/* CLI.c
 * Command Line Interface wrapper to 
 * define and route commands
 */

#include "CLI.h"
#include <ctype.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#define MAX_TOKEN_SIZE 4

static cell_asic* Minions;
int32_t hashTokens[MAX_TOKEN_SIZE];
char hexString[8];
const float MILLI_UNIT_CONVERSION = 1000;
const float PERCENT_CONVERSION = 100;

/** CLI_Init
 * Initializes the CLI with the values it needs
 * @param boards is a cell_asic struct pointer to the minion boards
 */
void CLI_Init(cell_asic* boards) {
	Minions = boards;
	UART3_Init();
}

/** CLI_InputParse
 * Parses the input string and stores in hashTokens[]
 * @param input is a pointer to the input string
 */
void CLI_InputParse(char *input) {
	char *tokenized;
	char *split = strtok_r(input, " ", &tokenized);
	for(int i = 0; i < MAX_TOKEN_SIZE && split != NULL; i++) {
		for(int j = 0; j < strlen(split); j++) {
			split[j] = tolower(split[j]);
		}
		if(i == 3) {//CAN requires argument #3 to be in hex
			strcpy(hexString, split);
		}
		if(!isalpha(split[0])) {
			sscanf(split, "%d", &hashTokens[i]);
		} else {
			hashTokens[i] = CLI_StringHash(split);
		}
		split = strtok_r(NULL, " ", &tokenized);
	}
}

/** CLI_StringHash
 * Calculates the hashed value of the input
 * @param string is the string to be hashed
 * @return the integer hash value
 */ 
uint32_t CLI_StringHash(char* string) {
	uint32_t hash = 0;
	uint32_t m = 1000000009;
	uint8_t p = 31;
	for(uint8_t i = 0; string[i]; i++) {
		hash += string[i] * pow(p, i);
	}
	return hash % m;
}

/** CLI_Startup
 * Prints the welcome messages
 */
void CLI_Startup(void) {
	printf("\n\r\n\r%s\n\r\n\r", utsvt_logo);
	printf("Hello! Welcome to the BPS System! I am your worst nightmare...\n\r");
	printf("Please enter a command (Type 'help' to see a list of commands)\n\r");
	printf(">> ");
}

/** CLI_Help
 * Displays the help menu
 */
void CLI_Help(void) {
	printf("-------------------------Help Menu-------------------------\n\r");
	printf("The available commands are: \n\r");
	printf("Voltage\t\t\tCurrent\t\t\tTemperature\n\r");
	printf("Contactor/Switch\tCharge\t\t\tLights/LED\n\r");
	printf("CAN\t\t\tEEPROM\t\t\tDisplay\n\r");
	printf("LTC/Register\t\tWatchdog\t\tADC\n\r");
	printf("Critical/Abort\t\tAll\n\r");
	printf("Keep in mind: all values are 1-indexed\n\r");
	printf("-----------------------------------------------------------\n\r");
}

/** CLI_Voltage
 * Checks and displays the desired
 * voltage parameter(s)
 */
void CLI_Voltage(void) {
	Voltage_UpdateMeasurements();
	if(hashTokens[1] == NULL) {
		for(int i = 0; i < NUM_BATTERY_MODULES; i++){
			printf("Module number %d: %.3fV\n\r", i+1, Voltage_GetModuleMillivoltage(i)/MILLI_UNIT_CONVERSION);
		}
		return;
	}
	SafetyStatus voltage = Voltage_CheckStatus();
	switch(hashTokens[1]){		
		// Specific module
		case CLI_MODULE_HASH:
			if (hashTokens[2] == NULL || hashTokens[2] > NUM_BATTERY_MODULES || hashTokens[2] < 1){
				printf("Invalid module number");
			}
			else {
				printf("Module number %d: %.3fV\n\r", hashTokens[2], Voltage_GetModuleMillivoltage(hashTokens[2]-1)/MILLI_UNIT_CONVERSION);
			}
			break;
		// Total
		case CLI_TOTAL_HASH:
			printf("Total voltage: %.3fV\n\r", Voltage_GetTotalPackVoltage()/MILLI_UNIT_CONVERSION);
			break;
		// Safety Status
		case CLI_SAFE_HASH:
		case CLI_SAFETY_HASH:	
			printf("Safety Status: ");
				switch(voltage) {
					case SAFE: 
						printf("SAFE\n\r");
						break;
					case DANGER: 
						printf("DANGER\n\r");
						break;
					case OVERVOLTAGE:
						printf("OVERVOLTAGE\n\r");
						break;
					case UNDERVOLTAGE: 
						printf("UNDERVOLTAGE\n\r");
						break;
					default:
						break;
				}		
				break;
		default:
			printf("Invalid voltage command\n\r");
			break;
	}
}

/** CLI_Current
 * Checks and displays the desired
 * current parameter(s)
 */
void CLI_Current(void) {
	if(hashTokens[1] == NULL) {
		printf("High: %.3fA\n\r", Current_GetHighPrecReading()/MILLI_UNIT_CONVERSION);	// Prints 4 digits, number, and A
		printf("Low: %.3fA\n\r", Current_GetLowPrecReading()/MILLI_UNIT_CONVERSION);
		return;
	}
	switch (hashTokens[1]) {
		case CLI_HIGH_HASH: // High precision reading
			printf("High: %.3fA     \n\r", Current_GetHighPrecReading()/MILLI_UNIT_CONVERSION);
			break;
		case CLI_LOW_HASH: // Low precision reading
			printf("Low: %.3fA     \n\r", Current_GetLowPrecReading()/MILLI_UNIT_CONVERSION);
			break;
		case CLI_SAFE_HASH: 
		case CLI_SAFETY_HASH:
			if (Current_CheckStatus() == SAFE) {
				printf("Safety Status: SAFE\n\r");
			}
			else {
				printf("Safety Status: DANGER\n\r");
			}
			break;
		case CLI_CHARGE_HASH: // Whether battery is charging
		case CLI_CHARGING_STATE:
			if (Current_IsCharging()) {
				printf("Charging State: CHARGING\n\r");
			}
			else {
				printf("Charging State: DISCHARGING\n\r");
			}
			break;
		default:
			printf("Invalid Current Command\n\r");
			break;
		}
}

/** CLI_Temperature
 * Checks and displays the desired
 * temperature parameter(s)
 */
void CLI_Temperature(void) {
	if(hashTokens[1] == NULL) {
		for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
			printf("Module number %d: %.3f C\n\r", i+1, Temperature_GetModuleTemperature(i)/MILLI_UNIT_CONVERSION);
		}
		return;
	}
	switch(hashTokens[1]) {			
		// All temperature sensors
		case CLI_ALL_HASH:
			for(int i = 0; i < NUM_MINIONS; i++) {		// last minion only has 14 sensors
				for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
					if(i == NUM_MINIONS-1 && j > NUM_TEMP_SENSORS_LAST_MINION-1) {	// -1 for 0-index
						break;
					}
					printf("Sensor number %d: %.3f C\n\r",(i*MAX_TEMP_SENSORS_PER_MINION_BOARD)+j+1, Temperature_GetSingleTempSensor(i, j)/MILLI_UNIT_CONVERSION);
				}
			}
			break;
		// Temperature of specific module
		case CLI_MODULE_HASH:
			if (hashTokens[2] == NULL || hashTokens[2]-1 > NUM_BATTERY_MODULES || hashTokens[2]-1 < 0){
				printf("Invalid module number\n\r");
			}
			else {
				if(hashTokens[3] == NULL) {//temperature of module
					printf("Module number %d: %.3f C\n\r", hashTokens[2], Temperature_GetModuleTemperature(hashTokens[2]-1)/MILLI_UNIT_CONVERSION);
				} else if(hashTokens[3]-1 == 0 || hashTokens[3]-1 == 1) {//temperature of specific sensor in module
					uint16_t boardNum = (hashTokens[2]-1)/MAX_VOLT_SENSORS_PER_MINION_BOARD;
					printf("Sensor %d on module %d: %.3f C\n\r", hashTokens[3], hashTokens[2], 
							Temperature_GetSingleTempSensor(boardNum, hashTokens[3]-1)/MILLI_UNIT_CONVERSION);
				} else {
					printf("Invalid sensor number\n\r");
				}
			}
			break;
		// Average temperature of the whole pack
		case CLI_TOTAL_HASH:
			printf("Total average temperature: %.3f C\n\r", Temperature_GetTotalPackAvgTemperature()/MILLI_UNIT_CONVERSION);
			break;
		default:
			printf("Invalid temperature command\n\r");
			break;
	}
}

/** CLI_LTC6811
 * Prints register information
 * All registers are of the same type
 * Prints each register's respective three registers
 * (tx_data, rx_date, and rx_pec_match)
 */
void CLI_LTC6811(void) {
	for(uint8_t current_ic = 0; current_ic < NUM_MINIONS; current_ic++) {
		printf("Minion board %d: ", current_ic);
		printf("Config: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].config.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].config.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].config.rx_pec_match);
		printf("Config B: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].configb.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].configb.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].configb.rx_pec_match);
		printf("COM: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].com.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].com.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].com.rx_pec_match);
		printf("S Control: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].sctrl.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].sctrl.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].sctrl.rx_pec_match);
		printf("S Control B: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].sctrlb.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].sctrlb.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].sctrlb.rx_pec_match);
	}
}

/** CLI_Contactor
 * Interacts with contactor status by
 * printing the status of the contactor
 */
void CLI_Contactor(void) {
	FunctionalState contactor = Contactor_Flag();
	if(hashTokens[1] == NULL) {
		if(contactor == ENABLE) {
			printf("Contactor is Enabled\n\r");
		} else {
			printf("Contactor is Disabled\n\r");
		}
	} else {
		printf("Invalid contactor command\n\r");
	}
}

/** CLI_Charge
 * Checks and displays the desired
 * state of charge parameter(s)
 */
void CLI_Charge(void) {
	if(hashTokens[1] == NULL) {
		printf("The battery percentage is %.2f%%\n\r", SoC_GetPercent()/PERCENT_CONVERSION);
		return;
	}
	switch(hashTokens[1]) {
		case CLI_RESET_HASH:
			SoC_SetAccum(0);	//resets accumulator
			printf("Accumulator has been reset\n\r");
			break;
		case CLI_SET_HASH:
			SoC_SetAccum(hashTokens[2]);
			printf("Accumulator has been set to %d%%\n\r", hashTokens[2]);
			break;
		default: 
			printf("Invalid Charge Command\n\r");
			break;
	}
}

/** toggleLED
 * Helper function for CLI_LED
 * that toggles a given led
 * @param led is the led to toggle
 */
void toggleLED(led input) {
	if(hashTokens[2] == 1 || hashTokens[2] == CLI_ON_HASH) {
		LED_On(input);
	}
	else if(hashTokens[2] == 0 || hashTokens[2] == CLI_OFF_HASH) {
		LED_Off(input);
	} else {
		printf("Invalid LED command\n\r");
	}
}

/** CLI_LED
 * Interacts with the LEDs by 
 * checking error light status
 * running a full LED test
 * and turning a specific LED on/off
 */
void CLI_LED(void) {
	LED_Init();
	uint8_t error = (GPIOB->ODR) & GPIO_Pin_12;
	if(hashTokens[1] == NULL) {
		if(error) {
			printf("Error light is On\n\r");
		} else {
			printf("Error light is Off\n\r");
		}
		return;
	}
	switch(hashTokens[1]) {
		case CLI_TEST_HASH:
			printf("\n\r");
			for(int i = 0; i < 10; i++) {
				LED_Toggle(FAULT);
				DelayMs(100);
				LED_Toggle(RUN);
				DelayMs(100);
				LED_Toggle(UVOLT);
				DelayMs(100);
				LED_Toggle(OVOLT);
				DelayMs(100);
				LED_Toggle(OTEMP);
				DelayMs(100);
				LED_Toggle(OCURR);
				DelayMs(100);
				LED_Toggle(WDOG);
				DelayMs(100);
				LED_Toggle(CAN);
				DelayMs(100);
				LED_Toggle(EXTRA);
				DelayMs(100);
			}
			break;
		case CLI_FAULT_HASH:
			toggleLED(FAULT);
			break;
		case CLI_RUN_HASH:
			toggleLED(RUN);
			break;
		case CLI_OCURR_HASH:
			toggleLED(OCURR);
			break;
		case CLI_OTEMP_HASH:
			toggleLED(OTEMP);
			break;
		case CLI_OVOLT_HASH:
			toggleLED(OVOLT);
			break;
		case CLI_WDOG_HASH:
			toggleLED(WDOG);
			break;
		case CLI_CAN_HASH:
			toggleLED(CAN);
			break;
		case CLI_EXTRA_HASH:
			toggleLED(EXTRA);
			break;
		default:
			printf("Invalid LED command\n\r");
			break;
	}
}

/** CLI_CAN
 * Interacts with CAN by
 * reading and writing to bus
 */
void CLI_CAN(void) {
	uint8_t readData;
	uint8_t writeData[8];
	for(uint8_t i = 0; i < 7; i+= 2) {
		char tempData[2] = {hexString[i], hexString[i+1]};
		writeData[i/2] = strtol(tempData, NULL, 16);
	}
	switch(hashTokens[1]) {
		case CLI_READ_HASH: 
			while(CAN1_Read(&readData)){
				printf("%d\n\r ", readData);
			}
		break;
		case CLI_WRITE_HASH: 
			switch(hashTokens[2]) {
				case CLI_TRIP_HASH:
					CAN1_Write(CAN_ID_BPS_TRIP, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_CLEAR_HASH:
					CAN1_Write(CAN_ID_BPS_ALL_CLEAR, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_OFF_HASH:
					CAN1_Write(CAN_ID_BPS_OFF, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_CURRENT_HASH:
					CAN1_Write(CAN_ID_CURRENT_DATA, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_VOLTAGE_HASH:
					CAN1_Write(CAN_ID_TOTAL_VOLTAGE_DATA, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_TEMPERATURE_HASH:
					CAN1_Write(CAN_ID_AVG_TEMPERATURE_DATA, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_CHARGE_HASH:
					CAN1_Write(CAN_ID_SOC_DATA, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_WATCHDOG_HASH:
					CAN1_Write(CAN_ID_WDOG_TRIGGERED, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				case CLI_ERROR_HASH:
					CAN1_Write(CAN_ID_ERROR, writeData, (strlen(hexString)/2)+(strlen(hexString)%2));
					break;
				default:
					printf("Invalid ID\n\r");
					break;
			}
			break;
		default:
			printf("Invalid CAN command\n\r");
			break;
	}
}
	
// TODO: Confirm status of Display;
//		 if operational, display proper information (specifics found on Drive)
/** CLI_Display
 * Interacts with the display
 */
void CLI_Display(void) {}

/** CLI_Watchdog
 * Shows whether watchdog was tripped
 */
void CLI_Watchdog(void) {
	if(hashTokens[1] == NULL) {
		printf("Safety Status: ");
		if (WDTimer_DidSystemReset() == SAFE){
			printf("SAFE\n\r");
		} else if (WDTimer_DidSystemReset() == DANGER){
			printf("DANGER\n\r");
		}
	}
	uint8_t errorAddrArray[2];
	EEPROM_ReadMultipleBytes(EEPROM_WATCHDOG_PTR_LOC, 2, errorAddrArray);
	uint16_t errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
	switch(hashTokens[1]) {
		case ERROR:
			printf("Most recent Watchdog error: %d", EEPROM_ReadByte(errorAddr-1));
			break;
		default:
			printf("Invalid Watchdog command");
			break;
	}
}

/** CLI_EEPROM
 * Interacts with EEPROM
 * by reading and writing to the EEPROM
 */
void CLI_EEPROM(void) {
	if(hashTokens[1] == NULL) {
		EEPROM_SerialPrintData();
		return;
	}
	uint8_t errorAddrArray[2];
	uint16_t errorAddr = 0;
	switch(hashTokens[1]) {
		case RESET:
			EEPROM_Reset();
			printf("EEPROM has been reset");
			break;
		case CLI_ERROR_HASH:
			switch(hashTokens[2]) {
				case FAULT:
					EEPROM_ReadMultipleBytes(EEPROM_FAULT_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Fault error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CLI_TEMPERATURE_HASH:
					EEPROM_ReadMultipleBytes(EEPROM_TEMP_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Temperature error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CLI_VOLTAGE_HASH:
					EEPROM_ReadMultipleBytes(EEPROM_VOLT_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Voltage error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CLI_CURRENT_HASH:
					EEPROM_ReadMultipleBytes(EEPROM_CURRENT_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Current error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CLI_WATCHDOG_HASH:
					EEPROM_ReadMultipleBytes(EEPROM_WATCHDOG_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Watchdog error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CAN:
					EEPROM_ReadMultipleBytes(EEPROM_CAN_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("CAN error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CLI_CHARGE_HASH:
					EEPROM_ReadMultipleBytes(EEPROM_SOC_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("SoC error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				default:
					break;
			}
			break;
		default:
			printf("Invalid EEPROM command\n\r");
			break;
	}
}

/** CLI_ADC
 * Prints the high precision and
 * low precision readings
 */
void CLI_ADC(void) {
	printf("High precision ADC: %d\n\r", ADC_ReadHigh());
	printf("Low precision ADC: %d\n\r", ADC_ReadLow());
}


/** CLI_Critical
 * Shuts off contactor manually
 */  
void CLI_Critical(void) {
	Fifo criticalFifo;
	char response[fifo_size];
	printf("Please type 'shutdown' to turn the contactor off\n\r");
	printf(">> ");
	while(1) {
		UART3_CheckAndEcho(&criticalFifo);
		if(UART3_HasCommand(&criticalFifo)) {
			UART3_GetCommand(&criticalFifo, response);
			if(CLI_StringHash(response) == CLI_SHUTDOWN_HASH) {
				Contactor_Off();
				printf("Contactor is off\n\r");
				break;
			} else {
				printf("Contactor is still on\n\r");
				break;
			}
		}
	}
}

/** CLI_All
 * Displays all information about BPS modules
 * (voltage, current, temperature, charge, contactor)
 */
void CLI_All(void){
	printf("Voltage: \n\r");
	hashTokens[0] = CLI_VOLTAGE_HASH;
	hashTokens[1] = 0;
	CLI_Voltage();
	printf("Current: \n\r");
	hashTokens[0] = CLI_CURRENT_HASH;
	CLI_Current();
	printf("Temperature: \n\r");
	hashTokens[0] = CLI_TEMPERATURE_HASH;
	CLI_Temperature();
	printf("State of Charge: \n\r");
	hashTokens[0] = CLI_CHARGE_HASH;
	CLI_Charge();
	printf("Contactor: \n\r");
	hashTokens[0] = CLI_CONTACTOR_HASH;
	CLI_Contactor();
}

/** CLI_Handler
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Handler(char *input) {
	CLI_InputParse(input);
	if(hashTokens[0] == CLI_PARTYTIME_HASH) {
		printf("\n\r");
		for(int i = 0; i < 100; i++) {
			printf("%s", party_parrot_frames[i%10]);
			for(int j = 0; j < 18 && i < 99; j++) {
					printf("\033[A\r");
			}
		}
		printf("\n\r");
		printf(">> ");
	return;
	}
	if(hashTokens[0] == CLI_PING_HASH) {
		printf("pong\n\r");
		printf(">> ");
		return;
	}
	switch(hashTokens[0]) {
		// Help menu
		case CLI_MENU_HASH:
		case CLI_HELP_HASH:
			CLI_Help();
			break;
		// Voltage commands
		case CLI_VOLTAGE_HASH:
			CLI_Voltage();
			break;
		// Current commands
		case CLI_CURRENT_HASH:
			CLI_Current();
			break;
		// Temperature commands
		case CLI_TEMPERATURE_HASH:
			CLI_Temperature();
			break;
		// LTC6811 register commands
		case CLI_REGISTER_HASH:
		case CLI_LTC_HASH:
			CLI_LTC6811();
			break;
		// Contactor/Switch commands
		case CLI_SWITCH_HASH:
		case CLI_CONTACTOR_HASH:
			CLI_Contactor();
			break;
		// State of Charge commands
		case CLI_CHARGE_HASH:
			CLI_Charge();
			break;
		// Error light commands
		case CLI_LED_HASH:
		case CLI_LIGHTS_HASH:
			CLI_LED();
			break;
		// CAN commands
		case CLI_CAN_HASH:
		case CLI_CANBUS_HASH:
			CLI_CAN();
			break;
		// Display commands
		case CLI_DISPLAY_HASH:
			CLI_Display();
			break;
		// Watchdog commands
		case CLI_WATCHDOG_HASH:
			CLI_Watchdog();
			break;
		// EEPROM commands
		case CLI_EEPROM_HASH:
			CLI_EEPROM();
			break;
		// Peripheral commands
		case CLI_ADC_HASH:
			CLI_ADC();
			break;
		// Emergency Abort
		case CLI_CRITICAL_HASH:
		case CLI_ABORT_HASH:
			CLI_Critical();
			break;		// ABORT
		// All
		case CLI_ALL_HASH:
			CLI_All();
			break;
		default:
			printf("Invalid command. Type 'h' or 'm' or '?' for the help menu\n\r");
			break;
	}
	hashTokens[0] = NULL;
	hashTokens[1] = NULL;
	hashTokens[2] = NULL;
	hashTokens[3] = NULL;
	printf(">> ");
}
