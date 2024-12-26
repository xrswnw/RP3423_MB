#ifndef _ANYID_CANFESTIVAL_WIFI_
#define _ANYID_CANFESTIVAL_WIFI_

#include "AnyID_CanFestival_Uart_Receive.h"
#include "AnyID_CanFestival_Wifi_HL.h"


#define WIFI_STAT_TX_IDLE               0
#define WIFI_STAT_TX_BUSY               1


extern UART_RCVFRAME g_sWifiRxFrame;
extern UART_TXFRAME g_sWifiTxFrame;
extern u32 g_nWifiStatus;

#define WIFI_STATUS_AT_ERR_OR_NOSUPP        1
#define WIFI_STATUS_WIFI_CONNET_OK          2
#define WIFI_STATUS_WIFI_CONNET_FAIL        3
#define WIFI_STATUS_SOCEKT_CONNET_OK        4
#define WIFI_STATUS_SOCEKT_CONNET_FAIL      5
#define WIFI_STATUS_OTHER_ERR               6


//----------------------
#define WIFI_CMD_AT_AT                          0               //AT测试                 
#define WIFI_CMD_AT_RST                         1               //复位
#define WIFI_CMD_AT_GMR                         2               //获取版本呢
#define WIFI_CMD_AT_ATE                         3               //禁止回声

#define WIFI_CMD_AT_WMODE                       4               //WiFi模式
#define WIFI_CMD_AT_WSCAN                       5               //WiFi扫描
#define WIFI_CMD_AT_WSDHCP                      6               //WiFi DHCP模式
#define WIFI_CMD_AT_WJAP                        7               //WiFi 连接
#define WIFI_CMD_AT_WJAP_QUERY                  8               //WiFi 连接状态查询
#define WIFI_CMD_AT_WDISCONNECT                 9               //WiFi 断开
#define WIFI_CMD_AT_WAPDHCP                     10              //WiFi Ap 模式DHCP
#define WIFI_CMD_AT_WAP                         11              //WiFi AP 模式
#define WIFI_CMD_AT_PING                        12              //WiFi Ping
#define WIFI_CMD_AT_CIPSTAMAC_DEF               13              //Wi-Fi station MAC 
#define WIFI_CMD_AT_CIPSTAMAC_QUERY             14              //Wi-Fi station MAC 查询
#define WIFI_CMD_AT_WCOUNTRY                    15              //Wi-Fi WiFi国家码
#define WIFI_CMD_AT_WRSSI                       16              //Wi-Fi WiFi rssi

#define WIFI_CMD_AT_SOCKET                      17              //Tcp Server 监听      其实在wifi层面都是连接socket
#define WIFI_CMD_AT_SKT_CONNECT                 18              //Tcp client 主动连接
#define WIFI_CMD_AT_SKT_RECVCFG                 19              //Tcp socket 数据接收模式
#define WIFI_CMD_AT_SKT_ENTTT                   20              //Tcp 使能透传（TTT）
#define WIFI_CMD_AT_WDOMAIN                     21              //Tcp 通过DNS解析域名IP
#define WIFI_CMD_AT_SOCKETQT                    22              //Tcp 自动进入socket透传（只针对client有效）
#define WIFI_CMD_AT_SKT_LISTEN                  23              //Tcp 查询状态，监听是否有主机接入
#define WIFI_CMD_AT_SOCKETRD                    24              //Tcp 读数据

#define WIFI_CMD_AT_DIS_DTU                     25 

#define WIFI_CMD_AT_WAIT                        31              

#define WIFI_CONFIG_WIFI_SSID_MAX_LEN           32
#define WIFI_CONFIG_WIFI_PWD_MAX_LEN            32
#define WIFI_CONFIG_WIFI_IP_LEN                 4
#define WIFI_CONFIG_WIFI_MAC_LEN                6

