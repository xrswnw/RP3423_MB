#ifndef _ANYID_SYSTICK_HL_
#define _ANYID_SYSTICK_HL_


#include "AnyID_Config.h"


extern vu32 g_nSysTick;
extern vs32 g_nSysDelay;

#ifdef _ANYID_STM32_
    #if (SYS_CLOCK_FREQ == 72000000)
        #define STICK_1MS       9000            //72M��ϵͳʱ��
    #elif (SYS_CLOCK_FREQ == 24000000)
        #define STICK_1MS       3000            //24M��ϵͳʱ��
    #endif
#elif _ANYID_PIC18_
    //����ϵͳʱ�ӵĲ�ͬ�����ֵ��Ҫ�޸ģ�������8λ������������ֻ����5ms��systick
    #if (SYS_CLOCK_FREQ == 32000000)
        #define STICK_5MS       156
    #endif
#endif


void STick_InitSysTick(void);
void STick_StopSysTick(void);
#define STick_SetDelayTime(delay)          (g_nSysDelay = delay) //��һ��TickΪ��λ
#define STick_IsDelayTimeOver()            (g_nSysDelay <= 0)


#ifdef _ANYID_STM32_

    #define STick_StartSysTick()           SysTick_CounterCmd(SysTick_Counter_Enable)
    #define STick_EnableInt(bEnable)       SysTick_ITConfig(bEnable)

#elif _ANYID_PIC18_

    #define STick_StartSysTick()           (T2CONbits.TMR2ON = 1)
    void STick_EnableInt(FunctionalState bEnable);
#endif



#ifdef _ANYID_STM32_

    #if (defined(_ANYID_STM32_BCTRL_) | defined(_ANYID_STM32_BLDC_))
        #define STICK_TIME_MS               50
    #else
        #define STICK_TIME_MS               10
    #endif

#elif _ANYID_PIC18_

    #define STICK_TIME_MS                   5

#endif



#define STICK_1S                            (1000 / STICK_TIME_MS)






#endif
