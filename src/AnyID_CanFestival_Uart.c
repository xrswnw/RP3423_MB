#include "AnyID_CanFestival_Uart.h"

QueueHandle_t g_hUartRxQueue = NULL;
QueueHandle_t g_hUartTxQueue = NULL;
QueueHandle_t g_hUartComStatus = NULL;
BOOL Uart_Init(u32 bud)
{
    BOOL bOk = FALSE;
    
    Uart_InitInterface(bud);
    Uart_ConfigInt();
    Uart_InitTxDma(g_sUartTxFrame.buffer, UART_BUFFER_MAX_LEN);
    Uart_InitRxDma(g_sUartRxFrame.buffer, UART_BUFFER_MAX_LEN);
    Uart_EnableRxDma();
    
    g_hUartRxQueue = xQueueCreate(UART_RX_FRAME_MAX_LEN, sizeof(UART_RCVFRAME));
    g_hUartTxQueue = xQueueCreate(UART_TX_FRAME_MAX_LEN, sizeof(UART_TXFRAME)); 
    g_hUartComStatus = xSemaphoreCreateBinary();
    
    if(g_hUartRxQueue && g_hUartTxQueue && g_hUartComStatus)
    {
        bOk = TRUE;
    }
    
    return bOk;
}