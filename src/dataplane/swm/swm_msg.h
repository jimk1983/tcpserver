/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_msg.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : SWM����Ϣ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/



/*������Ϣ���ʹ����Ӧ�Ľṹ��Ϣ*/
typedef enum
{
    SWM_MSGTYPE_UNKNOW=0,
    SWM_MSGTYPE_ADDTLSCONN,        /*���ؼ����ͻ��˽ڵ����*/

    SWM_MSGTYPE_NUMS,
}SWM_MSG_TYPE_E;


typedef struct tagSwmMsgAddConn
{
    /*ÿ����Ϣ������ö��������ͷ��������Ϣ����
    ͷ����Ϣ��ͨ������:RCT_API_EnvGetPthreadConfInfo()����ȡ*/
    ULONG                       ulMsgCode;
    /************************************/
    /*���ؼ������Ŀͻ���socket*/
    LONG                        lConnfd;
    /*���������ص�ID*/
    ULONG                       ulVtID;
    /*���ص�SSL������*/
    //SSL_CTX_S                  *pstSSLCtx;
    
}SWM_MSG_ADDCONN_S;


LONG SWM_MSG_AddTlsConn(  RCTMSG_DSTINFO_S *pstDstInfo, 
                               LONG lConnfd,
                               ULONG ulVtID);


