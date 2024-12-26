#include "AnyID_CanFestival_SysCfg.h"

u32 g_nSysState = SYS_STAT_IDLE;

#if SYS_ENABLE_WDT
    #define SYS_ENABLE_TEST         0
#else
    #define SYS_ENABLE_TEST         1
#endif

void Sys_Delayms(u32 n)
{
    //72MHZ
    n *= 0x6000;
    n++;
    while(n--);
}

void Sys_CfgClock(void)
{
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();
    //Enable HSE
    RCC_HSEConfig(RCC_HSE_ON);

    //Wait till HSE is ready
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS)
    {

        //HCLK = SYSCLK = 72.0M
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //PCLK2 = HCLK = 72.0M
        RCC_PCLK2Config(RCC_HCLK_Div1);

        //PCLK1 = HCLK/2 = 33.9M
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //ADCCLK = PCLK2/2
        RCC_ADCCLKConfig(RCC_PCLK2_Div8);

        // Select USBCLK source 72 / 1.5 = 48M
        RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

        //Flash 2 wait state
        FLASH_SetLatency(FLASH_Latency_2);

        //Enable Prefetch Buffer
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_10);    //PLL在最后设置

        //Enable PLL
        RCC_PLLCmd(ENABLE);

        //Wait till PLL is ready
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        //Select PLL as system clock source
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        //Wait till PLL is used as system clock source
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
}

void Sys_CfgPeriphClk(FunctionalState state)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_AFIO |
                           RCC_APB2Periph_USART1 |
                           RCC_APB2Periph_SPI1 |
                           RCC_APB2Periph_TIM1 |
                           RCC_APB2Periph_GPIOD, state);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 |
                           RCC_APB1Periph_TIM2 |
                           RCC_APB1Periph_PWR  |
                           RCC_APB1Periph_BKP  |
                           RCC_APB1Periph_USART2 |
                           RCC_APB1Periph_UART4 |
                           RCC_APB1Periph_USART3 |
                           RCC_APB1Periph_TIM4 |
                           RCC_APB1Periph_WWDG , state);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, state);
}

void Sys_CfgNVIC(void)
{
    //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
    //Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  //VECT_TAB_FLASH
    //Set the Vector Table base location at 0x08000000
#ifdef _ANYID_BOOT_GD32_
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
#endif

    //Configure the Priority Group to 2 bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}


void Sys_CtrlIOInit(void)
{
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

}

void Sys_Init(void)
{
    Sys_CfgClock();
    Sys_CfgNVIC();
    Sys_CfgPeriphClk(ENABLE);
		
	#if SYS_ENABLE_WDT
    WDG_InitIWDG();
	#endif
    Sys_CtrlIOInit();
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );			                //将所有优先级位都指定为抢占优先级位， 不保留任何优先级位作为子优先级位，方便调度管理 否则报错
    
    Sys_CtrlIOInit();
	RCC_ClocksTypeDef g_sSysclockFrep; RCC_GetClocksFreq(&g_sSysclockFrep);    //查看时钟频率
    
	FRam_InitInterface();
	Device_ReadDeviceParamenter();

    while(Can_InitInterface(CAN_PSC_BUD_500K))
	{			
        Periph_Ledon(1);
		Sys_Delayms(10);
        Periph_Ledoff(1);
		Sys_Delayms(10);
		#if SYS_ENABLE_WDT
    	WDG_FeedIWDog();
		#endif
	}

    Wifi_Init(WIFI_BAUDRARE);
    Wifi_Connect(&g_sWifiOpRegs, &g_sDeviceParams.wifiParams);
    Uart_Init(UART_BAUDRARE);
    
    Lan_Init(UART_BAUDRARE);
    
    Flash_InitInterface();
    if(Flash_ChkErr() == FALSE)
    {   //FLASH问题，报警指示
    
    
    }
        
    Periph_InitInterface();
    
	Device_Init();
    
	#if SYS_ENABLE_WDT
	WDG_FeedIWDog();
	#endif
			
    Sys_EnableInt();
}

u32 g_nSysTotalTime = 0;
void Sys_LedTask(void)
{		
	if(a_CheckStateBit(g_nSysState, SYS_STAT_RUNLED))
	{	
		a_ClearStateBit(g_nSysState, SYS_STAT_RUNLED);

        #if SYS_ENABLE_WDT
        WDG_FeedIWDog();
        #endif
	}
}

TaskHandle_t g_hDevice_CanRxDispatch = NULL;
TaskHandle_t g_hDevice_CanTxDispatch = NULL;

TaskHandle_t g_hDevice_UartRxDispatch = NULL;
TaskHandle_t g_hDevice_UartTxDispatch = NULL;

