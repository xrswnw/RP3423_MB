#ifndef _ANYID_CP_BIKE_
#define _ANYID_CP_BIKE_
#include "AnyID_CP.h"

//�ýṹ����Ϊͨ��Э���ʽʱ������Ҫ����ϵͳ�ṹ��Ķ���ģʽ��ϵͳ��"�������"��"�Ƕ������"������



//�����豸�趨��һ���豸�ȿ����Ǵ��豸Ҳ���������豸
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CP_BIKE_MASTER          0x01
#define CP_BIKE_SLAVER          0x01
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//ϵͳ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ѯ���г���ʶ��UID��	0X01
//�������г���\��״̬   0X02
//��ȡ���г�������Ϣ    0X03
//�������г�������Ϣ    0X11
//��ȡ���г�������Ϣ    0X12
//�������г�ϵͳ����    0XE1
//ִ�����г�ϵͳ����    0XE2
//ֹͣ���г�ϵͳ����    0XE3
#define CP_BIKE_CMD_GET_ID                  0x01
#define CP_BIKE_CMD_SET_STAT                0x02
#define CP_BIKE_CMD_GET_RUN_INF             0x03


#define CP_BIKE_CMD_SET_TRADE_INF           0x11
#define CP_BIKE_CMD_GET_TRADE_INF           0x12

#define CP_BIKE_CMD_REQUEST_UPDATE          0xE1
#define CP_BIKE_CMD_UPDATE                  0xE2
#define CP_BIKE_CMD_STOP_UPDATE             0xE3


//�ڲ�������
#define CP_BIKE_CMD_SET_STAT_AND_TRDINF     0x21
#define CP_BIKE_CMD_GET_INF                 0x22
#define CP_BIKE_CMD_SET_ID                  0x23
#define CP_BIKE_CMD_SET_RUN_INF             0x24
#define CP_BIKE_CMD_SET_WPW                 0x25    //�������߷��书��



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//��ȡBikeID��Bike������Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȷ��ȡBikeID��Ӧ��֡������ṹ��
#define CP_BIKE_ERRCODE_LEN                                     1
#define CP_BIKE_ID_LEN                                          12
//Bike������
#define CP_BIKE_ERRCODE_CHARGE                                  0x80    //������
#define CP_BIKE_ERRCODE_BLDC                                    0x40    //�������
#define CP_BIKE_ERRCODE_HALL                                    0x20    //Hall����
#define CP_BIKE_ERRCODE_MP3                                     0x10    //MP3����
#define CP_BIKE_ERRCODE_FLASH                                   0x08    //Flash����
typedef struct bikeIDinf{
    u8 errCode;
    u8 id[CP_BIKE_ID_LEN];
}CP_BikeIDInf, CP_BikeGIDRSP;


//��ȡBikeID������֡��ֻ�����豸����
#if CP_BIKE_MASTER
    #define CP_BikeGetIDReqFrame(pFrame)                        CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_REQUEST | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_GET_ID, \
                                                                             CP_FRAME_STATE_REQUEST, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)
#endif


//��Ӧ֡��ֻ�д��豸����
#if CP_BIKE_SLAVER
    u8 CP_BikeGetIDRspFrame(u8 * pFrame, u8 state, u8 bikeErrCode, u8 *pBikeID);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����BikeID������֡
 typedef struct bikeIDinf CP_BikeSIDREQ;
#if CP_BIKE_MASTER
     u8 CP_BikeSetIDReqFrame(u8 * pFrame, u8 bikeErrCode, u8 *pBikeID);                                                                        
#endif


//��Ӧ֡
#if CP_BIKE_SLAVER
    u8 CP_BikeSetIDRspFrame(u8 * pFrame, u8 state, u8 bikeState);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










///����Bike��/��״̬
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CP_BIKE_TRADE_TIME_LEN                                  6
typedef union tradeTime{
    u8 tradeTime[CP_BIKE_TRADE_TIME_LEN];
    struct{
        u8 year;
        u8 month;
        u8 day;
        u8 hour;
        u8 minute;
        u8 second;
    }sTradeTime;
} CP_BIKETradeTime;
#define CP_BIKE_BSTAT_RETURN                                    0x00        //����
#define CP_BIKE_BSTAT_CRUISE                                    0x01        //Ѳ��
#define CP_BIKE_BSTAT_BOOST                                     0x02        //����
#define CP_BIKE_BSTAT_POS                                       (CP_FRAME_DSTART_POS)
#define CP_BIKE_BSTAT_LEN                                       1
#define CP_BIKE_TIME_POS                                        (CP_BIKE_BSTAT_POS + CP_BIKE_BSTAT_LEN)
#define CP_BIKE_TIME_LEN                                        sizeof(CP_BIKETradeTime)
#if CP_BIKE_MASTER
    u8 CP_BikeSetStateReqFrame(u8 *pFrame, u8 state, u8 *pTimes);
