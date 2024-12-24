#include "AnyID_ST25R391x.h"

ST25R3916_FRAME g_sSt25r3916Frame = {0};

void St25r3916_WriteCmd(u8 cmd)
{
    cmd &= (~ST25R3916_MODE_MSK);
    cmd |= ST25R3916_MODE_CMD;
    
    St25r3916_CsLow();
    
    St25r3916_WriteByte(cmd);
    
    St25r3916_CsHigh();
}

u8 St25r3916_ReadReg(u8 addr)
{
    u8 reg = 0;
    
    St25r3916_CsLow();
    
    if(addr & ST25R3916_SPACE_B)
    {
        addr &= (~ST25R3916_SPACE_B);
        St25r3916_WriteByte(ST25R3916_CMD_SPACE_B_ACCESS);
    }
    St25r3916_WriteByte(addr | ST25R3916_MODE_READ);
    reg = St25r3916_ReadByte();
    
    St25r3916_CsHigh();
    
    return reg;
}

void St25r3916_SetRegBits(u8 addr, u8 msk, u8 regBits)
{
    u8 reg = 0;
    
    reg = St25r3916_ReadReg(addr);
    reg &= (~msk);
    reg |= regBits;
    St25r3916_WriteReg(addr, reg);
}

void St25r3916_ClearRegBits(u8 addr, u8 msk)
{
    u8 reg = 0;
    
    reg = St25r3916_ReadReg(addr);
    reg &= (~msk);
    St25r3916_WriteReg(addr, reg);
}

BOOL St25r3916_CheckRegBits(u8 addr, u8 msk, u8 checkBits)
{
    u8 reg = 0;
    
    reg = St25r3916_ReadReg(addr);
    reg &= msk;
    
    return (BOOL)(reg == checkBits);
}

void St25r3916_WriteReg(u8 addr, u8 reg)
{
    St25r3916_CsLow();
    
     if(addr & ST25R3916_SPACE_B)
    {
        addr &= (~ST25R3916_SPACE_B);
        St25r3916_WriteByte(ST25R3916_CMD_SPACE_B_ACCESS);
    }
    St25r3916_WriteByte(addr | ST25R3916_MODE_WRITE);
    St25r3916_WriteByte(reg);
    
    St25r3916_CsHigh();
}

void St25r3916_ReadRegs(u8 addr, u8 *pRegs, u16 num)
{
    u16 i = 0;
    
    St25r3916_CsLow();
    
    if(addr & ST25R3916_SPACE_B)
    {
        addr &= (~ST25R3916_SPACE_B);
        St25r3916_WriteByte(ST25R3916_CMD_SPACE_B_ACCESS);
    }
    St25r3916_WriteByte(addr | ST25R3916_MODE_READ);
    for(i = 0; i < num; i++)
    {
        pRegs[i] = St25r3916_ReadByte();
    }
    
    St25r3916_CsHigh();
}

void St25r3916_WriteRegs(u8 addr, u8 *pRegs, u16 num)
{
    u16 i = 0;
    
    St25r3916_CsLow();
    
    if(addr & ST25R3916_SPACE_B)
    {
        addr &= (~ST25R3916_SPACE_B);
        St25r3916_WriteByte(ST25R3916_CMD_SPACE_B_ACCESS);
    }
    St25r3916_WriteByte(addr | ST25R3916_MODE_WRITE);
    for(i = 0; i < num; i++)
    {
        St25r3916_WriteByte(pRegs[i]);
    }
    
    St25r3916_CsHigh();
}

void St25r3916_ReadFifo(u8 *pBuffer, u16 len)
{
    u16 i = 0;
    u8 reg = 0;
    St25r3916_CsLow();
    
    St25r3916_WriteByte(ST25R3916_MODE_RFIFO);
    for(i = 0; i < len; i++)
    {
        reg = St25r3916_ReadByte();
        pBuffer[i] = reg;
    }
    
    St25r3916_CsHigh();
}

void St25r3916_WriteFifo(u8 *pBuffer, u16 len)
{
    u16 i = 0;
    
    St25r3916_CsLow();
    
    St25r3916_WriteByte(ST25R3916_MODE_WFIFO);
    for(i = 0; i < len; i++)
    {
        St25r3916_WriteByte(pBuffer[i]);
    }
    
    St25r3916_CsHigh();
}

