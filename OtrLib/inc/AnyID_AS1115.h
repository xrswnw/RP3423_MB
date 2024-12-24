#ifndef _ANYID_AS1115_
#define _ANYID_AS1115_

#include "AnyID_config.h"
#include "AnyID_SI2C_Driver.h"

#define AS1115_FACTORY_ADDR             0x00
#define AS1115_ADDRESS0                 0x00
#define AS1115_ADDRESS1                 0x02
#define AS1115_ADDRESS2                 0x04
#define AS1115_ADDRESS3                 0x06

#define AS1115_ADDR_LED_START           0x01

#define AS1115_ADDR_D0                  0x01
#define AS1115_ADDR_D1                  0x02
#define AS1115_ADDR_D2                  0x03
#define AS1115_ADDR_D3                  0x04
#define AS1115_ADDR_D4                  0x05
#define AS1115_ADDR_D5                  0x06
#define AS1115_ADDR_D6                  0x07
#define AS1115_ADDR_D7                  0x08
#define AS1115_ADDR_DECODE_MOD          0x09
#define AS1115_ADDR_GLOBAL_INTENSITY    0x0A
#define AS1115_ADDR_SCAN_LIMIT          0x0B
#define AS1115_ADDR_SHUT_DOWN           0x0C
#define AS1115_ADDR_SELF_ADRESS         0x2D
#define AS1115_ADDR_FEATURE             0x0E
#define AS1115_ADDR_TEST_MOD            0x0F
#define AS1115_ADDR_D01_INTENSITY       0x10
#define AS1115_ADDR_D23_INTENSITY       0x11
#define AS1115_ADDR_D45_INTENSITY       0x12
#define AS1115_ADDR_D67_INTENSITY       0x13
#define AS1115_ADDR_TEST_D0             0x14
#define AS1115_ADDR_TEST_D1             0x15
#define AS1115_ADDR_TEST_D2             0x16
#define AS1115_ADDR_TEST_D3             0x17
#define AS1115_ADDR_TEST_D4             0x18
#define AS1115_ADDR_TEST_D5             0x19
#define AS1115_ADDR_TEST_D6             0x1A
#define AS1115_ADDR_TEST_D7             0x1B
#define AS1115_ADDR_KEY_A               0x1C
#define AS1115_ADDR_KEY_B               0x1D

//����ģʽ
#define AS1115_DECODE_BHEX(mode) ((u8)(mode))


//ÿ������DIGx�ĵ���������ÿ��DIGx������(��������PWM��ռ�ձȿ��Ƶ�)��
//ÿ��DIGx���ж����Ŀ��ƼĴ������������ֳ�16���ȼ�
//ֵԽ������Խ��
#define AS1115_ITST_DUTYCYCLE_00        0x00
#define AS1115_ITST_DUTYCYCLE_01        0x01
#define AS1115_ITST_DUTYCYCLE_02        0x02
#define AS1115_ITST_DUTYCYCLE_03        0x03
#define AS1115_ITST_DUTYCYCLE_04        0x04
#define AS1115_ITST_DUTYCYCLE_05        0x05
#define AS1115_ITST_DUTYCYCLE_06        0x06
#define AS1115_ITST_DUTYCYCLE_07        0x07
#define AS1115_ITST_DUTYCYCLE_08        0x08
#define AS1115_ITST_DUTYCYCLE_09        0x09
#define AS1115_ITST_DUTYCYCLE_0A        0x0A
#define AS1115_ITST_DUTYCYCLE_0B        0x0B
#define AS1115_ITST_DUTYCYCLE_0C        0x0C
#define AS1115_ITST_DUTYCYCLE_0D        0x0D
#define AS1115_ITST_DUTYCYCLE_0E        0x0E
#define AS1115_ITST_DUTYCYCLE_0F        0x0F

//ɨ�����ƣ����Կ���ֻɨ��DIG0��DIGx(����)
#define AS1115_SCAN_DISPLAY_D00         0x00
#define AS1115_SCAN_DISPLAY_D01         0x01
#define AS1115_SCAN_DISPLAY_D02         0x02
#define AS1115_SCAN_DISPLAY_D03         0x03
#define AS1115_SCAN_DISPLAY_D04         0x04
#define AS1115_SCAN_DISPLAY_D05         0x05
#define AS1115_SCAN_DISPLAY_D06         0x06
#define AS1115_SCAN_DISPLAY_D07         0x07

//ϵͳ���ƼĴ���
#define AS1115_SYS_CTRL_DEFAULT         0x00
#define AS1115_SYS_CTRL_CLK_OUT         0x01
#define AS1115_SYS_CTRL_RESET_EN        0x02
#define AS1115_SYS_CTRL_DECODE_HEX      0x04
#define AS1115_SYS_CTRL_BLINK_EN        0x10
#define AS1115_SYS_CTRL_BLINK_2HZ       0x20
#define AS1115_SYS_CTRL_BLINK_SYNC_EN   0x40
#define AS1115_SYS_CTRL_BLINK_START_ON  0x80

#define AS1115_SYS_CTRL_CLK_IN          0x00
#define AS1115_SYS_CTRL_RESET_DIS       0x00
#define AS1115_SYS_CTRL_DECODE_B        0x00
#define AS1115_SYS_CTRL_BLINK_DIS       0x00
#define AS1115_SYS_CTRL_BLINK_1HZ       0x00
#define AS1115_SYS_CTRL_BLINK_SYNC_DIS  0x00
#define AS1115_SYS_CTRL_BLINK_START_OFF 0x00

//shutdownģʽ���ƣ��͹���
#define AS1115_SHUTDOWN_RESET_FEATURE   0x00
#define AS1115_SHUTDOWN_UCHANGE_FEATURE 0x80
#define AS1115_NORMAL_RESET_FEATURE     0x01
#define AS1115_NORMAL_UNCHANGE_FEATURE  0x81

//��ַģʽ
#define AS1115_ADDR_MODE_FACTORY        0x00 //������ַ
//�û������ַ:0x00 0x02 0x04 0x06����KEYA��������SEGF/SEGG
#define AS1115_ADDR_MODE_USER           0x01 

#define AS1115_LED_DP                   0x80 //С����

#define AS1115_MAX_LED_NUM              64
#define AS1115_LED_MEMORY_LEN           8

#define AS1115_RW_REPEAT_TIME           10

//#define AS1115_SysLowPower() I2C_WriteByte(AS1115_DFUADDR, AS1115_ADDR_SHUT_DOWN, AS1115_SHUTDOWN_RESET_FEATURE)

#define AS1115_InterfaceInit()  I2C_Init()
void AS1115_Init(u8 decodeMode);
void AS1115_LowPower(LPMode lp);
BOOL AS1115_WriteByte(u8 addr, u8 command, u8 data);

#endif

