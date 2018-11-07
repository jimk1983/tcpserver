/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_tls.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月4日
  最近修改   :
  功能描述   : TLS的发送和接收
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月4日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"


/*****************************************************************************
 函 数 名  : SWM_TLS_ConnExpireConfirm
 功能描述  : 给本节点打上老化标记
 输入参数  : SWM_TLS_CONN_S *pstTlsConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID SWM_TLS_ConnExpireConfirm(SWM_TLS_CONN_S *pstTlsConn)
{
    if ( NULL == pstTlsConn )
    {
        VOS_Printf("param error");
        return;
    }

    /*设置确认可以老化*/
    pstTlsConn->stExpireOps.ulExpireConfirm = VOS_TRUE;
    
    return;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_DelNotify
 功能描述  : 通知所有业务删除
 输入参数  : SWM_BIZ_CHANNEL_S  *pstBizChannel  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月6日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID SWM_TLS_ConnDelNotify(SWM_TLS_CONN_S *pstTlsConn)
{
    SWM_PIPE_CONN_S *pstPipeNext = NULL;
    VOS_DLIST_NODE_S *pstListHead = NULL;
    
    if ( NULL == pstTlsConn )
    {
        VOS_Printf("param error");
        return;
    }
    
    /*需要第一时间先删除该注册网络事件,
    关闭内核产生的大量错误的网络事件
    其余网络节点也要这么处理*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps) )
    {
        VOS_Printf("rct net unregister error!");
    }
    
    /*设置本节点进入老化*/
    SWM_TLS_ConnExpireConfirm(pstTlsConn);
    
    /*如果已经有人设置，就不用继续通知了，
        表示已经有人在遍历通知老化了*/
    if ( pstTlsConn->pstBizChannel->ulExitConfirm == VOS_TRUE )
    {
        return;
    }
    
    /*否则设置一下，防止有人再继续通知,下面开始通知所有业务管道节点*/
    pstTlsConn->pstBizChannel->ulExitConfirm = VOS_TRUE;
    
    pstListHead = &pstTlsConn->pstBizChannel->stPipeHead;
    
    /*通知所有该管道节点进行退出*/    
    VOS_DLIST_FOR_EACH_ENTRY(pstPipeNext, pstListHead, SWM_PIPE_CONN_S, stNode)
    {
        if ( NULL == pstPipeNext )
        {
            VOS_Printf("Not found the next pipe node!");
            break;
        }

        /*本节点就是TLS源节点*/
        if ( pstPipeNext->ePipeType == SWM_PIPE_TYPE_SRC_TLS )
        {
            /*避免自己不再进入该函数，产生递归循环*/
            continue;
        }

        /*调用下一个节点, 通知所有删除*/
        if ( NULL != pstPipeNext->stPipeDelNotifyProc.pvcbFunc ) 
        {   
            VOS_Printf("found the next pipe node, Goto Delnotify!pstPipeNode=%p,data=%p", 
                pstPipeNext, pstPipeNext->stPipeDelNotifyProc.pvUserData);
            ((swm_pipe_delnotify_cb)pstPipeNext->stPipeDelNotifyProc.pvcbFunc)(pstPipeNext->stPipeDelNotifyProc.pvUserData);
        }
    }
    
    return;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_ConnRecvCb
 功能描述  : 接收网络事件处理
 输入参数  : VOID *pvTlsConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID SWM_TLS_ConnRecvCb(VOID *pvTlsConn)
{
    SWM_TLS_CONN_S        *pstTlsConn = NULL;
    SWM_BIZ_CHANNEL_S     *pstBizChannel = NULL;
    CHAR                  *pcData = NULL;
    LONG lErrorStatus = 0;
    LONG lError          = 0;
    LONG lRecvLen     = 0;
    LONG lRet            = 0;
    LONG lLeftDataLen     = 0;
    LONG lDataLen = 0;
    
    if ( NULL == pvTlsConn )
    {
        VOS_Printf("tls-con is NULL!");
        return;
    }

    pstTlsConn = (SWM_TLS_CONN_S *)pvTlsConn;

    pstBizChannel = pstTlsConn->pstBizChannel;
    if ( NULL == pstBizChannel )
    {
        VOS_Printf("system error!");
        return;
    }

    VOS_Printf("SWM_TLS_ConnRecvCb recvfd=%d", pstTlsConn->lConnfd);
    
    /*这里会重新创建一个iobuf,
       iobuf就随着管道一级一级的往后推送处理, 最后一个需要释放*/
    if ( NULL == pstTlsConn->pstRecvIobuf )
    {    
        /*申请TCP类型的IOBUF内存*/
        pstTlsConn->pstRecvIobuf = COM_Iobuf_Malloc(SWM_MID_SID_TLS);
        if ( NULL == pstTlsConn->pstRecvIobuf )
        {
            VOS_Printf("iobuf create error!");
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
    }
    
    /*这里也可能是上一次只是收了一个字节，或者需要继续收的
    接收完成的话，会将这个iobuf推送到下一个节点*/
    lRecvLen = COM_IOBUF_GETSPACE_LEN(pstTlsConn->pstRecvIobuf);
    if ( 0 >= lRecvLen )
    {
        VOS_Printf("iobuf get free len error,  lRecvLen=%d!", lRecvLen);
        SWM_TLS_ConnDelNotify(pstTlsConn);
        return;
    }
    

    VOS_Printf("swm tls conn recv cb entry, connfd=%d!", pstTlsConn->lConnfd);
    
    /*读取数据*/
    //lError = UTL_SSL_Read(pstTlsConn->pstSsl, pcData, lRecvLen, &lErrorStatus);
    /*新包开始的时候，需要先接收头部信息*/
    if ( pstTlsConn->pstRecvIobuf->ulPreDataLen == COM_IOBUF_LEN )
    {
        lLeftDataLen = SWM_BIZ_HEAD_LEN;
    }
    else
    {
        lLeftDataLen = pstTlsConn->pstRecvIobuf->ulPreDataLen - pstTlsConn->pstRecvIobuf->ulDataLen;
    }
    
    /*获取空闲的数据空间*/
    pcData = COM_IOBUF_GETSAPCE_DATA(pstTlsConn->pstRecvIobuf);
    
    lError = VOS_SOCK_Recv(pstTlsConn->lConnfd, pcData, lLeftDataLen, &lErrorStatus);
    if ( VOS_ERR == lError )
    {
        if ( VOS_SOCK_EWOULDBLOCK == lErrorStatus )
        {
            VOS_Printf("UTL_SSL_Read ewouldblock!");
            return;
        }
        else 
        {
            VOS_Printf("UTL_SSL_Read error!");
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
    }
    else if ( 0 == lError )
    {
        VOS_Printf("UTL_SSL_Read ret = 0!");

        /*可能已经产生了下一个节点，需要通知方式删除*/
        SWM_TLS_ConnDelNotify(pstTlsConn);
        
        return;
    }
    else        
    {
         if ( 1 == lError )
         {
             /* 接收到1个字节,还得继续接收 */
             VOS_Printf("UTL_SSL_Read must be continue!");
             /*设置接收到更新的长度*/
             COM_IOBUF_SETINPUTED_LEN(pstTlsConn->pstRecvIobuf, lError);
             return;
         }
         
         /*继续更新*/
         COM_IOBUF_SETINPUTED_LEN(pstTlsConn->pstRecvIobuf, lError);
    }

    /* 严格根据长度更新来接收，需要获取一个接收的准确的长度信息 */
    if ( COM_IOBUF_LEN == pstTlsConn->pstRecvIobuf->ulPreDataLen )
    {
        pstTlsConn->pstRecvIobuf->ulPreDataLen = SWM_Biz_ChannelPreGetPackLen(pstTlsConn->pstRecvIobuf->pcData);
        if ( pstTlsConn->pstRecvIobuf->ulPreDataLen >= COM_IOBUF_LEN   )
        {
            VOS_Printf("swm pre-check pack length exceed max-length, datalen=%d!",pstTlsConn->pstRecvIobuf->ulPreDataLen );

            SWM_TLS_ConnDelNotify(pstTlsConn);
            
            return;
        }
    }
    
    lDataLen = pstTlsConn->pstRecvIobuf->ulDataLen;
    
    /*严格检查下整包长度，是否需要继续接收*/
    lRet = SWM_Biz_ChannelCheckLen(pstTlsConn->pstRecvIobuf->pcData, lDataLen);
    if ( VOS_OK != lRet )
    {   
        /*表示需要继续接收*/
        if ( VOS_SYS_EWOULDBLOCK == lRet )
        {
            return;
        }
        else
        {
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
    }
    
    /*完整包接收到后，开始进入业务识别处理*/
    if ( EMPTO_BIZTYPEID_UNKNOW == pstBizChannel->ulBizType  )
    {
        if(EMPTO_BIZTYPEID_UNKNOW == SWM_Biz_ChannelMatch(pstBizChannel, pstTlsConn->pstRecvIobuf->pcData, lDataLen) )
        {
            SWM_TLS_ConnDelNotify(pstTlsConn);
            VOS_Printf("swm biz type is unknow!");
            return;
        }
        VOS_Printf("swm biz type is[%d], start match the bizchannel", pstBizChannel->ulBizType);
    }

    /*将接收到的Iobuf 推送给下一个节点 */
    lRet = SWM_TLS_PipeTransBufToNextPipeNode(&pstTlsConn->stTlsPipe, pstTlsConn->pstRecvIobuf);
    switch(lRet)
    {
        case SWM_PIPE_IOBUF_OK:
            /*这种都表示推送成功*/
            /*TODO: 统计流量*/
            VOS_Printf("Tls connect node pipe to next node ok!!");
            /*本接收指针需要重新赋值为空*/
            pstTlsConn->pstRecvIobuf = NULL;
            break;
        case SWM_PIPE_IOBUF_EWOULDBLOCK:
            /*这种要关闭外网的接收*/
            VOS_Printf("Tls connect node pipe to next node ewouldblock!");
            (VOID)RCT_API_NetOpsEventCtrl(&pstTlsConn->stNetEvtOps, VOS_EPOLL_CTRL_INCLOSE);            
            return;
        case SWM_PIPE_IOBUF_AGAIN:
            /*继续接收的话，什么都不做*/
            break;
        case SWM_PIPE_IOBUF_PARAM_ERR:
        case SWM_PIPE_IOBUF_PIPENODE_ERR:
        case SWM_PIPE_IOBUF_UNKNOW_ERR:
            VOS_Printf("Tls connect node pipe to next node error, notify all pipe goto expire!");
            SWM_TLS_ConnDelNotify(pstTlsConn);
            break;
        default:
            break;
    }
    
    return;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_ConnSendCb
 功能描述  : 发送事件处理
 输入参数  : VOID *pvTlsConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID SWM_TLS_ConnSendCb(VOID *pvTlsConn)
{
    LONG lErrorStatus = 0;
    SWM_TLS_CONN_S *pstTlsConn = NULL;
    LONG lRet = 0;
    CHAR *pcData = 0;
    LONG lLeftLen = 0;
    COM_IOBUF_S *pstIobufTmp = NULL;
    ULONG ulIndex = 0;
    ULONG ulNums = 0;
    
    if ( NULL == pvTlsConn )
    {
        VOS_Printf("param error!");
        return;
    }
    
    pstTlsConn = (SWM_TLS_CONN_S *)pvTlsConn;
    
    /*先看看队列是不是空的,如果是空的，则关闭发送*/
    if ( VOS_TRUE == COM_Iobuf_QueIsEmpty(pstTlsConn->pstBizChannel->pstSwmSendQueue))
    {
        /*关闭发送*/
        (VOID)RCT_API_NetOpsEventCtrl(&pstTlsConn->stNetEvtOps, VOS_EPOLL_CTRL_OUTCLOSE);
        return;
    }

    /*上一次还没有发完，需要继续发送*/
    if ( NULL != pstTlsConn->pstSendIobuf )
    {
        VOS_Printf("Tls send iobuf is not empty , must be send again!");
        goto SendAgain;
    }

    /*发送队列的数目，不为空肯定是>0*/
    ulNums = COM_Iobuf_QueGetNums(pstTlsConn->pstBizChannel->pstSwmSendQueue);
            
    for(ulIndex = 0; ulIndex < ulNums; ulIndex++)
    {
        /*先入先出，队列尾插入，队列头取出*/
        pstIobufTmp = COM_Iobuf_QuePop(pstTlsConn->pstBizChannel->pstSwmSendQueue);
        if ( NULL == pstIobufTmp )
        {
            /*不应该发生,系统错误*/
            VOS_Printf("tls pop the iobuf nums error!");
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
        
        /*进入到发送流程*/
        pstTlsConn->pstSendIobuf = pstIobufTmp;
        
SendAgain:
        pcData   = COM_IOBUF_GETSAVED_DATA(pstTlsConn->pstSendIobuf);
        lLeftLen = COM_IOBUF_GETSAVED_LEN(pstTlsConn->pstSendIobuf);
        //lRet = UTL_SSL_Write(pstTlsConn->pstSsl, pcData, lLeftLen, &lErrorStatus);
        lRet = VOS_SOCK_Send(pstTlsConn->lConnfd, pcData, lLeftLen, &lErrorStatus);
        if ( 0 < lRet )
        {
            COM_IOBUF_SETOUTPUTED_LEN(pstTlsConn->pstSendIobuf, lRet);
            lLeftLen = COM_IOBUF_GETSAVED_LEN(pstTlsConn->pstSendIobuf);

            /*表明所有数据已经发送完成,继续链表中的buf进行发送*/
            if ( lLeftLen == 0 )
            {
                COM_Iobuf_Free(pstTlsConn->pstSendIobuf);
                pstTlsConn->pstSendIobuf = NULL;
                continue;
            }
            else if ( 0 < lLeftLen )
            {
                /*还有继续剩余*/
                goto SendAgain;
            }
        }
        else 
        {
            /*TODO:出现这个，表示阻塞了，可能需要起个一次性的定时器来不停的尝试*/
            if ( VOS_SOCK_SSL_EWOULDBLOCK == lErrorStatus )
            {
                VOS_Printf("UTL_SSL_Write EWOULDBLOCK!");
                return;
            }
            else
            {
                VOS_Printf("UTL_SSL_Write error!");
                SWM_TLS_ConnDelNotify(pstTlsConn);
                return;
            }
        }
    }
    
    #if 0
    
    /*SSL 握手相关, 还没有握手完成状态*/
    if (  (SWM_TLS_SSL_STATUS_INIT == pstTlsConn->lHandShakeStatus ) 
        || (SWM_TLS_SSL_STATUS_ACCEPTING == pstTlsConn->lHandShakeStatus ) )
    {
        if(VOS_OK == UTL_SSL_Accept(pstTlsConn->pstSsl, &lErrorStatus) ) 
        {
            if ( VOS_SOCK_SSL_ACCEPTING == lErrorStatus )
            {
                pstTlsConn->lHandShakeStatus = SWM_TLS_SSL_STATUS_ACCEPTING;
            }
            else if ( VOS_SOCK_SSL_ACCEPTED == lErrorStatus )
            {
                VOS_Printf("send acceptted!");
                pstTlsConn->lHandShakeStatus = SWM_TLS_SSL_STATUS_ACCEPTED;
            }
            else
            {
                VOS_Printf("send accept error!");
                SWM_TLS_ConnDelNotify(pstTlsConn);
                return;
            }
        }
        else
        {
            VOS_Printf("SSN_SSL_Accept error!");
            /*SSL协商失败，没有产生下一个节点，直接释放*/
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
    }
    #endif
    return;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_ConnExpireCb
 功能描述  : 老化处理
 输入参数  : VOID *pvTlsConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID SWM_TLS_ConnExpireCb(VOID *pvTlsConn)
{
    SWM_TLS_CONN_S *pstTlsConn = NULL;
    
    if ( NULL == pvTlsConn )
    {
        VOS_Printf("param error!");
        return;
    }

    pstTlsConn = (SWM_TLS_CONN_S *)pvTlsConn;
    
    VOS_Printf("SWM_TLS_ConnExpireCb fd[%d]:confirm=%d!", 
                pstTlsConn->lConnfd,pstTlsConn->stExpireOps.ulExpireConfirm);
        
    /*老化节点的规则:
       1. 下面的ulExpireConfirm只能在DelNotify中设置
       2. Notify需要通知下一个节点进行老化
       3. Notify自己不负责具体的老化，只是设置标记位
           所有的业务在老化检查中，检查BizChannel-->ulExitConfirm标记，然后进行老化
       4. 所有节点的具体老化工作在ExpireCB回调中进行回收*/
    if( VOS_TRUE != pstTlsConn->stExpireOps.ulExpireConfirm )
    {
        VOS_Printf("system error");
        return;
    }
    
    if(VOS_ERR == SWM_TLS_ConnRelease(pstTlsConn))
    {
        VOS_Printf("SWM_TLS_ConnExpireCb release error!", pstTlsConn->lConnfd);
        return;
    }

    return;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_ConnCreate
 功能描述  : 创建接收到的TLS节点
 输入参数  : SWM_MSG_ADDCONN_S *pstMsgInfo  
 输出参数  : 无
 返 回 值  : SWM_TLS_CONN_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月13日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_ConnCreate(SWM_MSG_ADDCONN_S *pstMsgInfo)
{
    SWM_TLS_CONN_S *pstTlsConn      = NULL;
    

    if ( NULL == pstMsgInfo )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    
    /*申请TLS节点内存*/
    pstTlsConn = (SWM_TLS_CONN_S *)VOS_Malloc(SWM_MID_SID_TLS, sizeof(SWM_TLS_CONN_S));
    if ( NULL == pstTlsConn )
    {
        VOS_Printf("VOS_Malloc error!");
        VOS_SOCK_Close(pstMsgInfo->lConnfd);
        return VOS_ERR;
    }
    
    VOS_Mem_Zero((CHAR *)pstTlsConn,  sizeof(SWM_TLS_CONN_S));
    VOS_DLIST_INIT(&pstTlsConn->stNode);
    
    pstTlsConn->pstRecvIobuf  = NULL;
    pstTlsConn->lConnfd         = pstMsgInfo->lConnfd;
    pstTlsConn->lHandShakeStatus = SWM_TLS_SSL_STATUS_INIT;
    

    if(VOS_ERR == VOS_SOCK_SetOption(pstMsgInfo->lConnfd))
    {
        VOS_Printf("VOS_SOCK_SetOption() error!");
        VOS_Free((CHAR *)pstTlsConn);
        VOS_SOCK_Close(pstMsgInfo->lConnfd);
        return VOS_ERR;
    }
    
    /*将该socket注册网络事件*/
    if ( VOS_ERR == RCT_API_NetOpsEventRegister(&pstTlsConn->stNetEvtOps, 
                                                pstMsgInfo->lConnfd,
                                                VOS_EPOLL_MASK_POLLIN | VOS_EPOLL_MASK_POLLOUT,
                                                SWM_TLS_ConnRecvCb,
                                                SWM_TLS_ConnSendCb,
                                                pstTlsConn))
    {
        VOS_Printf("swm tls network register error!!");
        VOS_SOCK_Close(pstMsgInfo->lConnfd);
        VOS_Free((CHAR *)pstTlsConn);
        return VOS_ERR;
    }

    /*老化注册*/
    if ( VOS_ERR == RCT_API_ExpireOpsEventRegister(&pstTlsConn->stExpireOps, 
                                                    SWM_TLS_ConnExpireCb, 
                                                    pstTlsConn))
    {
        VOS_Printf("swm tls expire register error!");
        RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps);
        VOS_SOCK_Close(pstMsgInfo->lConnfd);
        VOS_Free((CHAR *)pstTlsConn);
        return VOS_ERR;
    } 
    
    pstTlsConn->pstBizChannel = SWM_Biz_ChannelCreate(pstMsgInfo->lConnfd);
    if ( NULL == pstTlsConn->pstBizChannel )
    {
        VOS_Printf("swm biz channel create error!");
        RCT_API_ExpireOpsEventUnRegister(&pstTlsConn->stExpireOps);
        RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps);
        VOS_SOCK_Close(pstMsgInfo->lConnfd);
        VOS_Free((CHAR *)pstTlsConn);
        return VOS_ERR;
    }
    
    if ( VOS_ERR == SWM_TLS_PipeConnCreate(pstTlsConn) )
    {
        VOS_Printf("swm pipe conn create error!");
        RCT_API_ExpireOpsEventUnRegister(&pstTlsConn->stExpireOps);
        RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps);
        SWM_Biz_ChannelRelease(pstTlsConn->pstBizChannel);
        VOS_SOCK_Close(pstMsgInfo->lConnfd);
        VOS_Free((CHAR *)pstTlsConn);
        return VOS_ERR;
    }

    VOS_Printf("New tls connect node create OK!sockfd=%d, pstTlsConn=%p", pstTlsConn->lConnfd, pstTlsConn);
    
    return VOS_OK;
}



/*****************************************************************************
 函 数 名  : SWM_TLS_ConnRelease
 功能描述  : 释放TLS节点
 输入参数  : SWM_TLS_CONN_S *pstTlsConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月13日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_ConnRelease(SWM_TLS_CONN_S *pstTlsConn)
{
    if ( NULL == pstTlsConn )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    VOS_Printf("SWM_TLS_ConnRelease start!fd=[%d]", pstTlsConn->lConnfd);
    
    /* 将网络去注册放在DelNotify中,快速关闭，否则会产生大量的0错误事件*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps) )
    {
        VOS_Printf("RCT_Reactor_NetEvtOptsUnRegister error!");
    }
    
    VOS_SOCK_Shutdown(pstTlsConn->lConnfd);
    
    /*注意,一定要先注销老化,否则会再次进入,出现问题*/
    if(VOS_ERR == RCT_API_ExpireOpsEventUnRegister(&pstTlsConn->stExpireOps) )
    {
        VOS_Printf("RCT_Reactor_ExpireOptsUnRegister error!");
    }

    if ( NULL != pstTlsConn->pstRecvIobuf )
    {
        COM_Iobuf_Free(pstTlsConn->pstRecvIobuf);
        pstTlsConn->pstRecvIobuf = NULL;
    }

    if ( NULL != pstTlsConn->pstSendIobuf )
    {
        COM_Iobuf_Free(pstTlsConn->pstSendIobuf);
        pstTlsConn->pstSendIobuf = NULL;
    }
    
    SWM_TLS_PipeConnRelease(pstTlsConn);
    
    SWM_Biz_ChannelRelease(pstTlsConn->pstBizChannel);

    VOS_SOCK_Close(pstTlsConn->lConnfd);
    
    VOS_Free((CHAR *)pstTlsConn);
    
    return VOS_OK;
}