BOOL St25r3916_Init(void)
{
    BOOL b = FALSE;
    u8 reg = 0;
    u32 irqReg = 0;
    u16 delay = ST25R3916_TOUT_OSC_STABLE;
    
    reg = St25r3916_ReadReg(ST25R3916_REG_IC_IDENTITY);
    
    St25r3916_WriteCmd(ST25R3916_CMD_SET_DEFAULT);
    St25r3916_Delayms(10);
    
    
    if((reg & ST25R3916_REG_IC_IDENTITY_ic_type_mask) == ST25R3916_REG_IC_IDENTITY_ic_type_st25r3916)                           //3917���ͺź�3916һ��
    {
        if(St25r3916_CheckRegBits(ST25R3916_REG_OP_CONTROL, ST25R3916_REG_OP_CONTROL_en, ST25R3916_REG_OP_CONTROL_en) == FALSE)
        {
            St25r3916_SetRegBits(ST25R3916_REG_OP_CONTROL, ST25R3916_REG_OP_CONTROL_en, ST25R3916_REG_OP_CONTROL_en);
            while(delay--)
            {
                St25r3916_ReadRegs(ST25R3916_REG_IRQ_MAIN, (u8 *)&irqReg, ST25R3916_INT_REGS_LEN);
                if(irqReg & ST25R3916_IRQ_MASK_OSC)
                {
                    b = TRUE;
                    break;
                }
                else
                {
                    St25r3916_Delayms(1);
                }
            }
        }
        else
        {
            b = TRUE;
        }

        if(b == TRUE)
        {
            irqReg = 0;
            //ֹͣ��������ģʽ
            St25r3916_WriteCmd(ST25R3916_CMD_STOP); 
            //��ֹclk���
            St25r3916_WriteReg(ST25R3916_REG_IO_CONF1, 0x07);
            //Ĭ��5V�����Ը���Ӳ���޸�
            St25r3916_ClearRegBits(ST25R3916_REG_IO_CONF2, ST25R3916_REG_IO_CONF2_sup3V);       //���3V������5V
            //������ͨ��ʹ�ܣ��ֶ�ѡ�����ͨ��
            //��ֹ���ͺͽ��գ���ֹ�ⲿField��⣬��ֹwakeup
            St25r3916_SetRegBits(ST25R3916_REG_OP_CONTROL, ST25R3916_REG_OP_CONTROL_rx_chn | ST25R3916_REG_OP_CONTROL_rx_man, ST25R3916_REG_OP_CONTROL_rx_chn | ST25R3916_REG_OP_CONTROL_rx_man);
            St25r3916_ClearRegBits(ST25R3916_REG_OP_CONTROL, ST25R3916_REG_OP_CONTROL_rx_en | ST25R3916_REG_OP_CONTROL_tx_en | ST25R3916_REG_OP_CONTROL_en_fd_mask | ST25R3916_REG_OP_CONTROL_wu | ST25R3916_REG_OP_CONTROL_rx_chn | ST25R3916_REG_OP_CONTROL_rx_man);
            //��ֹ�ж�
            irqReg = ST25R3916_IRQ_MASK_ALL;
            St25r3916_WriteRegs(ST25R3916_REG_IRQ_MASK_MAIN, (u8 *)&irqReg, ST25R3916_INT_REGS_LEN);
            //RFO�������裬0x02----4.1ŷķ---Demo Ĭ��
            St25r3916_SetRegBits(ST25R3916_REG_TX_DRIVER, ST25R3916_REG_TX_DRIVER_d_res_mask, ST25R3916_REG_TX_DRIVER_d_res1);
            //regulator vol mpsv===VDDRF
            St25r3916_SetRegBits(ST25R3916_REG_REGULATOR_CONTROL, ST25R3916_REG_REGULATOR_CONTROL_mpsv_mask, ST25R3916_REG_REGULATOR_CONTROL_mpsv_vdd_rf);
            //additionally start at end of rx
            St25r3916_WriteReg(ST25R3916_REG_TIMER_EMV_CONTROL, ST25R3916_REG_TIMER_EMV_CONTROL_gptc0);
        }
    }
    
    return b;
}
