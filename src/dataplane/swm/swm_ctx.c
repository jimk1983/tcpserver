/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_ctx.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月4日
  最近修改   :
  功能描述   : SWM任务模块的上下文
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月4日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"


/*****************************************************************************
 函 数 名  : Swm_CtxMsgHandler
 功能描述  : switch业务识别切换模块的消息处理
 输入参数  : CHAR *pcMgtData  ---接收到的消息
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID Swm_CtxMsgHandler(RCT_MSG_HEAD_S *pstHead, CHAR *pcMgtData, UINT32 uiLen)
{
    ULONG   ulMsgType = SWM_MSGTYPE_UNKNOW;
    if ( NULL == pstHead
        || NULL == pcMgtData)
    {
        VOS_Printf("param error!");
        return;
    }

    /*获取消息码*/
    ulMsgType = *((ULONG *)pcMgtData);

    VOS_Printf("SWM ctx handler msg type=%d!", ulMsgType);
   
    /*网关创建相关的消息*/
    switch(ulMsgType)
   {
        /*添加SSL业务节点*/
        case SWM_MSGTYPE_ADDTLSCONN:
            if ( VOS_ERR == SWM_TLS_ConnCreate((SWM_MSG_ADDCONN_S *)pcMgtData) )
            {
                VOS_Printf("Vgm listen swm conn-node create error!");
            }
            break;
        default:
            break;
    }
}

/*****************************************************************************
 函 数 名  : SWM_Ini_CtxInit
 功能描述  : switch业务识别切换模块初始化注册
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
VOID SWM_CtxTaskInit(VOID *pvArg)
{
   
    /*业务通道注册创建, 本任务初始化需要在NEM/TCM等之前创建*/
    if ( VOS_ERR == SWM_Biz_ChannelMatchInit()  )
    {
        VOS_Printf("SWM channel match init error!!");
        return;
    }

    /*注册消息接收处理函数*/
    if(VOS_ERR == RCT_API_MgtHandlerRegister(RCT_TYPE_DP_SSL, RCT_SUBTYPE_SSL_SWM, "SWM",  Swm_CtxMsgHandler))
    {
        VOS_Printf("SWM mgt register error!!");
        SWM_Biz_ChannelMatchUnInit();
        return;
    }
}
    
/*****************************************************************************
 函 数 名  : SWM_Ini_CtxTaskUnInit
 功能描述  : SWM的业务任务资源释放
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
VOID SWM_CtxTaskUnInit()
{    
    SWM_Biz_ChannelMatchUnInit();

    /*注册本线程的消息接收信息*/
    RCT_API_MgtHandlerUnRegister(RCT_TYPE_DP_SSL, RCT_SUBTYPE_SSL_SWM); 
}


/*****************************************************************************
 函 数 名  : SWM_Ini_CtxInit
 功能描述  : switch业务识别切换模块初始化注册
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
LONG SWM_CtxInit()
{
    /*先注册上下文初始化*/
    if ( VOS_ERR == RCT_API_EnvTaskInitRegister(SWM_CtxTaskInit, NULL, RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_SWM,  SWM_CtxTaskUnInit) )
    {
        VOS_Printf("Swm ctx inti register error!!");
        return VOS_ERR;
    }
    
    VOS_Printf("SWM init ...........................OK!");
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : SWM_Ini_CtxUnInit
 功能描述  : 任务初始化
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
VOID SWM_CtxUnInit()
{
    RCT_API_EnvTaskInitUnRegister(RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_SWM);
}


