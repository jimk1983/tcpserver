/******************************************************************************

                  ��Ȩ���� (C), 2017-2020, �������޹�˾

 ******************************************************************************
  �� �� ��   : rds_tminfo.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2018��8��17��
  ����޸�   :
  ��������   : redis���ն���Ϣ�ӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��8��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*�ο�UPFģ��Ķ��弴�ɣ���Ҫ����ģ��ʹ��*/

/*�豸��ǩ����*/
#define RDS_UPF_DEVLAB_LEN      64
#define RDS_UPF_FILELEN         32
#define RDS_UPF_FILEVALEN       36


/*������Ϣ���ʹ����Ӧ�Ľṹ��Ϣ*/
typedef enum
{
    RDS_MSGTYPE_UNKNOW=0,
    RDS_MSGTYPE_TerminalInfoAdd,        /*��ӿͻ���״̬*/
    RDS_MSGTYPE_TerminalInfoDel,        /*ɾ���ͻ���״̬*/
    RDS_MSGTYPE_TerminalInfoUpdate,     /*���¿ͻ���״̬*/
    RDS_MSGTYPE_RedisServerAdd,         /*Redis���������*/
    RDS_MSGTYPE_RedisServerDel,         /*Redis������ɾ��*/

    RDS_MSGTYPE_NUMS,
}RDS_MSG_TYPE_E;


typedef struct tagRdsMsgTerminalInfoAdd
{
    /*�̶���Ϣͷ��ͨ������:RCT_API_EnvGetPthreadConfInfo()*/
    ULONG                       ulMsgCode;
    /************************************/
    /*���ؼ������Ŀͻ���socket*/
    CHAR        acTerminalID[RDS_UPF_DEVLAB_LEN];       /*�豸��ǩ*/
    CHAR        acTerminalDecptor[RDS_UPF_DEVLAB_LEN];       /*�豸����*/
    UINT32      uiClientAddr;                           /*�ͻ��˵�ַ��Ϣ*/
    UINT32      uiClientPort;                           /*�ͻ��˶Ͽ���Ϣ*/
}RDS_MSG_TMINFOADD_S, *PRDS_MSG_TMINFOADD_S;

LONG RDS_MSG_TerminalInfoAdd(CHAR*  pcTerminalID, 
                                   CHAR*  pcTerminalDesptor,
                                   UINT32 uiCliAddr, 
                                   UINT32 uiCliPort);

LONG RDS_MSG_TerminalInfoDel(      CHAR*  pcTerminalID);
  
LONG RDS_MSG_TerminalInfoUpdate(    CHAR*  pcTerminalID);






