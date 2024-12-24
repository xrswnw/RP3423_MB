#include "AnyID_Key.h"

#define KEY_DELAY_100MS         10
#define KEY_DELAY_20MS          2

void Key_GetKeyValue(u32 time)
{
    static u8 keyCurrent = KEY_NO_VAL, keyOld = KEY_NO_VAL;
    u8 keyTemp = 0;

    if(time % KEY_DELAY_20MS == 0)                                         //��ʱ��ȡ��ֵ
    {
        keyCurrent = Key_Read();
    }
    else
    {
        if(keyCurrent != KEY_NO_VAL && keyOld == KEY_NO_VAL)                                //�ϴζ�ȡ��ֵ��Ч��ȥ�����ٴζ�ȡ��ֵ
        {
            keyTemp= Key_Read();
            if(keyCurrent == keyTemp)
            {
                keyOld = keyCurrent;
            }
        }
        else if(keyCurrent == KEY_NO_VAL && keyOld != KEY_NO_VAL)                           //�ȴ���������
        {
            g_nKeyValue = keyOld;
            keyOld = 0;
        }
    }
}

