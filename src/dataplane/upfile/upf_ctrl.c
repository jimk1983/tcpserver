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
INT32 UPF_CtrlHandler(UPF_CONN_S *pstUpfConn, COM_IOBUF_S *pstIobuf)
{ 
    SWM_BIZ_HEAD_S* pstBizHead = NULL;
    PUPF_HEAD_S     pstUpfHead = NULL; 
    CHAR*           pcPacket        = NULL;
    UINT32          uiPackLen        =0 ;
    LONG            lRet = 0;
    ULONG           uiCtrlCode = 0;
    
    if ( NULL == pstUpfConn
            || NULL == pstIobuf )
    {
        return VOS_ERR;
    }

    pcPacket   = COM_IOBUF_GETSAVED_DATA(pstIobuf);
    uiPackLen  = COM_IOBUF_GETSAVED_LEN(pstIobuf);
    
    pstBizHead = (SWM_BIZ_HEAD_S  *)pcPacket;

    pstUpfHead = (PUPF_HEAD_S)(pcPacket + SWM_BIZ_HEAD_LEN);

    uiCtrlCode = VOS_ntohl(pstUpfHead->uiCtrlCode);
    
    VOS_Printf("UPF conn upper :recv the iobuf=%p, upf conn=%p, uiPackLen=%d, pstBizHead=%p, acDeviceLab=%s!",  
        pstIobuf, pstUpfConn, uiPackLen, pstBizHead, pstUpfHead->acDeviceLab);
    
    switch(uiCtrlCode)
    {
        case UPSER_CTLCODE_KPVINFO_REQ:
            pstUpfHead->uiCtrlCode = VOS_htonl(UPSER_CTLCODE_KPVINFO_RESP);
            
            VOS_StrCpy_S(pstUpfHead->acDeviceLab, UPF_DEVLAB_LEN, "hello world");
        
            lRet = UPF_Conn_TransBufToDownPipeNode(pstUpfConn, pstIobuf);
            if( VOS_ERR == lRet )
            {
                VOS_Printf("pipe down iobuf error!");
                COM_Iobuf_Free(pstIobuf);
                
                return VOS_ERR;
            }
            break;
        case UPSER_CTLCODE_XMLFILE_GET:
            break;
        case UPSER_CTLCODE_APPFILE_GET:
            break;
        default:
            break;
    }
        
    

    return VOS_OK;
}