#define WIFI_NET_CONFIG_STATION                 0x00
#define WIFI_NET_CONFIG_AP                      0x01
#define WIFI_NET_CONFIG_S_A                     0x02
#define WIFI_NET_SSID_MAX_LEN                   32
#define WIFI_NET_PWD_MAX_LEN                    32
typedef struct bw16NetRegs{
    u16 config;
    u16 ssidLen;
    u8 ssid[WIFI_NET_SSID_MAX_LEN + 2];
    u16 pwdLen;
    u8 pwd[WIFI_NET_PWD_MAX_LEN + 2];
    u32 rfu;
}WIFI_NETREGS;

#define WIFI_STATION_CONFIG_TCPMASK             0x06

#define WIFI_STATION_CONFIG_STICIP              0x00        //固定IP
#define WIFI_STATION_CONFIG_ENDHCP              0x01

#define WIFI_STATION_CONFIG_TCPSERVER           0x00
#define WIFI_STATION_CONFIG_TCPCLIENT           0x02

#define WIFI_IP_LEN                             4
#define WIFI_GATEWAY_LEN                        4
#define WIFI_MASK_LEN                           4
#define WIFI_MAC_LEN                            6
typedef struct bw16StaionRegs{
    u16 config;
    
    u16 localPort;
    u8 localIp[WIFI_IP_LEN];
    u8 localGateWay[WIFI_GATEWAY_LEN];
    u8 localMask[WIFI_MASK_LEN];
    
    u16 remotePort;
    u8 remoteIp[WIFI_IP_LEN];
    
    u32 rfu;
}WIFI_STATIONREGS;

#define WIFI_DISENABLE                          0x00
#define WIFI_ENABLE                             0x01

typedef struct bw16Regs{
    u8 enable;
    WIFI_NETREGS netRegs;
    WIFI_STATIONREGS stationRegs;
}WIFI_REGS;

#define WIFI_OP_NUM                         32
#define BW_OP_REG_VERSION_LEN               32
typedef struct bw16OpWifiRegs{
    u8 sate;
    u8 ssidLen;
    u8 pwdLen;
    u32 ch;
    u32 errRegs;
    u8 ssid[WIFI_CONFIG_WIFI_SSID_MAX_LEN];
    u8 pwd[WIFI_CONFIG_WIFI_PWD_MAX_LEN];
    u8 security[WIFI_CONFIG_WIFI_SSID_MAX_LEN];
    u16 ip[WIFI_CONFIG_WIFI_IP_LEN];
    u16 gateWay[WIFI_CONFIG_WIFI_IP_LEN];
    u8 mac[WIFI_CONFIG_WIFI_MAC_LEN];
    u8 bssid[WIFI_CONFIG_WIFI_MAC_LEN];
}WIFI_OPWIFIREGS;

typedef struct bw16OpTcpClientReg{
    u8 id;
    u8 type;
    u8 status;
    u8 ip[WIFI_CONFIG_WIFI_IP_LEN];
    u16 port;
}WIFI_OPTCPCREG;

#define WIFI_OP_MODE_IDLE               0x00
#define WIFI_OP_MODE_DTU                0x01
#define WIFI_OP_MODE_ERR                0x02

#define WIFI_OP_STATE_IDEL              0x00
#define WIFI_OP_STATE_TX                0x01
#define WIFI_OP_STATE_WAIT              0x02
#define WIFI_OP_STATE_RCV               0x04
#define WIFI_OP_STATE_STEP              0x08
#define WIFI_OP_STATE_RCV_OVER          0x10
#define WIFI_OP_STATE_DELAY             0x20
#define WIFI_OP_STATE_MONITOR           0x40
#define WIFI_OP_STATE_S                 0x80

#define WIFI_OP_DELAY_TIME              3
#define WIFI_TO_AT_MIN                  20
#define WIFI_TO_AT_NORMAL               200
#define WIFI_TO_AT_LINK                 2000                    
#define WIFI_OP_REPAT_TICK              3

#define WIFI_OP_ERR_GET_GMR             0x00000001
#define WIFI_OP_ERR_SET_WORKMOD         0x00000002
#define WIFI_OP_ERR_SET_IP              0x00000004
#define WIFI_OP_ERR_SCAN_AP             0x00000008
#define WIFI_OP_ERR_LINK_AP             0x00000010
#define WIFI_OP_ERR_LINK_AP_QUEN        0x00000020
#define WIFI_OP_ERR_SET_SOCKET          0x00000040
#define WIFI_OP_ERR_SET_SOCKET_TT       0x00000080
#define WIFI_OP_ERR_SET_SOCKET_SC       0x00000100

