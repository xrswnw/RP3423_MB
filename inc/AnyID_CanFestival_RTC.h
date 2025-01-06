#ifndef _ANYID_CANFESTIVAL_RTC_
#define _ANYID_CANFESTIVAL_RTC_

#include "AnyID_CanFestival_Config.h"

#define RTC_BUF_SIZE            6
typedef struct rtc_time{
    u32 counter;
    u8 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
}RTC_TIME;
extern RTC_TIME g_sRtcTime;

void RTC_Init(void);
void RTC_Check(void);
void RTC_SetTime(u32 count);
void RTC_FormatTime(u32 count);
u32 RTC_CalCount(RTC_TIME *pRtcTime);
void RTC_ConfigInt(FunctionalState state);
void Rtc_ConfigAlarmInt(FunctionalState state);
//If n Mod 400 = 0 Or (n Mod 4 = 0 And n Mod 100 <> 0)
#define RTC_MINUTE_PER_SECOND       60
#define RTC_HOUR_PER_SECOND         3600
#define RTC_DAY_PER_SECOND          86400
#define RTC_MONTH28_PER_SECOND      2419200
#define RTC_MONTH29_PER_SECOND      2505600
#define RTC_MONTH30_PER_SECOND      2592000
#define RTC_MONTH31_PER_SECOND      2678400
#define RTC_YEAR365_PER_SECOND      31536000
#define RTC_YEAR366_PER_SECOND      31622400


//时间是00~99年，所以闰年的计算方法特殊化
#define RTC_IsLeapYear(y)           ((y) % 4 == 0)
extern const u32 g_nRtcDayPerMonthLeapYear[12];
extern const u32 g_nRtcDayPerMonthNormalYear[12];



typedef struct rtcRealTime{
  int year; 
  int month;  
  int day;  
  int hour; 
  int minute;  
  int second;
}RTC_REALTIME ;
 
#define RTC_REAL_TIME_UNIX_BASE_YEAR                    1970

#define RTC_REAL_TIME_MINUTE_HAVE_SCEOND                60
#define RTC_REAL_TIME_HOUR_HAVE_MINUTE                  60
#define RTC_REAL_TIME_DAY_HAVE_HOUR                     24
#define RTC_REAL_TIME_YEAR_HAVE_DAY                     365

#define RTC_REAL_TIME_VALUE_SCEOND_COUNT              1
#define RTC_REAL_TIME_VALUE_MINUTE_COUNT              RTC_REAL_TIME_VALUE_SCEOND * RTC_REAL_TIME_MINUTE_HAVE_SCEOND
#define RTC_REAL_TIME_VALUE_HOUR_COUNT                RTC_REAL_TIME_VALUE_MINUTE * RTC_REAL_TIME_HOUR_HAVE_MINUTE
#define RTC_REAL_TIME_VALUE_DAY_COUNT                 RTC_REAL_TIME_VALUE_HOUR * RTC_REAL_TIME_DAY_HAVE_HOUR
#define RTC_REAL_TIME_VALUE_YRAR_COUNT                RTC_REAL_TIME_VALUE_HOUR * RTC_REAL_TIME_YEAR_HAVE_DAY

extern u32 g_nRtcUnixStamp;
extern RTC_REALTIME g_nRtcRealTime;
void Rtc_UnixStampToRealTime(u32 stamp, RTC_REALTIME *realTime);
#endif
