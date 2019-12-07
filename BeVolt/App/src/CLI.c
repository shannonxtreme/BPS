/* CLI.c
 * Command Line Interface wrapper to 
 * define and route commands
 */

#include "CLI.h"
#include <ctype.h>

#define MAX_TOKEN_SIZE 4

char* tokens[MAX_TOKEN_SIZE];

/** CLI_InputParse
 * Parses the input string
 * @return ptr to token
 */
void CLI_InputParse(char *input) {
	char *tokenized;
	char *split = strtok_r(input, " ", &tokenized);
	for(int i = 0; i < MAX_TOKEN_SIZE && split != NULL; i++) {
		tokens[i] = split;
		split = strtok_r(NULL, " ", &tokenized);
	}
}

/** CLI_GetToken
 * @param index of desired token
 * @return token at desired index
 */
char* CLI_GetToken(uint8_t idx) {
	return tokens[idx];
}

/** CLI_Help
 * Displays the help menu
 * @param input command
 */
void CLI_Help(char *input) {
	
}

/** CLI_Voltage
 * Checks and displays the desired
 * voltage parameter(s)
 * @param input command
 */
void CLI_Voltage(char *input) {
	switch(CLI_GetToken(1)[0]){
		// All modules
		case NULL:
				for(int i = 0; i < NUM_BATTERY_MODULES; i++){
					printf("\n\rModule Number ");
					printf("%d", i+1);
					printf(": ");
					printf("%d",Voltage_GetModuleMillivoltage(i));
					printf("\n\r");
				}
				break;
		
		// Specific module
		char modNum = CLI_GetToken(2)[0]-1;
		case 'm':
			if (modNum == NULL || modNum > NUM_BATTERY_MODULES || modNum < 0){
				printf("Invalid Module Number");
			}
			else {
				printf("\n\rModule Number ");
				printf("%c", modNum+1);
				printf(": ");
				printf("%c",Voltage_GetModuleMillivoltage(modNum));
				printf("\n\r");
			}
			break;
		
		// Total
		case 't':
			printf("\n\rTotal voltage: ");
			printf("%d",Voltage_GetTotalPackVoltage());
			printf("\n\r");
			break;
		
		// Safety Status
		case 's':	
			printf("\n\r");
				SafetyStatus voltage = Voltage_IsSafe();
				switch(voltage) {
					case SAFE: 
						printf("SAFE");
						break;
					case DANGER: 
						printf("DANGER");
						break;
					case OVERVOLTAGE:
						printf("OVERVOLTAGE");
						break;
					case UNDERVOLTAGE: 
						printf("UNDERVOLTAGE");
						break;
					default:
						break;
			break;
			}
				
		default:
			printf("Invalid voltage command.");
			break;
		}
	}

/** CLI_Current
 * Checks and displays the desired
 * current parameter(s)
 * @param input command
 */
void CLI_Current(char *input) {}

/** CLI_Temperature
 * Checks and displays the desired
 * temperature parameter(s)
 * @param input command
 */
void CLI_Temperature(char *input) {
	switch(CLI_GetToken(1)[0]){
		// Average temperature of modules
		case NULL:
			for(int i = 0; i < NUM_BATTERY_MODULES; i++){
					printf("\n\rModule Number ");
					printf("%d", i+1);
					printf(": ");
					printf("%d",Temperature_GetModuleTemperature(i));
					printf("\n\r");
			break;
			
		// All temperature sensors
		case 'a':
			break;
			
		// Temperature of specific module
		case 'm':
			break;
			
		// Average temperature of the whole pack
		case 't':
			break;
			
		default:
			printf("Invalid temperature command.");
			break;
		}
	}
}

/** CLI_Contactor
 * Interacts with contactor status
 * @param input command
 */
void CLI_Contactor(char *input) {}

/** CLI_Charge
 * Checks and displays the desired
 * state of charge parameter(s)
 * @param input command
 */
void CLI_Charge(char *input) {}

/** CLI_ErrorLight
 * Interacts with the error light
 * @param input command
 */
void CLI_ErrorLight(char *input) {}

/** CLI_CAN
 * Interacts with CAN
 * @param input command
 */
void CLI_CAN(char *input) {}

/** CLI_Display
 * Interacts with the display
 * @param input command
 */
void CLI_Display(char *input) {}

/** CLI_Watchdog
 * Interacts with the watchdog timer
 * @param input command
 */
void CLI_Watchdog(char *input) {}

/** CLI_EEPROM
 * Interacts with EEPROM
 * @param input command
 */
void CLI_EEPROM(char *input) {}

/** CLI_Peripherals
 * Interacts with the peripherals
 * @param input command
 */
void CLI_Peripherals(char *input);

/** CLI_Critical
 * Checks and displays the desired
 * voltage parameter(s)
 * @param input command
 */
void CLI_Critical(char *input) {}
	
/** CLI_Commands
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input command
 */
