#include "common.h"
#include "config.h"
#include "BSP_Timer.h"


int main(void){
    uint32_t test;
    uint32_t delay = 150000000;
    uint32_t time = 40;        
    
    BSP_Timer_Init();
    BSP_Timer_Start();
    uint32_t freq = BSP_Timer_GetRunFreq();
    printf("Timer frequency: %d\n\r", freq);
    
    while(1){
        test = BSP_Timer_GetTicksElapsed();
        printf("Ticks elapsed : %d\n\r", test);
        freq = BSP_Timer_GetRunFreq();
        printf("Timer frequency: %d\n\r", freq);  
        while(time){                        //this is a delay to prevent the elapsed time being negligible
            delay = 150000000;
            while(delay){
                delay--;
            }
            time--;
        }
        time = 40;
    }
   
}