#include "AnyID_CanFestival_RTC.h"

RTC_TIME g_sRtcTime = {0, 1, 1, 0, 0, 0};
u32 g_nRtcUnixStamp = 0;
RTC_REALTIME g_nRtcRealTime = {0};
void RTC_Init(void)
{
    // Allow access to BKP Domain
    PWR_BackupAccessCmd(ENABLE);

    // Reset Backup Domain
    BKP_DeInit();

    // Enable LSE
    RCC_LSEConfig(RCC_LSE_ON);
   // RCC_LSICmd(ENABLE);

    // Wait till LSI is ready
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    //while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    // Select LSI as RTC Clock Source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    // Enable RTC Clock
    RCC_RTCCLKCmd(ENABLE);

    // Wait for RTC registers synchronization
    RTC_WaitForSynchro();

    // Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();

    // Disable the RTC Second
    RTC_ITConfig(RTC_IT_SEC, DISABLE);

    // Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();

    // Set RTC prescaler: set RTC period to 1sec

    RTC_SetPrescaler(32767);

    // Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();
}

void RTC_ConfigInt(FunctionalState state)
{
    RTC_WaitForLastTask();             //等待写操作完成
    RTC_WaitForSynchro();//等待RTC寄存器和APB时钟同步
    RTC_WaitForLastTask();             //等待写操作完成
    RTC_ITConfig(RTC_IT_SEC, state);   //使能RTC秒中断
    RTC_WaitForLastTask();             //等待写操作完成
}

void RTC_Check(void)
{
    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        //重新配置RTC
        RTC_Init();
        //配置完成后，向后备寄存器中写特殊字符0xA5A5
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);

        RTC_SetTime(0);
    }
    else
    {
        //清除RCC中复位标志
        RCC_ClearFlag();

        RCC_LSEConfig(RCC_LSE_ON);
        //RCC_LSEConfig(RCC_LSE_ON);

        // Wait till LSI is ready
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

        // Select LSI as RTC Clock Source
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        // Enable RTC Clock
        RCC_RTCCLKCmd(ENABLE);

        // Wait for RTC registers synchronization
        RTC_WaitForSynchro();
    }
}

const u32 g_nRtcDayPerMonthLeapYear[12] = {RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH29_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND};
const u32 g_nRtcDayPerMonthNormalYear[12] = {RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH28_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND};
void RTC_SetTime(u32 count)
{
    PWR_BackupAccessCmd(ENABLE);
    //Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();
    //Change the current time
    RTC_SetCounter(count);
    //Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();
}

u32 RTC_CalCount(RTC_TIME *pRtcTime)
{
    u32 count = 0;
    u8 i = 0;

    //year
    for(i = 0; i < pRtcTime->year; i++)
    {
        if(RTC_IsLeapYear(i))
        {
            count += RTC_YEAR366_PER_SECOND;
        }
        else
        {
            count += RTC_YEAR365_PER_SECOND;
        }
    }

    //month
    if(RTC_IsLeapYear(pRtcTime->year))
    {
        for(i = 0; i < pRtcTime->month - 1; i++)
        {
            count += g_nRtcDayPerMonthLeapYear[i];
        }
    }
    else
    {
        for(i = 0; i < pRtcTime->month - 1; i++)
        {
            count += g_nRtcDayPerMonthNormalYear[i];
        }
    }

    //day
    count += (pRtcTime->day - 1) * RTC_DAY_PER_SECOND;

    //hour
    count += (pRtcTime->hour) * RTC_HOUR_PER_SECOND;

    //minute
    count += (pRtcTime->minute) * RTC_MINUTE_PER_SECOND;

    //second
    count += pRtcTime->second;

    return count;
}

