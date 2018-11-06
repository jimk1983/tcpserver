/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : pfw_ctrl.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2017年4月29日
  最近修改   :
  功能描述   : 端口转发控制模块
  函数列表   :
  修改历史   :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/



#include "common.h"
#include "swm/swm_pub.h"
#include "upfile/upf_pub.h"


/*数据流返回时候的组装报文*/
VOID UPF_Ctrl_MakeCommBizHeadLen(COM_IOBUF_S *pstIobuf, UINT32 uiLocalSockfd, UINT32 uiStreamLen)
{
    UPF_HEAD_S      *pstBizHead = NULL;
    
    if  (NULL == pstIobuf)
    {
        return;
    }

    pstBizHead = (UPF_HEAD_S *)(pstIobuf->pcData);

    VOS_Printf("upf ctrl make pstBizhead=%p", pstBizHead);
  
    return;
}

/*****************************************************************************
 函 数 名  : UPF_CtrlHandler
 功能描述  : 控制处理
 输入参数  : PFW_CONN_S *pstPFWConn    
             PFW_CTRL_INFO_S *pstCtrl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年5月21日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
INT32 UPF_CtrlHandler(UPF_CONN_S *pstPFWConn)
{
    if ( NULL == pstPFWConn)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}



