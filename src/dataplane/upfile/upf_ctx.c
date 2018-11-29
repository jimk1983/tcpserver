/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : pfw_ctx.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2017年4月29日
  最近修改   :
  功能描述   : portforward
  函数列表   :
  修改历史   :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "swm/swm_pub.h"
#include "fsm/fsm_pub.h"
#include "upfile/upf_pub.h"

UPF_CTX_S *g_pstUPFCtx = NULL;


/*****************************************************************************
 函 数 名  : UPF_CtxBizMatch
 功能描述  :    UpFile 文件升级业务识别,识别后会新建一个UPF的业务管道节点
 输入参数  :    SWM_BIZ_CHANNEL_S *pstBizChannel  --业务通道，管道在里面
            CHAR *pcData                      --数据
            ULONG ulDataLen                   --数据长度
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
ULONG UPF_CtxBizMatch(SWM_BIZ_CHANNEL_S *pstBizChannel, CHAR *pcData, ULONG ulDataLen)
{
    SWM_BIZ_HEAD_S *pstUpfHead = NULL;

    if ( NULL == pstBizChannel
        || NULL == pcData )
    {
        VOS_Printf("param error");
        return EMPTO_BIZTYPEID_UNKNOW;
    }

    pstUpfHead = (SWM_BIZ_HEAD_S *)pcData;

    /*业务特征码匹配*/
    if ( UPF_PROTO_MARK1 == VOS_ntohl(pstUpfHead->uiMark1)
        && UPF_PROTO_MARK2 == VOS_ntohl(pstUpfHead->uiMark2) ) 
    {
        /*匹配后，创建本通道的UPF节点*/
        if ( VOS_ERR == UPF_Conn_Create(pstBizChannel) )
        {
            VOS_Printf("pfw connect node create error!");
            return EMPTO_BIZTYPEID_UNKNOW;
        }
        pstBizChannel->ulPipeNum++;
        VOS_Printf("pfw biz type has been matched and create nem node OK!");
        return EMPTO_BIZTYPEID_UPF;
    }
    
    return EMPTO_BIZTYPEID_UNKNOW;
}



/*****************************************************************************
 函 数 名  : UPF_Ini_CtxCreate
 功能描述  : UPF的上下文
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_CtxCreate()
{
    SWM_BIZ_MATCH_CTX_S  stBizCtx = {0};

    /*有多个SSL线程，所以要添加判断，不能创建多个*/
    if (NULL == g_pstUPFCtx)
    {
        g_pstUPFCtx = (UPF_CTX_S *)VOS_Malloc(PFW_MID_SID_INI, sizeof(UPF_CTX_S));
        if ( NULL == g_pstUPFCtx  )
        {
            VOS_Printf("param error");
            return VOS_ERR;
        }

        VOS_Mem_Zero((CHAR *)g_pstUPFCtx, sizeof(UPF_CTX_S));

        /*注册前赋值*/
        stBizCtx.ulBizType = EMPTO_BIZTYPEID_UPF;
        stBizCtx.pvFunc =  (VOID*)UPF_CtxBizMatch;
        stBizCtx.pvCtx   = g_pstUPFCtx;

        /*业务识别注册，在SWM模块中识别*/    
        if ( VOS_ERR == SWM_Biz_ChannelMatchRegister(&stBizCtx))
        {
            VOS_Printf("swm biz channel match register error");
            VOS_Free((CHAR *)g_pstUPFCtx);
            return VOS_ERR;
        }
    }
    
   
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : UPF_CtxRelease
 功能描述  : 释放上下文
 输入参数  : UPF_CTX_S *pstCtx  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID UPF_CtxRelease()
{
    if(NULL != g_pstUPFCtx)
    {
        (VOID)SWM_Biz_ChannelMatchUnRegister(EMPTO_BIZTYPEID_UPF);
        VOS_Free((CHAR *)g_pstUPFCtx);
    }
}


/*****************************************************************************
 函 数 名  : UPF_CtxTaskInit
 功能描述  : PFW的任务资源初始化
 输入参数  : VOID *pvArg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID UPF_CtxTaskInit(VOID *pvArg)
{
    /*业务上下文初始化*/
    if ( VOS_ERR == UPF_CtxCreate() )
    {
        VOS_Printf("UPF ctx create!!");
        return;
    }

    VOS_Printf("UPF ctx task initialize OK!");
}


/*****************************************************************************
 函 数 名  : UPF_CtxTaskUnInit
 功能描述  : 资源释放
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID UPF_CtxTaskUnInit(VOID *argv)
{
    
    UPF_CtxRelease();
}

/*****************************************************************************
 函 数 名  : PFW_CtxInit
 功能描述  : 端口转发模块的初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_CtxInit()
{
    /*先注册上下文初始化*/
    if ( VOS_ERR == RCT_API_EnvTaskInitRegister(UPF_CtxTaskInit, NULL, RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_UPF, UPF_CtxTaskUnInit) )
    {
        VOS_Printf("UPF ctx inti register error!!");
        return VOS_ERR;
    }
    
    VOS_Printf("UPF Ctx init .......................OK!");
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : PFW_CtxUnInit
 功能描述  : 去初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID UPF_CtxUnInit()
{
    RCT_API_EnvTaskInitUnRegister(RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_UPF);
}

