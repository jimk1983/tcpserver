/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : pfw_conn.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2017年4月29日
  最近修改   :
  功能描述   : UPF节点
  函数列表   :
  修改历史   :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "vgm/vgm_pub.h"
#include "swm/swm_pub.h"
#include "fsm/fsm_pub.h"
#include "upfile/upf_pub.h"



/*****************************************************************************
 函 数 名  : UPF_Conn_DelNotify
 功能描述  : 删除通知
 输入参数  : PFW_CONN_S *pstPfwConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_Conn_DelNotify(UPF_CONN_S *pstUpfConn)
{
    SWM_PIPE_CONN_S *pstPipeNext = NULL;
    VOS_DLIST_NODE_S *pstListHead = NULL;
    
    if ( NULL == pstUpfConn )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }
    
    /*需要第一时间先删除注册的网络事件,关闭内核产生的大量错误的网络事件*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstUpfConn->stNetEvtOps) )
    {
        VOS_Printf("rct net unregister error!");
    }
    
    /*设置本节点进入老化*/
    pstUpfConn->stExpireOps.ulExpireConfirm = VOS_TRUE;
    
    /*如果已经有人设置，就不用继续通知了，
        表示已经有人在遍历通知老化了*/
    if ( pstUpfConn->pstBizChannel->ulExitConfirm == VOS_TRUE )
    {
        return VOS_ERR;
    }
    
    /*否则设置一下，防止另外节点重复通知,下面开始通知另外业务管道节点*/
    pstUpfConn->pstBizChannel->ulExitConfirm = VOS_TRUE;
    
    pstListHead = &pstUpfConn->pstBizChannel->stPipeHead;
    
    /*通知所有该管道节点进行退出*/    
    VOS_DLIST_FOR_EACH_ENTRY(pstPipeNext, pstListHead, SWM_PIPE_CONN_S, stNode)
    {
        if ( NULL == pstPipeNext )
        {
            VOS_Printf("Not found the next pipe node!");
            break;
        }

        /*本节点就是PFW节点, 要排除本节点，防止递归*/
        if ( pstPipeNext->ePipeType == SWM_PIPE_TYPE_BIZ_UPF )
        {
            /*避免自己不再进入该函数，产生递归循环,跳过*/
            continue;
        }

        /*调用下一个节点, 通知所有删除*/
        if ( NULL != pstPipeNext->stPipeDelNotifyProc.pvcbFunc ) 
        {   
            //VOS_Printf("found the next pipe node, goto delnotify!pstPipeNode=%p,data=%p", 
            //    pstPipeNext, pstPipeNext->stPipeDelNotifyProc.pvUserData);
            ((swm_pipe_delnotify_cb)pstPipeNext->stPipeDelNotifyProc.pvcbFunc)(pstPipeNext->stPipeDelNotifyProc.pvUserData);
        }
    }
    return VOS_OK;
}



/*****************************************************************************
 函 数 名  : UPF_Conn_TransBufToDownPipeNode
 功能描述  : 主动推送给下行数据
 输入参数  :   PFW_CONN_S *pstNemConn  ---当前节点
           COM_IOBUF_S *pstIobuf   ---要发送的iobuf
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月14日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_Conn_TransBufToDownPipeNode(UPF_CONN_S *pstUpfConn, COM_IOBUF_S *pstIobuf)
{
    LONG lRet = 0;

    if ( NULL == pstUpfConn
        || NULL == pstIobuf
        || NULL == pstUpfConn->pstBizChannel )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    /*查找当前节点的前一层节点，推送数据*/
    lRet = SWM_TLS_PipeTransBufToPrePipeNode(&pstUpfConn->stPipe, pstIobuf);
    if (SWM_PIPE_IOBUF_EWOULDBLOCK == lRet )
    { 
        //VOS_Printf("upf down pipe data ewould block error, TerminalID=[%s]!", pstUpfConn->acTerminalID);
        return VOS_SYS_EWOULDBLOCK;
    }
    else if (SWM_PIPE_IOBUF_OK == lRet )
    {
        //VOS_Printf("UPF stastic flows!");
        return VOS_OK;
    }
    else
    {
        //VOS_Printf("Tls connect node pipe to next node error, notify all pipe goto expire!");
        UPF_Conn_DelNotify(pstUpfConn);
    }
    
    return VOS_ERR;
}


