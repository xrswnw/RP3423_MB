#include "AnyID_CanFestival_Lan.h"

UART_RCVFRAME g_sLanRxFrame = {0};
UART_TXFRAME g_sLanTxFrame = {0};

BOOL Lan_Init(u32 bud)
{
    Lan_InitInterface(bud);
    Lan_ConfigInt();
    Lan_InitTxDma(g_sLanTxFrame.buffer, UART_BUFFER_MAX_LEN);
    Lan_InitRxDma(g_sLanRxFrame.buffer, UART_BUFFER_MAX_LEN);
    Lan_EnableRxDma();

    return TRUE;
}