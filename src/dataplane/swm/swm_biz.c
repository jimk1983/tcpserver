/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_biz.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月4日
  最近修改   :
  功能描述   : bizchannel业务通道管理
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月4日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "swm/swm_pub.h"


/*业务识别匹配注册*/
static SWM_BIZ_Match_S *  g_pstSwmBizMatch = NULL;

/*****************************************************************************
 函 数 名  : SWM_Biz_ChannelCreate
 功能描述  : 业务通道节点创建
 输入参数  : LONG lSockfd               ---当前业务节点所属的socketfd
 输出参数  : 无
 返 回 值  : SWM_BIZ_CHANNEL_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

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
    
    /*获取客户端的地址信息*/
    if( VOS_ERR == VOS_SOCK_GetPeerSockAddr(lSockfd, acAddr, &pstBizChannel->stClientInfo.ulClientPort))
    {
        VOS_Printf("get peer sock addr error!");
        VOS_Free((CHAR *)pstBizChannel);
        return NULL;
    }
    
    /* 节省空间，以后Hash提高性能和利用率*/
    pstBizChannel->stClientInfo.ulClientAddr = VOS_aton(acAddr);
    
    /* 推送节点源头*/
    VOS_DLIST_INIT(&pstBizChannel->stNode);
    VOS_DLIST_INIT(&pstBizChannel->stPipeHead);

    /*发送队列*/
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
 函 数 名  : SWM_Biz_ChannelRelease
 功能描述  : 释放业务通道
 输入参数  : SWM_BIZ_CHANNEL_S *pstBizChannel  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

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
 函 数 名  : SWM_Biz_ChannelMatch
 功能描述  :   Biz业务匹配识别
 输入参数  :   SWM_BIZ_CHANNEL_S *pstBizChannel  ---业务通道
                             CHAR *pcData            ---首包数据                      
                             ULONG ulDataLen       ---数据长度
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

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
            /*匹配函数: pvUserData就是每个业务的CTX，例如:pvNemCtx*/
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
 函 数 名  : SWM_Biz_ChannelMatchInit
 功能描述  : SSL 的SWM的业务上下文全局结构, 注意该是线程变量的初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_Biz_ChannelMatchInit()
{
    /*该种情况也返回VOS_OK，在任务初始化时候防止多次创建*/
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
 函 数 名  : SWM_Biz_ChannelMatchUnInit
 功能描述  : 释放资源
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID SWM_Biz_ChannelMatchUnInit()
{
    if ( NULL != g_pstSwmBizMatch )
    {
        VOS_Free((CHAR *)g_pstSwmBizMatch);
    }
}

/*****************************************************************************
 函 数 名  : SWM_Biz_ChannelMatchRegister
 功能描述  : SWM后面的业务识别匹配函数注册
 输入参数  : SWM_BIZ_MATCH_CTX_S *pstBizMatch  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_Biz_ChannelMatchRegister(SWM_BIZ_MATCH_CTX_S *pstBizMatch)
{
    if ( NULL == g_pstSwmBizMatch
        || NULL == pstBizMatch )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    /*1. 需要添加NEM_CTX_BizMatch*/
    g_pstSwmBizMatch->stArrayBizTypeMatch[pstBizMatch->ulBizType].pvFunc      = pstBizMatch->pvFunc;
    g_pstSwmBizMatch->stArrayBizTypeMatch[pstBizMatch->ulBizType].pvCtx        = pstBizMatch->pvCtx;
    g_pstSwmBizMatch->stArrayBizTypeMatch[pstBizMatch->ulBizType].ulBizType  = pstBizMatch->ulBizType;
    
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : SWM_Biz_ChannelMatchUnRegister
 功能描述  : 去注册函数
 输入参数  : ULONG ulBizType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

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
 函 数 名  : SWM_Biz_ChannelCheckLen
 功能描述  : 检查包的长度
 输入参数  : CHAR *pcPack      
             UINT32 uiPackLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月10日
    作    者   : jimk
    修改内容   : 新生成函数

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
 函 数 名  : SWM_Biz_ChannelPreGetPackLen
 功能描述  : 获取当前要接收的包大小
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 新生成函数

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

    /*当前该包的大小*/
    lPackLen = sizeof(SWM_BIZ_HEAD_S) + VOS_ntohl(pstSwmBiz->uiDataLen);

    return lPackLen;
}
