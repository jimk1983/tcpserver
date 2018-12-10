/******************************************************************************

                  ��Ȩ���� (C), 2017-2020, �������޹�˾

 ******************************************************************************
  �� �� ��   : rds_ctx.h
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
#include "common.h"
#include "rds/rds_pub.h"



/*****************************************************************************
 �� �� ��: RDS_MSG_TerminalInfoAdd
 ��������  : ����ն���Ϣ
 �������  : RCTMSG_DSTINFO_S *pstDstInfo    
           pcTerminalID                     ---��Ҫ���µ�TerminalID��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��22��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG RDS_MSG_TerminalInfoAdd(CHAR*  pcTerminalID, 
                                   CHAR*  pcTerminalDesptor,
                                   UINT32 uiCliAddr, 
                                   UINT32 uiCliPort)
{
    PRDS_MSG_TMINFOADD_S pstMsgInfo = NULL;
    RCTMSG_DSTINFO_S     stDstInfo  = {0};
    ULONG                ulMsgSize  = sizeof(RDS_MSG_TMINFOADD_S);
    

    pstMsgInfo = (PRDS_MSG_TMINFOADD_S)VOS_Malloc(RDS_MID_MSG, ulMsgSize);
    if ( NULL == pstMsgInfo )
    {
        VOS_Printf("malloc error");
        return VOS_ERR;
    }
        
    pstMsgInfo->ulMsgCode  = RDS_MSGTYPE_TerminalInfoAdd;
    
    /*��Ϣ���ݵİ�ȫ����*/
    VOS_StrCpy_S(pstMsgInfo->acTerminalID, RDS_UPF_DEVLAB_LEN-1, pcTerminalID);
    VOS_StrCpy_S(pstMsgInfo->acTerminalDecptor, RDS_UPF_DEVLAB_LEN-1, pcTerminalDesptor);
    pstMsgInfo->uiClientAddr = uiCliAddr;
    pstMsgInfo->uiClientPort = uiCliPort;

    /*ֱ��ȷ��Ŀ��*/
    stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //һ�㶼�����
    stDstInfo.ulDstRctType = RCT_TYPE_MP_RDS;       
    stDstInfo.ulDstSubType = RCT_SUBTYPE_SINGLE;   //SSL�ж���ҵ��ڵ�����
    stDstInfo.ulDstPthIndex= RCT_PTHNUM_ONLYONE;
    
    /*��Ϣ����*/
    if ( VOS_ERR == RCT_API_MgtNormalSend( stDstInfo.ulSrcSubType, 
                                            stDstInfo.ulDstRctType, 
                                            stDstInfo.ulDstSubType, 
                                            stDstInfo.ulDstPthIndex,
                                            (CHAR *)pstMsgInfo, 
                                            ulMsgSize))
    {
        VOS_Printf("rct msg send message error");
        return VOS_ERR;
    }
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : RDS_MSG_TerminalInfoDel
 ��������  : ɾ��
 �������  : RCTMSG_DSTINFO_S *pstDstInfo  
             CHAR*  pcTerminalID           
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��12��10��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG RDS_MSG_TerminalInfoDel(      CHAR*  pcTerminalID)
{
    PRDS_MSG_TMINFOADD_S pstMsgInfo = NULL;
    RCTMSG_DSTINFO_S     stDstInfo  = {0};
    ULONG                ulMsgSize  = sizeof(RDS_MSG_TMINFOADD_S);
    
    pstMsgInfo = (PRDS_MSG_TMINFOADD_S)VOS_Malloc(RDS_MID_MSG, ulMsgSize);
    if ( NULL == pstMsgInfo )
    {
        VOS_Printf("malloc error");
        return VOS_ERR;
    }
        
    pstMsgInfo->ulMsgCode  = RDS_MSGTYPE_TerminalInfoDel;
    
    /*��Ϣ���ݵİ�ȫ����*/
    VOS_StrCpy_S(pstMsgInfo->acTerminalID, RDS_UPF_DEVLAB_LEN-1, pcTerminalID);
    
    /*ֱ��ȷ��Ŀ��*/
    stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //һ�㶼�����
    stDstInfo.ulDstRctType = RCT_TYPE_MP_RDS;       
    stDstInfo.ulDstSubType = RCT_SUBTYPE_SINGLE;   //SSL�ж���ҵ��ڵ�����
    stDstInfo.ulDstPthIndex= RCT_PTHNUM_ONLYONE;
    /*��Ϣ����*/
    /*��Ϣ����*/
    if ( VOS_ERR == RCT_API_MgtNormalSend( stDstInfo.ulSrcSubType, 
                                            stDstInfo.ulDstRctType, 
                                            stDstInfo.ulDstSubType, 
                                            stDstInfo.ulDstPthIndex,
                                            (CHAR *)pstMsgInfo, 
                                            ulMsgSize))
    {
        VOS_Printf("rct msg send message error");
        return VOS_ERR;
    }
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : RDS_MSG_TerminalInfoUpdate
 ��������  : �ն���Ϣ����
 �������  : RCTMSG_DSTINFO_S *pstDstInfo  
             CHAR*  pcTerminalID           
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��12��10��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG RDS_MSG_TerminalInfoUpdate(    CHAR*  pcTerminalID)
{
    PRDS_MSG_TMINFOADD_S pstMsgInfo = NULL;
    RCTMSG_DSTINFO_S     stDstInfo  = {0};
    ULONG                ulMsgSize  = sizeof(RDS_MSG_TMINFOADD_S);
    
    pstMsgInfo = (PRDS_MSG_TMINFOADD_S)VOS_Malloc(RDS_MID_MSG, ulMsgSize);
    if ( NULL == pstMsgInfo )
    {
        VOS_Printf("malloc error");
        return VOS_ERR;
    }
        
    pstMsgInfo->ulMsgCode  = RDS_MSGTYPE_TerminalInfoUpdate;
    
    /*��Ϣ���ݵİ�ȫ����*/
    VOS_StrCpy_S(pstMsgInfo->acTerminalID, RDS_UPF_DEVLAB_LEN-1, pcTerminalID);
    
     /*ֱ��ȷ��Ŀ��*/
    stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //һ�㶼�����
    stDstInfo.ulDstRctType = RCT_TYPE_MP_RDS;       
    stDstInfo.ulDstSubType = RCT_SUBTYPE_SINGLE;   //SSL�ж���ҵ��ڵ�����
    stDstInfo.ulDstPthIndex= RCT_PTHNUM_ONLYONE;
    /*��Ϣ����*/
    if ( VOS_ERR == RCT_API_MgtNormalSend( stDstInfo.ulSrcSubType, 
                                            stDstInfo.ulDstRctType, 
                                            stDstInfo.ulDstSubType, 
                                            stDstInfo.ulDstPthIndex,
                                            (CHAR *)pstMsgInfo, 
                                            ulMsgSize))
    {
        VOS_Printf("rct msg send message error");
        return VOS_ERR;
    }
                                            
    return VOS_OK;
}