void CLI_Commands(char *input){	
	char* saveptr;
  //char* split = strtok_r(input," ", &saveptr);
	switch(input[0]) {
		// Help menu
		case 'h':
		case 'H':
		case 'm':
		case 'M':
		case '?':
			CLI_Help(input);
			break;
		
		case 'v':
		case 'V':
			break;		// voltage commands
		
		case 'i':
		case 'I':
			break;		// current commands
		
		case 't':
		case 'T':
			break;		// temp commands
		
		case 's':
		case 'S':
			break;		// contactor/switch commands
		
		case 'q':
		case 'Q':
		case '%':
			break;		// SoC commands
		
		case 'l':
		case 'L':
			break;		// error light commands
		
		case 'c':
		case 'C':
			break;		// CAN commands
		
		case 'd':
		case 'D':
			break;		// display commands
		
		case 'w':
		case 'W':
			break;		// watchdog commands
		
		case 'e':
		case 'E':
			break;		// EEPROM commands
		
		case 'p':
		case 'P':
			break;		// peripheral commands
		
		case '!':
			break;		// ABORT
		
		default:
			printf("Invalid command. Type 'h' or 'm' or '?' for the help menu");
			break;
	}
	//char* measurement;
  //char* function;
  //char* parameter;
	//parameter = NULL;
	//uint8_t word = 0;
/*
	// Parsing the input string and tokenizing into the variables
	while(split != NULL) {
    if (word == 0){
      measurement = split;
    }
		else if (word == 1){
      function = split;
    }
		else if (word == 2){
      parameter = split;
    }
    split = strtok_r(NULL, " ", &saveptr);
    word++;
	}
	// Voltage commands
	if(strcmp(measurement, "voltage") == 0){
		if(strcmp(function, "check") == 0){
			if(parameter == NULL){			// All modules
				for(int i = 0; i<12; i++){
					printf("\n\r%d", i);
					printf("th module: ");
					printf("%d",Voltage_GetModuleMillivoltage(i));
				}
				printf("\n\r");
			}else{			// Specific module
				printf("\n\r%c", *parameter);
				printf("th module: ");
				printf("%d",Voltage_GetModuleMillivoltage(*parameter));
				printf("\n\r");
			}
		}
		else if (strcmp(function,"safe") == 0){		// Check SafetyStatus
			printf("\n\r");
			SafetyStatus voltage = Voltage_IsSafe();
			switch(voltage) {
				case SAFE: 
					printf("SAFE");
					break;
				case DANGER: 
					printf("DANGER");
					break;
				case OVERVOLTAGE:
					printf("OVERVOLTAGE");
					break;
				case UNDERVOLTAGE: 
					printf("UNDERVOLTAGE");
					break;
				default:
					break;
			}
			printf("\n\r");
		}
		else if (strcmp(function,"total") == 0) {			// Total pack voltage
			printf("\n\rTotal voltage: ");
			printf("%d",Voltage_GetTotalPackVoltage());
			printf("\n\r");
		}					
  }
	// Current commands
	else if (strcmp(measurement,"current") == 0){
		if (strcmp(function,"safe") == 0) {		// Check SafetyStatus
			printf("\n\r");
			SafetyStatus current = Current_IsSafe();
			switch(current){
				case SAFE:
					printf("SAFE");
					break;
				case DANGER: 
					printf("DANGER");
					break;
				default:
					break;
			}
			printf("\n\r");			
		}
		else if (strcmp(function,"charging") == 0){
			printf("\n\r");
			uint8_t charge = Current_IsCharging();
			switch(charge){
				case 0: 
					printf("DISCHARGING");
					break;
				case 1:
					printf("CHARGING");
					break;
				default:
					break;
			}
			printf("\n\r");			
		}
		else if (strcmp(function,"high") == 0){			// High precision current reading
			printf("\n\r");		
			printf("%d",Current_GetHighPrecReading());
			printf("\n\r");			
		}
		else if (strcmp(function,"low") == 0){		// Low precision current reading
			printf("\n\r");	
			printf("%d",Current_GetLowPrecReading());
			printf("\n\r");			
		}
  }
	// Temperature commands
	else if (strcmp(measurement,"temp") == 0){
		if (strcmp(function,"average") == 0){		// Average pack temperature
			printf("\n\r");		
			printf("%d",Temperature_GetTotalPackAvgTemperature());
			printf("\n\r");			
		}
		else if (strcmp(function,"charge") == 0){		// Set charge state
			if(parameter != NULL){
				Temperature_SetChargeState(*parameter);
				printf("%d",*parameter);	
				printf("\n\r");
			}else{
				printf("Please provide a charge state (1 for charging, 0 for discharging)");
			}
		}
		else if (strcmp(function,"check") == 0){		// Get module temperatures
			if (parameter == NULL){			
				for (int i = 0; i<12; i++){
					printf("%d",Temperature_GetModuleTemperature(i));
					printf("\n\r");
					}
				}else { 
					printf("%d",Temperature_GetModuleTemperature(*parameter));
					printf("\n\r");		
				}
		}
	}else{
		printf("Invalid measurement. Enter a valid measurement and function");
	}
	*/
}