TaskHandle_t g_hDevice_MainDispatch = NULL;
TaskHandle_t g_hDevice_HeartDispatch = NULL;
TaskHandle_t g_hDevice_HLDispatch = NULL;


#if configUSE_STATS_FORMATTING_FUNCTIONS
    TaskHandle_t g_hSys_RunTime = NULL;
    
    //监控各个任务所占时间，调试使用
    //监控各个任务的堆栈使用情况，尽量保持再分配空间的2/3内，否则溢出会导致任务切换错误
    //按时间情况测试，如果队列毒死，可超时时间拉低，但需找到毒死具体原因，尽量减小开销
    void Sys_RunTime(void *p)
    {
        char frame[1024] = {0};          //堆栈区间过小可能导致越界
        while(1)
        {
            
            Log_WriteStr("\r\nThe task run time :\r\n");
            vTaskGetRunTimeStats(frame);
            
            Log_WriteBuffer((u8 *)frame, strlen(frame));
            
            vTaskDelay(pdMS_TO_TICKS(2000));
            
            Log_WriteStr("\r\nThe task free stack size :\r\n");
              
            sprintf(frame, "%s : Sys_RunTime : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hSys_RunTime), 4 * uxTaskGetStackHighWaterMark(g_hSys_RunTime));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hDevice_CanRxDispatch), 4 * uxTaskGetStackHighWaterMark(g_hDevice_CanRxDispatch));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hDevice_CanTxDispatch), 4 * uxTaskGetStackHighWaterMark(g_hDevice_CanTxDispatch));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hDevice_UartRxDispatch), 4 * uxTaskGetStackHighWaterMark(g_hDevice_UartRxDispatch));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hDevice_UartTxDispatch), 4 * uxTaskGetStackHighWaterMark(g_hDevice_UartTxDispatch));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hWifi_NetInit), 4 * uxTaskGetStackHighWaterMark(g_hWifi_NetInit));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hDevice_MainDispatch), 4 * uxTaskGetStackHighWaterMark(g_hDevice_MainDispatch));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hDevice_HeartDispatch), 4 * uxTaskGetStackHighWaterMark(g_hDevice_HeartDispatch));
            
            sprintf(frame + strlen(frame), "%s : The free stack size is %d byte\r\n",  
                    pcTaskGetName(g_hDevice_HLDispatch), 4 * uxTaskGetStackHighWaterMark(g_hDevice_HLDispatch));
            
            
            Log_WriteBuffer((u8 *)frame, strlen(frame));
            
            vTaskDelay(pdMS_TO_TICKS(2000));
            
            vTaskList(frame);
            
            Log_WriteStr("\r\nThe task state :\r\n");
            
            Log_WriteBuffer((u8 *)frame, strlen(frame));
            
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
#endif

void Sys_TaskCreat()
{
    portENTER_CRITICAL();
    //Enter Ceitical

    #if FREE_RTOS_ENABLE_ERR_LOG
        Log_Init();
    #endif
    
    #if configUSE_STATS_FORMATTING_FUNCTIONS
        xTaskCreate(Sys_RunTime, "Sys_RunTime", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 4, &g_hSys_RunTime);
    #endif

    if((xTaskCreate(Device_CanRxDispatch, "Device_CanRxDispatch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &g_hDevice_CanRxDispatch) == pdPASS) &&
       (xTaskCreate(Device_CanTxDispatch, "Device_CanTxDispatch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &g_hDevice_CanTxDispatch) == pdPASS) &&
       (xTaskCreate(Device_UartRxDispatch, "Device_UartRxDispatch", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, &g_hDevice_UartRxDispatch) == pdPASS) &&
       (xTaskCreate(Device_UartTxDispatch, "Device_UartTxDispatch", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, &g_hDevice_UartTxDispatch) == pdPASS) &&
       (xTaskCreate(Wifi_NetInit, "Wifi_NetInit", configMINIMAL_STACK_SIZE, (void *)&g_sDeviceParams.wifiParams, tskIDLE_PRIORITY + 1, &g_hWifi_NetInit) == pdPASS) &&
       (xTaskCreate(Device_MainDispatch, "Device_MainDispatch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &g_hDevice_MainDispatch) == pdPASS) &&
       (xTaskCreate(Device_HeartDispatch, "Device_HeartDispatch", configMINIMAL_STACK_SIZE / 4, NULL, tskIDLE_PRIORITY + 1, &g_hDevice_HeartDispatch) == pdPASS) &&
       (xTaskCreate(Device_HLDispatch, "Device_HLDispatch", configMINIMAL_STACK_SIZE / 4, NULL, tskIDLE_PRIORITY + 1, &g_hDevice_HLDispatch) == pdPASS))
    {
        vTaskDelete(NULL);
    }
    else
    {   
        while(1);
    }
    //Exti Ceitical
    portEXIT_CRITICAL();
}