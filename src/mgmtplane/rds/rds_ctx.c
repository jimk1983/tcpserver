/******************************************************************************

                  ��Ȩ���� (C), 2017-2020, �������޹�˾

 ******************************************************************************
  �� �� ��   : rds_ctx.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2018��8��17��
  ����޸�   :
  ��������   : redis�������Ľӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��8��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include "common.h"
#include "rds/rds_pub.h"


PRDS_CTX_S g_pstRdsCtx = NULL;

/*****************************************************************************
 �� �� ��  : RDS_CtxMsgHandler
 ��������  : ��Ϣ����
 �������  : RCT_MSG_HEAD_S *pstHead  
             CHAR *pcMgtData          
             UINT32 uiLen             
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��12��10��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID RDS_CtxMsgHandler(RCT_MSG_HEAD_S *pstHead, CHAR *pcMgtData, UINT32 uiLen)
{
    ULONG               ulMsgType   = RDS_MSGTYPE_UNKNOW;
    REDIS_TERMAL_INFO_S stTmInfo    = {0} ;
    
    if ( NULL == pstHead
        || NULL == pcMgtData)
    {
        VOS_PRINT("param error!");
        return;
    }

    /*��ȡ��Ϣ��*/
    ulMsgType = *((ULONG *)pcMgtData);
        
    VOS_Printf("Redis message handler: message type=%d", ulMsgType);
   
    /*���ش�����ص���Ϣ*/
    switch(ulMsgType)
    {
        case RDS_MSGTYPE_TerminalInfoAdd:
            {
                PRDS_MSG_TMINFOADD_S pstMsg = (PRDS_MSG_TMINFOADD_S)pcMgtData;

                VOS_StrCpy_S((CHAR *)stTmInfo.acTerminalID, REDIS_TERMAL_STRLEN-1, (CHAR *)pstMsg->acTerminalID);
                VOS_StrCpy_S((CHAR *)stTmInfo.acTerminalDesptor, REDIS_TERMAL_STRLEN-1, (CHAR *)pstMsg->acTerminalDecptor);
                VOS_NToIPV4Str(pstMsg->uiClientAddr, (CHAR *)stTmInfo.acTerminalPubAddr);
                stTmInfo.uiTerminalPubPort = pstMsg->uiClientPort;
                
                if ( VOS_ERR == REDIS_API_TerminalInfoSet(g_pstRdsCtx->stArryRDSConn[0].pstRdsConn, &stTmInfo) )
                {
                   VOS_Printf("Redis tminfo set error: Add TerminalID=%s", pstMsg->acTerminalID);
                }
            }
            break;
        case RDS_MSGTYPE_TerminalInfoDel:
            {
                PRDS_MSG_TMINFOADD_S pstMsg = (PRDS_MSG_TMINFOADD_S)pcMgtData;
                VOS_Printf("Redis message: Del TerminalID=%s", pstMsg->acTerminalID);
            }
            break;
        case RDS_MSGTYPE_TerminalInfoUpdate:
            {

            }
            break;
        case RDS_MSGTYPE_TerminalInfoOffline:
            {
                PRDS_MSG_TMINFOADD_S pstMsg = (PRDS_MSG_TMINFOADD_S)pcMgtData;
                
                if ( VOS_ERR == REDIS_API_TerminalInfoSetStatus(g_pstRdsCtx->stArryRDSConn[0].pstRdsConn, (UCHAR *)pstMsg->acTerminalID, 0) )
                {
                   VOS_Printf("Redis tminfo set error: Add TerminalID=%s", pstMsg->acTerminalID);
                }
            }
            break;
        case RDS_MSGTYPE_RedisServerAdd:
            
            break;
        case RDS_MSGTYPE_RedisServerDel:
            
            break;
        default:
            break;
    }
}


/*****************************************************************************
 �� �� ��  : RDS_CtxTaskInit
 ��������  : �߳������ʼ��
 �������  : VOID *pvArg  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��8��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID RDS_CtxTaskInit(VOID *pvArg)
{
   /*ע����Ϣ���մ�����*/
    if(VOS_ERR == RCT_API_MgtHandlerRegister(RCT_TYPE_MP_RDS, RCT_SUBTYPE_SINGLE, "RDS",  RDS_CtxMsgHandler))
    {
        VOS_PRINT("RDS mgt register error!!");
        return;
    }
}

/*****************************************************************************
 �� �� ��  : RDS_CtxTaskUnInit
 ��������  : �������ʱ����Դ�ͷ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��8��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID RDS_CtxTaskUnInit(VOID *argv)
{
    RCT_API_MgtHandlerUnRegister(RCT_TYPE_MP_RDS, RCT_SUBTYPE_SINGLE); 
}


/*****************************************************************************
 �� �� ��  : RDS_CtxInit
 ��������  : RDS�����Ļ�����ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��8��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG RDS_CtxInit()
{
    /*����Դ��ʼ��*/
    g_pstRdsCtx = (PRDS_CTX_S)malloc(sizeof(RDS_CTX_S));
    if ( NULL == g_pstRdsCtx )
    {
        VOS_Printf("RDS ctx malloc error!\n");
        return VOS_ERR;
    }
    memset(g_pstRdsCtx, 0, sizeof(RDS_CTX_S));

    /*Ĭ�ϳ�ʼ����ǰ��Redis������*/
    VOS_StrCpy_S(g_pstRdsCtx->stArryRDSConn[0].acSevAddr, VOS_IPV4ADDR_STRLEN-1, VOS_SOCK_LOCALHOST_STR);

    g_pstRdsCtx->stArryRDSConn[0].iSevPort = RDS_SERVER_PORT;
    
    g_pstRdsCtx->stArryRDSConn[0].pstRdsConn = REDIS_API_ConnCreate(g_pstRdsCtx->stArryRDSConn[0].acSevAddr,
                                                                    g_pstRdsCtx->stArryRDSConn[0].iSevPort,
                                                                    NULL);
    if ( NULL == g_pstRdsCtx->stArryRDSConn[0].pstRdsConn )
    {
        VOS_Printf("RDS connection create error!");
        free(g_pstRdsCtx);
        g_pstRdsCtx = NULL;
        return VOS_ERR;
    }
    
    if ( VOS_ERR == RCT_API_EnvTaskInitRegister(RDS_CtxTaskInit, NULL, RCT_TYPE_MP_RDS,RCT_SUBTYPE_SINGLE, RDS_CtxTaskUnInit) )
    {
        VOS_Printf("RDS ctx init register error!");
        REDIS_API_ConnRelease(&g_pstRdsCtx->stArryRDSConn[0].pstRdsConn);
        free(g_pstRdsCtx);
        g_pstRdsCtx = NULL;
        return VOS_ERR;
    }
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : RDS_CtxUnInit
 ��������  : RDS�����Ļ�����ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID RDS_CtxUnInit()
{
    RDS_CtxTaskUnInit(NULL);
    
    if ( NULL != g_pstRdsCtx )
    {
        if ( NULL != g_pstRdsCtx->stArryRDSConn[0].pstRdsConn )
        {
            REDIS_API_ConnRelease(&g_pstRdsCtx->stArryRDSConn[0].pstRdsConn);
        }
        free(g_pstRdsCtx);
        g_pstRdsCtx = NULL;
    }
}

