#include BSP_Strobelight.h
#include <stdint.h>
#include <stdlib.h>

static const char* file = "BSP/Simulator/DataGeneration/Data/Strobe.csv"
int StrobeStatus = 0;

/**
 * @brief   Create text file if none exists (Simulator)
 * @param   None
 * @return  None
 */
void BSP_Strobe_Init(void) {
	FILE* fp = fopen(file, "w+");
	fprintf(fp, "0");
	fclose(fp);
}

/**
 * @brief   Turns on Strobe (pulse once per second)
 * @param   None
 * @return  None
 */
void BSP_Strobe_On(void) {
    StrobeStatus = 1;
    FILE* fp = fopen(file, "w");
    fprintf(fp, "%d", StrobeStatus);
    fclose(fp);
}

/**
 * @brief   Turns off Strobe
 * @param   None
 * @return  None
 */
void BSP_Strobe_Off(void) {
    StrobeStatus = 0;
    FILE* fp = fopen(file, "w");
    fprintf(fp, "%d", StrobeStatus);
    fclose(fp);
}