#endif

#if CP_BIKE_SLAVER
    //����Bike״̬��Ӧ��֡
    #define CP_BikeSetStateRspFrame(pFrame, state)              CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_SET_STAT, \
                                                                             state, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////















//��ȡBike��ʻ��Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȷ��ȡBike��ʻ��Ϣ��Ӧ��֡������ṹ��
typedef struct bikeRunInf{
    u32 mileage;
    u32 totalMileage;
    u32 reduceC;
    u32 totalReduceC;
}CP_BikeRunInf, CP_BikeGRunInfRSP;

#if CP_BIKE_MASTER
    //��ȡBike��ʻ��Ϣ����֡
    #define CP_BikeGetRunInfReqFrame(pFrame)                    CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_REQUEST | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_GET_RUN_INF, \
                                                                             CP_FRAME_STATE_REQUEST, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)
#endif


#if CP_BIKE_SLAVER
    u8 CP_BikeGetRunInfRspFrame(u8 * pFrame, u8 state, u32 mileage, u32 totalMileage, u32 reduceC, u32 totalReduceC);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//����Bike��ʻ��Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct bikeRunInf CP_BikeSRunInfREQ;
#if CP_BIKE_MASTER
    //����Bike��ʻ��Ϣ����֡
    u8 CP_BikeSetRunInfReqFrame(u8 * pFrame, u32 mileage, u32 totalMileage, u32 reduceC, u32 totalReduceC);
#endif


#if CP_BIKE_SLAVER
    #define CP_BikeSetRunInfRspFrame(pFrame)                    CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_SET_RUN_INF, \
                                                                             CP_FRAME_STATE_RESPONE_OK, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////













//����Bike������Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Bike������Ϣ������֡������ṹ��
#define CP_BIKE_USER_ID_LEN                                     4
#define CP_BIKE_POLE_ID_LEN                                     4
typedef struct bikeTradeInf{
    CP_BIKETradeTime uTradeTime;
    u8 userID[CP_BIKE_USER_ID_LEN];
    u8 poleID[CP_BIKE_POLE_ID_LEN];
}CP_BikeTradeInf, CP_BikeSTradeInfREQ;

#if CP_BIKE_MASTER
    //����Bike������Ϣ����֡
    u8 CP_BikeSetTradeInfReqFrame(u8 *pFrame, u8 *pTradeTime, u8 *pUserID, u8 *pPoleID);
#endif

#if CP_BIKE_SLAVER
    //����Bike������Ϣ��Ӧ��֡
    #define CP_BikeSetTradeInfRspFrame(pFrame, state)           CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_SET_TRADE_INF, \
                                                                             state, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////













//��ȡBike������Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȡBike������Ϣ����Ӧ֡������ṹ��
typedef struct bikeTradeInf CP_BikeGTradeInfRSP;
#if CP_BIKE_MASTER
    //��ȡBike������Ϣ����֡
    #define CP_BikeGetTradeInfReqFrame(pFrame)                  CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_REQUEST | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_GET_TRADE_INF, \
                                                                             CP_FRAME_STATE_REQUEST, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)

#endif


#if CP_BIKE_SLAVER
    //��ȡBike������Ϣ��Ӧ��֡
    u8 CP_BikeGetTradeInfRspFrame(u8 *pFrame, u8 state, u8 *pTradeTime, u8 *pUserID, u8 *pPoleID);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//����Bike״̬�ͽ�����Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct bikeStatAndTradeInf{
    u8 state;
    struct bikeTradeInf tradeInf;
}CP_BikeSStatAndTradeInfREQ;

#if CP_BIKE_MASTER
    //����Bike������Ϣ����֡
    u8 CP_BikeSetStatTrdInfReqFrame(u8 *pFrame, u8 *pTradeTime, u8 *pUserID, u8 *pPoleID, u8 state);
#endif

#if CP_BIKE_SLAVER
    //����Bike������Ϣ��Ӧ��֡
    #define CP_BikeSetStatTrdInfRspFrame(pFrame, state)         CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_SET_STAT_AND_TRDINF, \
                                                                             state, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*


