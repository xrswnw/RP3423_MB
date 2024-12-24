#include "AnyID_CanFestival_SysCfg.h"

u32 g_nSysState = SYS_STAT_IDLE;
u32 g_nLedDelayTime = 0;

u32 g_nChkComTime = 0;

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
                           RCC_APB1Periph_WWDG , state);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 |
                        RCC_AHBPeriph_DMA2, state);
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

    Lan_InitInterface(LAN_BAUDRARE);
    Lan_ConfigInt();
    Lan_EnableInt(ENABLE, DISABLE);
    
    
    Wifi_InitInterface(WIFI_BAUDRARE);
    Wifi_ConfigInt();
    Wifi_InitTxDma(g_sWifiTxInfo.txFrame, WIFI_FRAME_MAX_LENTH);
    Wifi_InitRxDma(g_sWifiRcvFrame.buffer, UART_BUFFER_MAX_LEN);
    
    
    Uart_Init(UART_BAUDRARE);
        
    Periph_InitInterface();
	Device_Init();
	Sys_Delayms(50);
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


void Sys_Rs485Task(void)
{


}

void Sys_LanTask(void)
{
    if(USART_GetFlagStatus(LAN_PORT, USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
    {
        USART_ClearFlag(LAN_PORT, USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
        Lan_InitInterface(LAN_BAUDRARE);
        Lan_ConfigInt();
        Lan_EnableInt(ENABLE, DISABLE);
    }
    if(Uart_IsRcvFrame(g_sLanRcvFrame))
    {
        memcpy(&g_sLanRcvTempFrame, &g_sLanRcvFrame, sizeof(g_sLanRcvFrame));
        Uart_ResetFrame(&g_sLanRcvFrame);
        
        
        
        Wifi_EnableRxDma();
        Wifi_WriteBuffer(g_sLanRcvTempFrame.buffer, g_sLanRcvTempFrame.length);
      /*  if(g_sLanRcvTempFrame.length >= UART_FRAME_MIN_LEN)
        {
            u16 crc1 = 0, crc2 = 0;
            
            crc1 = Uart_GetFrameCrc(g_sLanRcvTempFrame.buffer, g_sLanRcvTempFrame.length);
            crc2 = a_GetCrc(g_sLanRcvTempFrame.buffer + UART_FRAME_POS_LEN, g_sLanRcvTempFrame.length - 4);

            if(crc1 == crc2)
            {
                u16 txLen = 0;
                
                txLen = Reader_ProcessUartFrame(g_sLanRcvTempFrame.buffer, g_sLanRcvTempFrame.length);
                if(txLen > 0)
                {
                    a_SetStateBit(g_nSysState, SYS_STAT_RS485_TX);
                }
            }
        }
*/
    }
    
    if(a_CheckStateBit(g_nSysState, SYS_STAT_RS485_TX))
    {
        a_ClearStateBit(g_nSysState, SYS_STAT_RS485_TX);
        Lan_WriteBuffer(g_sReaderRspFrame.buffer, g_sReaderRspFrame.len);

    }

}


void Sys_DeviceTask(void)
{
    if(a_CheckStateBit(g_nSysState, SYS_STAT_CAN_AUTO_TASK))
    {
        Device_CanAutoTask();
        a_ClearStateBit(g_nSysState, SYS_STAT_CAN_AUTO_TASK);
    }
    
    Device_OpTask();
}



void Sys_TaskCreat()
{
    portENTER_CRITICAL();
    //Enter Ceitical

    #if FREE_RTOS_ENABLE_ERR_LOG
        Log_Init();
    #endif
    
    #if configUSE_STATS_FORMATTING_FUNCTIONS
        xTaskCreate(Sys_RunTime, "Sys_RunTime", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL);
    #endif

    xTaskCreate(Device_CanRxDispatch, "Device_CanRxDispatch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Device_CanTxDispatch, "Device_CanTxDispatch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    xTaskCreate(Device_UartRxDispatch, "Device_UartRxDispatch", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Device_UartTxDispatch, "Device_UartTxDispatch", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    xTaskCreate(Device_MainDispatch, "Device_MainDispatch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Device_HeartDispatch, "Device_HeartDispatch", configMINIMAL_STACK_SIZE / 4, NULL, tskIDLE_PRIORITY + 1, NULL);
    

    vTaskDelete(NULL);
    
    //Exti Ceitical
    portEXIT_CRITICAL();
}

//监控各个任务所占时间，调试使用
//监控各个任务的堆栈使用情况，尽量保持再分配空间的2/3内，否则溢出会导致任务切换错误
void Sys_RunTime(void *p)
{
    char frame[128] = {0};          //堆栈区间过小可能导致越界
    while(1)
    {
        vTaskGetRunTimeStats(frame);
        Log_WriteBuffer((u8 *)frame, strlen(frame));
        vTaskDelay(pdMS_TO_TICKS( 1000  + 900 + 3));
    }
}