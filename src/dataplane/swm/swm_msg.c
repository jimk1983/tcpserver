/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_msg.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : ��Ϣ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include "common.h"
#include "swm/swm_pub.h"
    

/*****************************************************************************
 �� �� ��  : SWM_MSG_AddTlsConn
 ��������  : ���TLS�ڵ�
 �������  : RCTMSG_DSTINFO_S *pstDstInfo  
             LONG lConnfd                  
             ULONG ulVtID                  
             SSL_CTX_S *pstSSLCtx          
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��22��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_MSG_AddTlsConn(  RCTMSG_DSTINFO_S *pstDstInfo, 
                               LONG lConnfd,
                               ULONG ulVtID)
{
    SWM_MSG_ADDCONN_S *pstMsgInfo = NULL;
    ULONG   ulMsgSize = sizeof(SWM_MSG_ADDCONN_S);
    

    if ( NULL == pstDstInfo)
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    pstMsgInfo = (SWM_MSG_ADDCONN_S *)VOS_Malloc(VGM_MID_SID_SSL, ulMsgSize);
    if ( NULL == pstMsgInfo )
    {
        VOS_Printf("malloc error");
        return VOS_ERR;
    }
        
    pstMsgInfo->ulMsgCode  = SWM_MSGTYPE_ADDTLSCONN;
    
    /*��Ϣ���ݵİ�ȫ����*/
    pstMsgInfo->lConnfd = lConnfd;
    pstMsgInfo->ulVtID  = ulVtID;
    //pstMsgInfo->pstSSLCtx = pstSSLCtx;
    
    /*��Ϣ����*/
    if ( VOS_ERR == RCT_API_MgtNormalSend( pstDstInfo->ulSrcSubType, 
                                            pstDstInfo->ulDstRctType, 
                                            pstDstInfo->ulDstSubType, 
                                            pstDstInfo->ulDstPthIndex,
                                            (CHAR *)pstMsgInfo, 
                                            ulMsgSize))
    {
        VOS_Printf("rct msg send message error");
        return VOS_ERR;
    }
    
    return VOS_OK;
}
                                  


