#ifndef _ANYID_CANFESTIVAL_FLASH_H
#define _ANYID_CANFESTIVAL_FLASH_H

#include "AnyID_CanFestival_Config.h"
#include "AnyID_CanFestival_FlashM25.h"


#define FLASH_STD_DATA				0x5A
#define FLASH_TEST_ADDR             0x00000000


#define FLASH_RECORD_UID_LEN            8
#define FLASH_RECORD_DATE_LEN           16

#define FLASH_RECORD_LENTH              0x30

typedef struct flashRecord{
    u8 rfPort;
    u8 mxPort;
    u8 amarFlag;
    u8 amarValue;
    u8 op;
    u8 opAddr;
    u8 opLenth;
    u8 rfu;
    u8 uid[FLASH_RECORD_UID_LEN];
    u8 data[FLASH_RECORD_DATE_LEN];
    u32 unixStamp;
}FLASH_RECORD;



BOOL Flash_ChkErr();
#endif

