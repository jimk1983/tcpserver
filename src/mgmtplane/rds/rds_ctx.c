/******************************************************************************

                  版权所有 (C), 2017-2020, 网络有限公司

 ******************************************************************************
  文 件 名   : rds_ctx.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2018年8月17日
  最近修改   :
  功能描述   : redis的上下文接口
  函数列表   :
  修改历史   :
  1.日    期   : 2018年8月17日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "rds/rds_pub.h"


PRDS_CTX_S g_pstRdsCtx = NULL;

/*****************************************************************************
 函 数 名  : RDS_CtxMsgHandler
 功能描述  : 消息处理
 输入参数  : RCT_MSG_HEAD_S *pstHead  
             CHAR *pcMgtData          
             UINT32 uiLen             
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月10日
    作    者   : 蒋康
    修改内容   : 新生成函数

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

    /*获取消息码*/
    ulMsgType = *((ULONG *)pcMgtData);
        
    VOS_Printf("Redis message handler: message type=%d", ulMsgType);
   
    /*网关创建相关的消息*/
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
 函 数 名  : RDS_CtxTaskInit
 功能描述  : 线程任务初始化
 输入参数  : VOID *pvArg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年8月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID RDS_CtxTaskInit(VOID *pvArg)
{
   /*注册消息接收处理函数*/
    if(VOS_ERR == RCT_API_MgtHandlerRegister(RCT_TYPE_MP_RDS, RCT_SUBTYPE_SINGLE, "RDS",  RDS_CtxMsgHandler))
    {
        VOS_PRINT("RDS mgt register error!!");
        return;
    }
}

/*****************************************************************************
 函 数 名  : RDS_CtxTaskUnInit
 功能描述  : 任务出错时，资源释放
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年8月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID RDS_CtxTaskUnInit(VOID *argv)
{
    RCT_API_MgtHandlerUnRegister(RCT_TYPE_MP_RDS, RCT_SUBTYPE_SINGLE); 
}


/*****************************************************************************
 函 数 名  : RDS_CtxInit
 功能描述  : RDS上下文环境初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年8月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG RDS_CtxInit()
{
    /*将资源初始化*/
    g_pstRdsCtx = (PRDS_CTX_S)malloc(sizeof(RDS_CTX_S));
    if ( NULL == g_pstRdsCtx )
    {
        VOS_Printf("RDS ctx malloc error!\n");
        return VOS_ERR;
    }
    memset(g_pstRdsCtx, 0, sizeof(RDS_CTX_S));

    /*默认初始化当前的Redis服务器*/
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
 函 数 名  : RDS_CtxUnInit
 功能描述  : RDS上下文环境初始化
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

