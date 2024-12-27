#include "AnyID_CanFestival_SysCfg.h"



int main(void)
{
    Sys_Init();
    
    if(xTaskCreate(Sys_TaskCreat, "Sys_TaskCreat", configMINIMAL_STACK_SIZE, NULL, 1, NULL) != NULL)
    {
        vTaskStartScheduler();              //Dispatch start
    }

    return 0;                               //err
}

