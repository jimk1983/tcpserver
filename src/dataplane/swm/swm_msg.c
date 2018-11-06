/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_msg.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月4日
  最近修改   :
  功能描述   : 消息发送
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月4日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "swm/swm_pub.h"
    

/*****************************************************************************
 函 数 名  : SWM_MSG_AddTlsConn
 功能描述  : 添加TLS节点
 输入参数  : RCTMSG_DSTINFO_S *pstDstInfo  
             LONG lConnfd                  
             ULONG ulVtID                  
             SSL_CTX_S *pstSSLCtx          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月22日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
    
    /*消息内容的安全拷贝*/
    pstMsgInfo->lConnfd = lConnfd;
    pstMsgInfo->ulVtID  = ulVtID;
    //pstMsgInfo->pstSSLCtx = pstSSLCtx;
    
    /*消息发送*/
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
                                  


