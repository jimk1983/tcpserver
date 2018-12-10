/******************************************************************************

                  ��Ȩ���� (C), 2017-2020, �������޹�˾

 ******************************************************************************
  �� �� ��   : rct_api.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2018��4��17��
  ����޸�   :
  ��������   : �ṩ���ⲿʹ�õ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��4��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
        
#ifdef __cplusplus
    extern "C" {
#endif

INT32   RCT_API_EnvInit();

VOID    RCT_API_EnvUnInit();

INT32   RCT_API_EnvRun();

INT32   RCT_API_EnvTaskInitRegister(VOS_CALLBACK pfInitCb, VOID *pvArg, ULONG ulRctType,ULONG ulRctSubType, VOS_CALLBACK pfUnInitCb);

INT32   RCT_API_EnvTaskInitUnRegister(ULONG ulRctType, ULONG ulRctSubType);

VOID    RCT_API_EnvGetPthConfInfo(  UINT32    *pulRctType, UINT32    *pulSubPthIndex);

ULONG   RCT_API_EnvGetTypeCfgSubNums(ULONG ulRctType);

INT32   RCT_API_MgtHandlerRegister(ULONG ulRctType, ULONG ulSubType, const CHAR *pcModuName, pfComMsgHandlerCallBack pfMethod);

VOID    RCT_API_MgtHandlerUnRegister(ULONG ulRctType, ULONG ulSubType);

LONG    RCT_API_MgtNormalSend(ULONG ulSrcSubType,
                                         ULONG ulDstRctType,
                                         ULONG ulDstSubType,
                                         ULONG ulDstPthIndex,
                                         CHAR *pcData,ULONG ulLen);

LONG    RCT_API_MgtLockySend( ULONG ulSrcSubType, 
                                        ULONG ulDstRctType, 
                                        ULONG ulDstSubType, 
                                        ULONG ulDstPthIndex, 
                                        CHAR *pcData, ULONG ulLen, UINT32 uiLockID);

LONG    RCT_API_MgtSendBack(RCT_MSG_HEAD_S            *pstSrcHead, CHAR *pcData, ULONG ulLen);

/*�����¼�*/
typedef struct tagRctNetOptsEvent
{
  LONG                  lSockfd;               
  ULONG                 ulEventMask;       
  VOS_CALLBACK_S        stRecv;               
  VOS_CALLBACK_S        stSend;               
}RCT_NETOPT_EVENT_S;

INT32 RCT_API_NetOpsEventRegister( RCT_NETOPT_EVENT_S *pstNetOps,  
                                                LONG lSockfd,  
                                                ULONG ulEMask, 
                                                VOS_CALLBACK pfRecv, 
                                                VOS_CALLBACK pfSend,
                                                VOID *pvConn);

INT32 RCT_API_NetOpsEventUnRegister(RCT_NETOPT_EVENT_S *pstNetOps);

INT32 RCT_API_NetOpsEventCtrl(RCT_NETOPT_EVENT_S *pstNetOps, ULONG ulEpollMask);

/*�ϻ��¼��ڵ�*/
typedef struct tagRctExpireOpts
{
    VOS_DLIST_NODE_S    stNode; 
    VOS_CALLBACK_S      stExpirecb;
    ULONG               ulExpireConfirm;
}RCT_EXPIROPT_EVENT_S;

INT32 RCT_API_ExpireOpsEventRegister(RCT_EXPIROPT_EVENT_S *pstExpireOps, 
                                                    VOS_CALLBACK pfRecv, 
                                                    VOID *pvConn);

INT32 RCT_API_ExpireOpsEventUnRegister(RCT_EXPIROPT_EVENT_S *pstExpireOps);

/*��ʱ������: һ�ζ�ʱ��*/
#define RCT_TIMERTYPE_ONCE             0
/*��ʱ������:ѭ����ʱ��*/
#define RCT_TIMERTYPE_RECYLE           1

typedef struct tagRctTimerOpts
{
    VOS_DLIST_NODE_S    stTimerNode;
    ULONG               ulTimeType;
    ULONG               ulTimerID;
    ULONG               ulTimerOut;
    ULONG               ulTimeStamp;
    VOS_CALLBACK_S      stTimercb;
}RCT_TIMEROPT_EVENT_S;

INT32 RCT_API_TimerOpsEventRegister(RCT_TIMEROPT_EVENT_S *pstTimerOps, 
                                                ULONG ulTimerType, 
                                                ULONG ulTimerOut, 
                                                VOS_CALLBACK pfRecv, 
                                                VOID *pvConn);

INT32 RCT_API_TimerOpsEventUnRegister(RCT_TIMEROPT_EVENT_S *pstTimerOps);

    
#ifdef __cplusplus
        }
#endif


