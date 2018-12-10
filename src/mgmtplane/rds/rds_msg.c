/******************************************************************************

                  版权所有 (C), 2017-2020, 网络有限公司

 ******************************************************************************
  文 件 名   : rds_ctx.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2018年8月17日
  最近修改   :
  功能描述   : redis的终端信息接口
  函数列表   :
  修改历史   :
  1.日    期   : 2018年8月17日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "rds/rds_pub.h"



/*****************************************************************************
 函 数 名: RDS_MSG_TerminalInfoAdd
 功能描述  : 添加终端信息
 输入参数  : RCTMSG_DSTINFO_S *pstDstInfo    
           pcTerminalID                     ---需要更新的TerminalID的
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月22日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
    
    /*消息内容的安全拷贝*/
    VOS_StrCpy_S(pstMsgInfo->acTerminalID, RDS_UPF_DEVLAB_LEN-1, pcTerminalID);
    VOS_StrCpy_S(pstMsgInfo->acTerminalDecptor, RDS_UPF_DEVLAB_LEN-1, pcTerminalDesptor);
    pstMsgInfo->uiClientAddr = uiCliAddr;
    pstMsgInfo->uiClientPort = uiCliPort;

    /*直接确定目的*/
    stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //一般都是这个
    stDstInfo.ulDstRctType = RCT_TYPE_MP_RDS;       
    stDstInfo.ulDstSubType = RCT_SUBTYPE_SINGLE;   //SSL有多种业务节点类型
    stDstInfo.ulDstPthIndex= RCT_PTHNUM_ONLYONE;
    
    /*消息发送*/
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
 函 数 名  : RDS_MSG_TerminalInfoDel
 功能描述  : 删除
 输入参数  : RCTMSG_DSTINFO_S *pstDstInfo  
             CHAR*  pcTerminalID           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月10日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
    
    /*消息内容的安全拷贝*/
    VOS_StrCpy_S(pstMsgInfo->acTerminalID, RDS_UPF_DEVLAB_LEN-1, pcTerminalID);
    
    /*直接确定目的*/
    stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //一般都是这个
    stDstInfo.ulDstRctType = RCT_TYPE_MP_RDS;       
    stDstInfo.ulDstSubType = RCT_SUBTYPE_SINGLE;   //SSL有多种业务节点类型
    stDstInfo.ulDstPthIndex= RCT_PTHNUM_ONLYONE;
    /*消息发送*/
    /*消息发送*/
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
 函 数 名  : RDS_MSG_TerminalInfoUpdate
 功能描述  : 终端信息更新
 输入参数  : RCTMSG_DSTINFO_S *pstDstInfo  
             CHAR*  pcTerminalID           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月10日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
    
    /*消息内容的安全拷贝*/
    VOS_StrCpy_S(pstMsgInfo->acTerminalID, RDS_UPF_DEVLAB_LEN-1, pcTerminalID);
    
     /*直接确定目的*/
    stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //一般都是这个
    stDstInfo.ulDstRctType = RCT_TYPE_MP_RDS;       
    stDstInfo.ulDstSubType = RCT_SUBTYPE_SINGLE;   //SSL有多种业务节点类型
    stDstInfo.ulDstPthIndex= RCT_PTHNUM_ONLYONE;
    /*消息发送*/
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

