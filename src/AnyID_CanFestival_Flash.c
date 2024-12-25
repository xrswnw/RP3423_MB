#include "AnyID_CanFestival_Flash.h"

u8 g_nFlashPublicBuffer[FLASE_PAGE_SIZE];
BOOL Flash_ChkErr()
{
    BOOL bOk = FALSE;
    u16 i = 0;
    
    if(Flash_EraseSector(FLASH_TEST_ADDR))
    {
        Flash_Delayms(120);             //典型值45ms，最大400ms，失败可拉大重试
        memset(g_nFlashPublicBuffer, FLASH_STD_DATA, FLASE_PAGE_SIZE);
        if(Flash_WriteBuffer(FLASH_TEST_ADDR, FLASE_PAGE_SIZE, g_nFlashPublicBuffer))
        {
            memset(g_nFlashPublicBuffer, 0x00, FLASE_PAGE_SIZE);
            if(Flash_ReadBuffer(FLASH_TEST_ADDR, FLASE_PAGE_SIZE, g_nFlashPublicBuffer))
            {
                for(i = 0; i < FLASE_PAGE_SIZE; i++)
                {
                    if(g_nFlashPublicBuffer[i] != FLASH_STD_DATA)
                    {
                        break;
                    }
                }
            }
        }
    }
    
    if(i == FLASE_PAGE_SIZE)
    {
        bOk = TRUE;
    }

    return bOk;
}