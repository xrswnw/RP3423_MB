#ifndef _ANYID_CANFESTIVAL_SYSCFG_H
#define _ANYID_CANFESTIVAL_SYSCFG_H

#include "AnyID_CanFestival_Device.h"

extern u32 g_nSysState;
#define SYS_NULL_TICK                   0

#define SYS_STAT_IDLE                   0x00000001	        //
#define SYS_STAT_RUNLED                 0x00000002
#define SYS_STAT_ALARMLED               0x00000004
#define SYS_STAT_ALARMDLY				0x00000008
#define SYS_STAT_ALARMOFF				0x00000010
#define SYS_STAT_CAN_ERR				0x00000020
#define SYS_STAT_RS485_TX               0x00000100



#define SYS_STAT_CAN_AUTO_TASK          0x80000000



void Sys_Delayms(u32 n);
void Sys_CfgClock(void);
void Sys_CfgPeriphClk(FunctionalState state);
void Sys_CfgNVIC(void);

void Sys_CtrlIOInit(void);
void Sys_Rs485Task(void);
void Sys_Init(void);
void Sys_LedTask(void);
void Sys_UartTask(void);
void Sys_DeviceTask();
void Sys_CanTsak();
void Sys_LanTask(void);

void Sys_DeviceTask(void);
void Sys_RunTime(void *p);

void Sys_TaskCreat();
#endif
