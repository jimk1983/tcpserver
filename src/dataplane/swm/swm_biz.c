/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_biz.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : bizchannelҵ��ͨ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include "common.h"
#include "swm/swm_pub.h"


/*ҵ��ʶ��ƥ��ע��*/
static SWM_BIZ_Match_S *  g_pstSwmBizMatch = NULL;

/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelCreate
 ��������  : ҵ��ͨ���ڵ㴴��
 �������  : LONG lSockfd               ---��ǰҵ��ڵ�������socketfd
 �������  : ��
 �� �� ֵ  : SWM_BIZ_CHANNEL_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
SWM_BIZ_CHANNEL_S *SWM_Biz_ChannelCreate(LONG lSockfd)
{
    SWM_BIZ_CHANNEL_S *pstBizChannel = NULL;
    CHAR acAddr[VOS_IPV4STR_MAXLEN] = {0};

    pstBizChannel = (SWM_BIZ_CHANNEL_S *)VOS_Malloc(SWM_MID_SID_BIZ, sizeof(SWM_BIZ_CHANNEL_S));
    if ( NULL == pstBizChannel )
    {
        VOS_Printf("VOS_Malloc error!");
        return NULL;
    }
    
    VOS_Mem_Zero((CHAR *)pstBizChannel, sizeof(SWM_BIZ_CHANNEL_S));
    
    /*��ȡ�ͻ��˵ĵ�ַ��Ϣ*/
    if( VOS_ERR == VOS_SOCK_GetPeerSockAddr(lSockfd, acAddr, &pstBizChannel->stClientInfo.ulClientPort))
    {
        VOS_Printf("get peer sock addr error!");
        VOS_Free((CHAR *)pstBizChannel);
        return NULL;
    }
    
    /* ��ʡ�ռ䣬�Ժ�Hash������ܺ�������*/
    pstBizChannel->stClientInfo.ulClientAddr = VOS_aton(acAddr);
    
    /* ���ͽڵ�Դͷ*/
    VOS_DLIST_INIT(&pstBizChannel->stNode);
    VOS_DLIST_INIT(&pstBizChannel->stPipeHead);

    /*���Ͷ���*/
    pstBizChannel->pstSwmSendQueue = COM_Iobuf_QueCreate();
    if(NULL == pstBizChannel->pstSwmSendQueue )
    {
        VOS_Printf("send queue create error!");
        VOS_Free((CHAR *)pstBizChannel);
        return NULL;
    }
    
    pstBizChannel->ulPipeNum++;
    pstBizChannel->ulExitConfirm = VOS_FALSE;
    pstBizChannel->ulBindFlags = VOS_FALSE;
    
    VOS_Printf("Create the Biz channel node ok, sockfd=%d, client addr=[%s:%d]",
        lSockfd, acAddr, pstBizChannel->stClientInfo.ulClientPort);
    
    return pstBizChannel;
}


/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelRelease
 ��������  : �ͷ�ҵ��ͨ��
 �������  : SWM_BIZ_CHANNEL_S *pstBizChannel  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID SWM_Biz_ChannelRelease(SWM_BIZ_CHANNEL_S *pstBizChannel)
{
    if ( NULL == pstBizChannel )
    {
        VOS_Printf("param error!");
        return;
    }

    COM_Iobuf_QueFree(pstBizChannel->pstSwmSendQueue);
    VOS_Free((CHAR *)pstBizChannel);
    
    return;
}

/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelMatch
 ��������  :   Bizҵ��ƥ��ʶ��
 �������  :   SWM_BIZ_CHANNEL_S *pstBizChannel  ---ҵ��ͨ��
                             CHAR *pcData            ---�װ�����                      
                             ULONG ulDataLen       ---���ݳ���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_Biz_ChannelMatch(SWM_BIZ_CHANNEL_S *pstBizChannel, CHAR *pcData, ULONG ulDataLen)
{
    ULONG ulIndex     = 0;
    ULONG ulBizType  = EMPTO_BIZTYPEID_UNKNOW;
    swm_biz_match_func pfcb = NULL;

    
    if ( NULL == pstBizChannel 
        || NULL == pcData )
    {
        VOS_Printf("param error!");
        return EMPTO_BIZTYPEID_UNKNOW;
    }
    
    VOS_Printf("SWM_CTX_BizMatch Start!pstBizChannel=%p, ulDataLen=%d", 
        pstBizChannel, ulDataLen);
    
    for(ulIndex=0; ulIndex < EMPTO_BIZTYPEID_NUMS; ulIndex++)
    {
          pfcb = g_pstSwmBizMatch->stArrayBizTypeMatch[ulIndex].pvFunc;
          if ( NULL != pfcb  )
          {
            /*ƥ�亯��: pvUserData����ÿ��ҵ���CTX������:pvNemCtx*/
            ulBizType = ((swm_biz_match_func)g_pstSwmBizMatch->stArrayBizTypeMatch[ulIndex].pvFunc)(
                pstBizChannel, 
                pcData,
                ulDataLen);
            if ( EMPTO_BIZTYPEID_UNKNOW == ulBizType )
            {
                continue;
            }
            else
            {
                pstBizChannel->ulBizType = ulBizType;
                break;
            }
        }
    }
    VOS_Printf("Match the [%d] Ctx bizness!", ulIndex);
    return ulBizType;
}