#define WIFI_OP_ERR_SOCKET_LINK         0x80000000              //sokect 连接
#define WIFI_DTU_STAT_DIS               0
#define WIFI_DTU_STAT_CONWIFI           1
#define WIFI_DTU_STAT_DTU               2

#define WIFI_LOG_DISENABLE              0x00
#define WIFI_LOG_ENABLE                 0x01
typedef struct bw16OpRegs{
    u8 log;
    u8 dtuState;                        //dtu状态
    u32 dtuTick;  
    //u8 conID;
    u8 index;
    u8 num;
    u8 mode;
    u8 state;
    u32 repeat;
    u8 op[WIFI_OP_NUM];
    u16 to[WIFI_OP_NUM];
    u32 tick;
    WIFI_REGS conRegs;
    u8 tcpMode;
}WIFI_OPREGS;

extern WIFI_OPREGS g_sWifiOpRegs;
extern UART_RCVFRAME g_sWifiRcvFrame;

#define WIFI_ANALSYS_INFO_IP             0x00
#define WIFI_ANALSYS_INFO_GATEWAY        0x01
#define WIFI_ANALSYS_INFO_NETMASK        0x02
#define WIFI_ANALSYS_INFO_MAC            0x03
#define WIFI_ANALSYS_INFO_STAT           0x04
#define WIFI_ANALSYS_INFO_SSID           0x05
#define WIFI_ANALSYS_INFO_PWD            0x06

#define Wifi_ChkDtu()                   (g_sWifiOpRegs.mode == WIFI_OP_MODE_DTU)

#define Wifi_ClearOpInfo()              do{g_sWifiOpRegs.state = WIFI_OP_STATE_IDEL; g_sWifiOpRegs.index = 0; g_sWifiOpRegs.num = 0; g_sWifiRcvFrame.state = UART_STAT_IDLE;}while(0)
#define Wifi_ClrRcv()                   do{g_sWifiOpRegs.state = WIFI_OP_STATE_IDEL; }while(0)
#define Wifi_OpDelay(t)                 do{g_sWifiOpRegs.state = WIFI_OP_STATE_DELAY; g_sWifiOpRegs.tick = t; g_sWifiOpRegs.repeat = 0;}while(0)

extern char g_aWifiTmpBuf[WIFI_FEAME_TX_LEN];

void Wifi_Connect(WIFI_OPREGS *pOpRegs, WIFI_REGS *pConRegs);
void Wifi_QueneEnter(WIFI_OPREGS *pOpRegs, u8 op, u16 to);
#define Wifi_QueneChk(t)                ((t)->index < (t)->num)
void Wifi_ConnectWifi(WIFI_OPREGS *pOpRegs);

void Wifi_Tranceive(WIFI_OPREGS *pOpRegs, u32 tick);
BOOL Wifi_ChkRcv(WIFI_OPREGS *pOpRegs, u8 *pRxFrame, u16 rxLen);
BOOL Wifi_Step(WIFI_OPREGS *pOpRegs);

#define WIFI_ERR_NO             0
#define WIFI_ERR_WIFI           1
#define WIFI_ERR_SOCKET         2
#define WIFI_ERR_OTHER          255
u8 Wifi_CheckEvent(char *pEvtStr, WIFI_OPREGS *pOpRegs);

BOOL Wifi_CheckClientConnected(char *str, WIFI_OPREGS *pOpRegs);
BOOL Wifi_GetSocketReadInfo(char *str, WIFI_OPREGS *pOpRegs, u16 *pDatStart);
BOOL Wifi_GetSocketDownInfo(char *str, WIFI_OPREGS *pOpRegs);
BOOL Wifi_DelSocket(char *str);

void Wifi_Init(u32 bud);
void  Wifi_NetInit(void *p);

BOOL Wifi_RcvQueue(QueueHandle_t queue);

extern TaskHandle_t g_hWifi_NetInit;
#endif