/*****************************************************************************
 函 数 名  : UPF_Conn_PipeConnDataUpperProc
 功能描述  : 接收来自SWM的上行的数据
 输入参数  :   VOID *pvhandler              ---本节点
           COM_IOBUF_S *pstIobuf    ---来自客户端的上行的iobuf
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_Conn_PipeConnDataUpperProc(VOID *pvhandler, COM_IOBUF_S *pstIobuf)
{
    UPF_CONN_S*     pstUpfConn   = NULL;
    
    if ( NULL == pvhandler 
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return SWM_PIPE_IOBUF_PARAM_ERR;
    }
    
    pstUpfConn = (UPF_CONN_S *)pvhandler;
        
    if ( VOS_ERR == UPF_Ctrl_Handler(pstUpfConn, pstIobuf)  )
    {
        VOS_Printf("pipe down iobuf error!");
        COM_Iobuf_Free(pstIobuf);
        UPF_Conn_DelNotify(pstUpfConn);
        return SWM_PIPE_IOBUF_UNKNOW_ERR;
    }
    
    return SWM_PIPE_IOBUF_OK;                         
}


/*****************************************************************************
 函 数 名: UPF_Conn_PipeConnDataDownProc
 功能描述  : 接收后一层的下行，目前PFW没有后一层，
           提供给NEM的后一层管道进行下行调用，因此暂时没有该下行处理
 输入参数  : VOID *pvhandler        
           COM_IOBUF_S *pstIobuf  
 输出参数  : 无
 返 回 值: 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_Conn_PipeConnDataDownProc(VOID *pvhandler, COM_IOBUF_S *pstIobuf)
{
    //UPF_CONN_S *pstConn = NULL;

    if ( NULL == pvhandler 
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    //pstConn = (UPF_CONN_S *)pvhandler;

    //VOS_Printf("UPF_Conn_PipeConnDataDownProc Entry!pstConn=%p", pstConn);
    
   return VOS_OK;                         
}

/*****************************************************************************
 函 数 名  : UPF_Conn_PipeConnCtrlProc
 功能描述  : 本节点的控制处理
 输入参数  : ULONG ulCtrlCmd  
                          VOID *pvhandler  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_Conn_PipeConnCtrlProc(VOID *pvhandler, ULONG ulCtrlCmd)
{
    UPF_CONN_S *pstConn = NULL;
    
    if ( NULL == pvhandler )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    pstConn = (UPF_CONN_S *)pvhandler;
    
    //VOS_Printf("UPF_Conn_PipeConnCtrlProc: TerminalID=[%s], continue out!", pstConn->acTerminalID);

    /*如果已经进入老化，就不要再继续处理了*/
    if( VOS_TRUE == pstConn->pstBizChannel->ulExitConfirm )
    {
        /*这里不应该发送*/
        VOS_Printf("system error!");
        return VOS_ERR;
    }

    switch(ulCtrlCmd)
    {
        case SWM_CTRLCMD_SNDOUT_COMPELETED:
        {
            /*查看当前的发送情况，是否还需要继续发送*/
            if ( pstConn->uiMgrChunkStatus == UPF_TRNSTATUS_SNDING )
            {
                if( VOS_ERR == UPF_Ctrl_PipeDownData(pstConn) )
                {
                    return VOS_ERR;
                }
            }
        }
        break;
        default:
            break;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : UPF_Conn_PipeConnDelNotifyProc
 功能描述  : 删除通知
 输入参数  : VOID *pvhandler  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID UPF_Conn_PipeConnDelNotifyProc(VOID *pvhandler)
{
    UPF_CONN_S *pstConn = NULL;

    if ( NULL == pvhandler )
    {
        VOS_Printf("param error!");
        return;
    }

    pstConn = (UPF_CONN_S *)pvhandler;
    
    //VOS_Printf("UPF_Conn_PipeConnDelNotifyProc Entry!");
    
    UPF_Conn_DelNotify(pstConn);

    return;                         
}

/*****************************************************************************
 函 数 名  : UPF_Conn_ExpireCb
 功能描述  : 老化具体处理
 输入参数  : VOID *pvPfwConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月14日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID UPF_Conn_ExpireCb(VOID *pstUpfConn)
{
    UPF_CONN_S *pstUPFConn = NULL;
    
    if ( NULL == pstUpfConn )
    {
        VOS_Printf("param error!");
        return;
    }

    pstUPFConn =(UPF_CONN_S *)pstUpfConn;
    
    //VOS_Printf("UPF connect node goto expiring...!");
    
    if( VOS_TRUE != pstUPFConn->stExpireOps.ulExpireConfirm )
    {
        /*这里不应该发送*/
        VOS_Printf("system error!");
        return;
    }
    
    (VOID)UPF_Conn_Release(pstUPFConn);

    return;
}