//��ȡBike������Ϣ��״̬
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȡBike������Ϣ��״̬����Ӧ֡������ṹ��
typedef struct bikeStatAndTradeInf CP_BikeGStatAndTradeInfRSP;
#if CP_BIKE_MASTER
    //��ȡBike������Ϣ��״̬����֡
    #define CP_BikeGetStatTrdInfReqFrame(pFrame)                CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_REQUEST | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_GET_STAT_AND_TRDINF, \
                                                                             CP_FRAME_STATE_REQUEST, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)

#endif


#if CP_BIKE_SLAVER
    //��ȡBike������Ϣ��״̬��Ӧ��֡
    u8 CP_BikeGetStatTrdInfRspFrame(u8 *pFrame, u8 state, u8 *pTradeTime, u8 *pUserID, u8 bikeState);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/

//��ȡBike��Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȡBike��Ϣ
typedef struct bikeInf{
    u8 state;
    struct bikeIDinf bikeID;
    struct bikeTradeInf tradeInf;
    struct bikeRunInf runInf;
}CP_BikeInf, CP_BikeGetInfREQ;
#define CP_BIKE_INF_STAT_POS                                    CP_FRAME_DSTART_POS
#define CP_BIKE_INF_STAT_LEN                                    1
#define CP_BIKE_INF_ID_POS                                      (CP_BIKE_INF_STAT_POS + CP_BIKE_INF_STAT_LEN) 
#define CP_BIKE_INF_TRADE_POS                                   (CP_BIKE_INF_ID_POS + sizeof(CP_BikeIDInf)) 
#define CP_BIKE_INF_RUN_POS                                     (CP_BIKE_INF_TRADE_POS + sizeof(CP_BikeTradeInf)) 



#if CP_BIKE_MASTER
    //��ȡBike��Ϣ
    #define CP_BikeGetInfReqFrame(pFrame)                       CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_REQUEST | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_GET_INF, \
                                                                             CP_FRAME_STATE_REQUEST, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)
#endif


#if CP_BIKE_SLAVER
    //��ȡBike������Ϣ��״̬��Ӧ��֡
    u8 CP_BikeGetInfRspFrame(u8 *pFrame, u8 state, u8 bikeState, CP_BikeIDInf *pSIDInf, CP_BikeTradeInf *pSTradeInf, CP_BikeRunInf *pSRunInf);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//�������߷��书��
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CP_BIKE_SET_WPW_LV_POS                                      CP_FRAME_DSTART_POS
#define CP_BIKE_SET_WPW_LV_MAX                                      17
#if CP_BIKE_MASTER
    u8 CP_BikeSetWPwLvReqFrame(u8 *pFrame, u8 lv);
#endif

#if CP_BIKE_SLAVER
    //�������߷��͹��ʵ�Ӧ��֡
    #define CP_BikeSetWPwLvRspFrame(pFrame)              CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                     CP_FRAME_BROADCAST_ADDR, \
                                                                     CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER, \
                                                                     CP_BIKE_CMD_SET_WPW, \
                                                                     CP_FRAME_STATE_RESPONE_OK, \
                                                                     pFrame, \
                                                                     CP_FRAME_NO_DATA)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������Bike�ļ�����֡���ݽṹ��
//�ļ�����
#define CP_BIKE_UPDATEINF_TYPE_BP                               0x01        //Bikeͣ��������
#define CP_BIKE_UPDATEINF_TYPE_MP                               0x02        //MP3���ŵ�����
#define CP_BIKE_UPDATEINF_TYPE_BIN                              0x03        //ϵͳ����Bin�ļ�
#define CP_BIKE_UPDATEINF_TYPE_BPN                              0x04        //Bikeͣ��������
#define CP_BIKE_UPDATEINF_TYPE_MP3                              0x05        //MP3�ļ�


#define CP_BIKE_UPDATEINF_BP_NUM_LEN                            4           //Bikeͣ��������
#define CP_BIKE_UPDATEINF_BPF_SIZE_LEN                          4
#define CP_BIKE_UPDATEINF_MP_NUM_LEN                            4           //MP3���ŵ�����
#define CP_BIKE_UPDATEINF_MPF_SIZE_LEN                          4
#define CP_BIKE_UPDATEINF_BIN_VERSION_LEN                       4           //ϵͳ����Bin�ļ�
#define CP_BIKE_UPDATEINF_BIN_SIZE_LEN                          4
#define CP_BIKE_UPDATEINF_BPN_NUM_LEN                           4           //Bikeͣ��������
#define CP_BIKE_UPDATEINF_BPNF_SIZE_LEN                         4
#define CP_BIKE_UPDATEINF_MP3_IDX_LEN                           4           //MP3�ļ�
#define CP_BIKE_UPDATEINF_MP3F_SIZE_LEN                         4