/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelMatchInit
 ��������  : SSL ��SWM��ҵ��������ȫ�ֽṹ, ע������̱߳����ĳ�ʼ��
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
LONG SWM_Biz_ChannelMatchInit()
{
    /*�������Ҳ����VOS_OK���������ʼ��ʱ���ֹ��δ���*/
    if ( NULL != g_pstSwmBizMatch )
    {
        VOS_Printf("biz channel match has created");
        return VOS_OK;
    }
        
    g_pstSwmBizMatch = (SWM_BIZ_Match_S  *)VOS_Malloc(SWM_MID_SID_BIZ, sizeof(SWM_BIZ_Match_S));
    if ( NULL == g_pstSwmBizMatch )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    VOS_Mem_Zero((CHAR *)g_pstSwmBizMatch, sizeof(SWM_BIZ_Match_S));

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelMatchUnInit
 ��������  : �ͷ���Դ
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
VOID SWM_Biz_ChannelMatchUnInit()
{
    if ( NULL != g_pstSwmBizMatch )
    {
        VOS_Free((CHAR *)g_pstSwmBizMatch);
    }
}

/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelMatchRegister
 ��������  : SWM�����ҵ��ʶ��ƥ�亯��ע��
 �������  : SWM_BIZ_MATCH_CTX_S *pstBizMatch  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_Biz_ChannelMatchRegister(SWM_BIZ_MATCH_CTX_S *pstBizMatch)
{
    if ( NULL == g_pstSwmBizMatch
        || NULL == pstBizMatch )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    /*1. ��Ҫ���NEM_CTX_BizMatch*/
    g_pstSwmBizMatch->stArrayBizTypeMatch[pstBizMatch->ulBizType].pvFunc      = pstBizMatch->pvFunc;
    g_pstSwmBizMatch->stArrayBizTypeMatch[pstBizMatch->ulBizType].pvCtx        = pstBizMatch->pvCtx;
    g_pstSwmBizMatch->stArrayBizTypeMatch[pstBizMatch->ulBizType].ulBizType  = pstBizMatch->ulBizType;
    
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelMatchUnRegister
 ��������  : ȥע�ắ��
 �������  : ULONG ulBizType  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_Biz_ChannelMatchUnRegister(ULONG ulBizType)
{
    if ( NULL == g_pstSwmBizMatch )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }
    
    g_pstSwmBizMatch->stArrayBizTypeMatch[ulBizType].pvFunc      =  NULL;
    g_pstSwmBizMatch->stArrayBizTypeMatch[ulBizType].pvCtx        =  NULL;
    g_pstSwmBizMatch->stArrayBizTypeMatch[ulBizType].ulBizType   =  EMPTO_BIZTYPEID_UNKNOW;
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelCheckLen
 ��������  : �����ĳ���
 �������  : CHAR *pcPack      
             UINT32 uiPackLen  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��10��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_Biz_ChannelCheckLen(CHAR *pcPack, UINT32 uiPackLen)
{
    SWM_BIZ_HEAD_S *pstSwmBiz = (SWM_BIZ_HEAD_S *)pcPack;
    ULONG           lPackLen = 0;
    
    if ( NULL == pcPack 
        || uiPackLen < SWM_BIZ_HEAD_LEN )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    lPackLen = sizeof(SWM_BIZ_HEAD_S) + VOS_ntohl(pstSwmBiz->uiDataLen);

    if (  uiPackLen == lPackLen )
    {
        VOS_Printf("Len check OK");
        return VOS_OK;
    }

    if ( uiPackLen < lPackLen  )
    {
        VOS_Printf("Len check recv continue");
        return VOS_SYS_EWOULDBLOCK;
    }
    
    VOS_Printf("packet check len error!, packlen=%d,  head ctrl len=%d", uiPackLen, VOS_ntohs(pstSwmBiz->uiDataLen));
    return VOS_ERR;
}

/*****************************************************************************
 �� �� ��  : SWM_Biz_ChannelPreGetPackLen
 ��������  : ��ȡ��ǰҪ���յİ���С
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_Biz_ChannelPreGetPackLen(CHAR *pcPack)
{
    LONG            lPackLen  = 0;
    SWM_BIZ_HEAD_S* pstSwmBiz = (SWM_BIZ_HEAD_S *)pcPack;

    if ( NULL == pcPack  )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    /*��ǰ�ð��Ĵ�С*/
    lPackLen = sizeof(SWM_BIZ_HEAD_S) + VOS_ntohl(pstSwmBiz->uiDataLen);

    return lPackLen;
}