/*****************************************************************************
 函 数 名  : UPF_Conn_Create
 功能描述  : 网络扩展节点的创建
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
LONG UPF_Conn_Create(SWM_BIZ_CHANNEL_S *pstBizChannel)
{
    UPF_CONN_S *pstConn = NULL;

    if ( NULL == pstBizChannel )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    pstConn = (UPF_CONN_S *)VOS_Malloc(PFW_MID_SID_CON, sizeof(UPF_CONN_S));
    if ( NULL == pstConn )
    {
        VOS_Printf("VOS_Malloc error!");
        return VOS_ERR;
    }
    
    pstConn->pstBizChannel = pstBizChannel;
        
    /*将Pipe加入到BizChannel中*/
    VOS_Mem_Zero((CHAR *)&pstConn->stPipe,sizeof(SWM_PIPE_CONN_S));

    VOS_DLIST_INIT(&pstConn->stPipe.stNode);
    
    /*初始化网络扩展的Pipe*/
    SWM_PIPE_CONN_INIT(
            &pstConn->stPipe,
            SWM_PIPE_TYPE_BIZ_UPF,
            UPF_Conn_PipeConnDataUpperProc,             /*上行数据*/
            UPF_Conn_PipeConnDataDownProc,              /*下行数据*/
            UPF_Conn_PipeConnCtrlProc,                  /*本管道的控制*/
            UPF_Conn_PipeConnDelNotifyProc,             /*删除处理*/
            pstConn);
    
    /*将其挂在BizChannel通道的管道链表中*/
    VOS_DLIST_ADD_TAIL(&(pstBizChannel->stPipeHead), &pstConn->stPipe.stNode);

    if ( VOS_ERR == RCT_API_ExpireOpsEventRegister(&pstConn->stExpireOps, UPF_Conn_ExpireCb, pstConn) )
    {
        VOS_Printf("expire register error!");
        VOS_Free((CHAR *)pstConn);
        return VOS_ERR;
    } 

    pstConn->uiMgrChunkStatus = UPF_TRNSTATUS_SNDEND;

                
    //VOS_Printf("UPF Conn Create OK");

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : UPF_Conn_Release
 功能描述  : 释放
 输入参数  : NEM_CONN_S *pstNemConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月14日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG UPF_Conn_Release(UPF_CONN_S *pstConn)
{
    ULONG   ulVtID = 0;

    if ( NULL == pstConn )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }
    
    pstConn->pstBizChannel->ulExitConfirm  = VOS_TRUE;

    ulVtID = pstConn->pstBizChannel->stVtInfo.ulVTID;
    
    VOS_Printf("UPF Connect Release Entry! TerminalID=[%s], CurrConnNums=%d", 
                    pstConn->acTerminalID, 
                    VGM_CFG_GatewayConnGetNums(ulVtID));

    /*重要: BizChannel 中脱离本节点*/
    VOS_DLIST_DEL(&pstConn->stPipe.stNode);

    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstConn->stNetEvtOps) )
    {
        VOS_Printf("rct net unregister error!");
    }

    if(VOS_ERR == RCT_API_ExpireOpsEventUnRegister(&pstConn->stExpireOps))
    {
        VOS_Printf("RCT_Reactor_ExpireOptsUnRegister error!");
    }

    if ( NULL != pstConn->pstRcvIobuf )
    {
        COM_Iobuf_Free(pstConn->pstRcvIobuf);
        pstConn->pstRcvIobuf = NULL;
    }
    
    VOS_Free((CHAR *)pstConn);

    pstConn = NULL;

    return VOS_OK;
}