typedef struct bikeReqUpdateInf{
    u8 updateType;
    union {
        struct{
            u8 num[CP_BIKE_UPDATEINF_BP_NUM_LEN];
            u8 size[CP_BIKE_UPDATEINF_BPF_SIZE_LEN];
        }sBikePoint;
        struct{
            u8 num[CP_BIKE_UPDATEINF_MP_NUM_LEN];
            u8 size[CP_BIKE_UPDATEINF_MPF_SIZE_LEN];
        }sMp3Point;
        struct{
            u8 version[CP_BIKE_UPDATEINF_BIN_VERSION_LEN];
            u8 size[CP_BIKE_UPDATEINF_BIN_SIZE_LEN];
        }sBikeBin;
        struct{
            u8 num[CP_BIKE_UPDATEINF_BPN_NUM_LEN];
            u8 size[CP_BIKE_UPDATEINF_BPNF_SIZE_LEN];
        }sBikePointName;
        struct{
            u8 index[CP_BIKE_UPDATEINF_MP3_IDX_LEN];
            u8 size[CP_BIKE_UPDATEINF_MP3F_SIZE_LEN];
        }sMp3;
    }uUpdateFileInf;
} CP_BikeReqUpdateInf;


//�������Bike�ļ���ʧ����Ӧ֡
#define CP_BIKE_REQUPDATE_ERR_NONEED                            0x01        //�Ѿ������£����ظ���
#define CP_BIKE_REQUPDATE_ERR_ERSFLASH                          0x02        //����FLASH����
#define CP_BIKE_REQUPDATE_ERR_FSIZE                             0x03        //�ļ�̫��


#if CP_BIKE_MASTER
    //�������Bike�ļ�����֡
    u8 CP_BikeReqUpdateReqFrame(u8 *pFrame, u8 fileType, u32 filePar, u32 fileSize);
#endif

#if CP_BIKE_SLAVER
    //�������Bike�ļ���Ӧ֡
    #define CP_BikeReqUpdateRspFrame(pFrame, state)             CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_BROADCAST_ADDR, \
                                                                             CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER, \
                                                                             CP_BIKE_CMD_REQUEST_UPDATE, \
                                                                             state, \
                                                                             pFrame, \
                                                                             CP_FRAME_NO_DATA)
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////












////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Bike�ļ�����֡���ݽṹ��
#define CP_BIKE_UPDATE_INDEX_LEN                            4
#define CP_BIKE_UPDATE_FILE_LEN                             32
typedef struct bikeUpdate{
    u8 index[CP_BIKE_UPDATE_INDEX_LEN];
    u8 file[CP_BIKE_UPDATE_FILE_LEN];
} CP_BikeUpdateREQ;

#if CP_BIKE_MASTER
    //����Bike�ļ�����֡
    u8 CP_BikeUpdateReqFrame(u8 *pFrame, u8 isNext, u32 fileIndex, u8 *pFile);
#endif

#if CP_BIKE_SLAVER
    //����Bike�ļ�Ӧ��֡
    u8 CP_BikeUpdateRspFrame(u8 *pFrame, u8 state, u32 fileIndex);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








//ֹͣ����ϵͳ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if CP_BIKE_MASTER
    //ֹͣ����Bike�ļ�����֡
    #define CP_BikeStpUpdateReqFrame(pFrame)             CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                      CP_FRAME_BROADCAST_ADDR, \
                                                                      CP_FRAME_CTRL_REQUEST | CP_FRAME_CTRL_OVER, \
                                                                      CP_BIKE_CMD_STOP_UPDATE, \
                                                                      CP_FRAME_STATE_REQUEST, \
                                                                      pFrame, \
                                                                      CP_FRAME_NO_DATA)
#endif

#if CP_BIKE_SLAVER
    #define CP_BikeStpUpdateRspFrame(pFrame, state)      CP_GetFrame(CP_FRAME_BROADCAST_ADDR, \
                                                                      CP_FRAME_BROADCAST_ADDR, \
                                                                      CP_FRAME_CTRL_RESPONSE | CP_FRAME_CTRL_OVER, \
                                                                      CP_BIKE_CMD_STOP_UPDATE, \
                                                                      state, \
                                                                      pFrame, \
                                                                      CP_FRAME_NO_DATA)

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