void RTC_FormatTime(u32 count)
{
    u8 y = 0, mth = 0, d = 0, h = 0, m = 0, s = 0;

    //year
    for(y = 0; y < 100; y++)
    {
        if(RTC_IsLeapYear(y))
        {
            if(count >= RTC_YEAR366_PER_SECOND)
            {
                count -= RTC_YEAR366_PER_SECOND;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(count >= RTC_YEAR365_PER_SECOND)
            {
                count -= RTC_YEAR365_PER_SECOND;
            }
            else
            {
                break;
            }
        }
    }

    //month
    if(RTC_IsLeapYear(y))
    {
        for(mth = 0; mth < 12; mth++)
        {
            if(count >= g_nRtcDayPerMonthLeapYear[mth])
            {
                count -= g_nRtcDayPerMonthLeapYear[mth];
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for(mth = 0; mth < 12; mth++)
        {
            if(count >= g_nRtcDayPerMonthNormalYear[mth])
            {
                count -= g_nRtcDayPerMonthNormalYear[mth];
            }
            else
            {
                break;
            }
        }
    }
    mth++;

    //day
    d = count / RTC_DAY_PER_SECOND;
    count -= d * RTC_DAY_PER_SECOND;
    d++;

    //hour
    h = count / RTC_HOUR_PER_SECOND;
    count -= h * RTC_HOUR_PER_SECOND;

    //minute
    m = count / RTC_MINUTE_PER_SECOND;
    count -= m * RTC_MINUTE_PER_SECOND;

    //second
    s = count;

    g_sRtcTime.year = y;
    g_sRtcTime.month = mth;
    g_sRtcTime.day = d;

    g_sRtcTime.hour = h;
    g_sRtcTime.minute = m;
    g_sRtcTime.second = s;
}

void Rtc_ConfigAlarmInt(FunctionalState state)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_FREERTOS_UART;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


static unsigned int g_nRtcMonth[12]={
        /*01月*/31, 
        /*02月*/28, 
        /*03月*/31, 
        /*04月*/30, 
        /*05月*/31, 
        /*06月*/30, 
        /*07月*/31, 
        /*08月*/31, 
        /*09月*/30, 
        /*10月*/31, 
        /*11月*/30, 
        /*12月*/31 };

//UNIX转为UTC 已进行时区转换 北京时间UTC+8
void Rtc_UnixStampToRealTime(u32 stamp, RTC_REALTIME *realTime)
{
    u32 days = 0, leapYearCount = 0; 

    realTime->second = stamp % RTC_REAL_TIME_MINUTE_HAVE_SCEOND;                    //获得秒 
    stamp /= RTC_REAL_TIME_MINUTE_HAVE_SCEOND; 
    
    realTime->minute = stamp % RTC_REAL_TIME_HOUR_HAVE_MINUTE;                      //获得分 
    stamp += 8 * RTC_REAL_TIME_HOUR_HAVE_MINUTE ;                                   //时区矫正 转为UTC+8 bylzs
    stamp /= RTC_REAL_TIME_HOUR_HAVE_MINUTE; 
    
    realTime->hour = stamp % RTC_REAL_TIME_DAY_HAVE_HOUR;                            //获得时 
    days = stamp / RTC_REAL_TIME_DAY_HAVE_HOUR;                                  //获得总天数 
    
    leapYearCount = (days + RTC_REAL_TIME_YEAR_HAVE_DAY) / 1461;                //过去了多少个闰年(4年一闰) 
    
    if(((days + RTC_REAL_TIME_YEAR_HAVE_DAY) % 1461) == 0) 
    {//闰年的最后1天 
        realTime->year = RTC_REAL_TIME_UNIX_BASE_YEAR + (days / 366);               //获得年 
        realTime->month = 12;                                                       //调整月 
        realTime->day = 31;
        return; 
    } 
    
    days -= leapYearCount; 
    realTime->year = 1970 + (days / RTC_REAL_TIME_YEAR_HAVE_DAY);                   //获得年 
    days %= RTC_REAL_TIME_YEAR_HAVE_DAY;                                            //今年的第几天 
    days = 01 + days;                                                               //1日开始 
    
    if((realTime->year % 4) == 0) 
    { 
        if(days > 60)
        {
            --days;
        }                                                                           //闰年调整 
        else 
        { 
            if(days == 60) 
            { 
                realTime->month = 2; 
                realTime->day = 29; 
                return; 
            } 
        } 
    } 
    
    for(realTime->month = 0; g_nRtcMonth[realTime->month] < days; realTime->month++) 
    { 
        days -= g_nRtcMonth[realTime->month]; 
    } 
    
    ++realTime->month;                                                              //调整月 
    realTime->day = days;                                                           //获得日 
}

