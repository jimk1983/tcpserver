/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_pipe.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月4日
  最近修改   :
  功能描述   : 业务管道
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月4日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"




/*****************************************************************************
 函 数 名  : SWM_TLS_PipeConnDataUpperProc
 功能描述  : 本节点的上行的操作处理, SWM源节点的前面已经没有节点了
                          目前该函数基本没有使用
 输入参数  :   VOID *pvhandler              
                            COM_IOBUF_S *pstIobuf  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_PipeConnDataUpperProc(VOID *pvhandler, COM_IOBUF_S *pstIobuf)
{
    SWM_TLS_CONN_S *pstTlsConn = NULL;
    
    if ( NULL == pvhandler 
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    pstTlsConn = (SWM_TLS_CONN_S *)pvhandler;

    VOS_Printf("swm tls pipe conn=%p", pstTlsConn);

    return VOS_OK;                         
}


/*****************************************************************************
 函 数 名  : SWM_TLS_PipeConnDataDownProc
 功能描述  : 本节点的下行数据操作处理
                         (后一级调用， 例如: NEM会调用这个进行下行推送)
 输入参数  :  VOID *pvhandler                   ---当前TLS节点
                            COM_IOBUF_S *pstIobuf        ---Iobuf数据
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_PipeConnDataDownProc(VOID *pvhandler, COM_IOBUF_S *pstIobuf)
{
    SWM_TLS_CONN_S *pstTlsConn = NULL;
    
     if ( NULL == pvhandler
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    VOS_Printf("swm pipe down entry ,send out!");

    pstTlsConn = (SWM_TLS_CONN_S *)pvhandler;

    if ( SWM_PIPE_IOBUF_THLD_HIGH <= COM_Iobuf_QueGetNums(pstTlsConn->pstBizChannel->pstSwmSendQueue) )
    {
        /*这里先返回该阻塞值, 先不做处理*/
        VOS_Printf("iobuf send exceed the high threshold!");
        
        /*NEM节点将数据发送过来后，进入发送的队列*/
        (VOID)COM_Iobuf_QuePush(pstTlsConn->pstBizChannel->pstSwmSendQueue, pstIobuf);
        return SWM_PIPE_IOBUF_EWOULDBLOCK;
    }
    
    /*NEM节点将数据发送过来后，进入发送的队列*/
    (VOID)COM_Iobuf_QuePush(pstTlsConn->pstBizChannel->pstSwmSendQueue, pstIobuf);
    
    /*打开队列发送*/
    RCT_API_NetOpsEventCtrl(&pstTlsConn->stNetEvtOps,  VOS_EPOLL_CTRL_OUTOPEN);
    
   return VOS_OK;                         
}


/*****************************************************************************
 函 数 名  : SWM_TLS_PipeConnCtrlProc
 功能描述  : 本节点的控制操作处理
 输入参数  :   ULONG ulCtrlCmd      --控制命令
                             VOID *pvhandler  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_PipeConnCtrlProc(VOID *pvhandler, ULONG ulCtrlCmd)
{
    SWM_TLS_CONN_S *pstTlsConn = NULL;
    
     if ( NULL == pvhandler )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    pstTlsConn = (SWM_TLS_CONN_S *)pvhandler;
    
    VOS_Printf("SWM tls ctrl proc tls conn=%p", pstTlsConn);
    
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_PipeConnDelNotifyProc
 功能描述  : 本节点的删除通知操作处理
 输入参数  : VOID *pvhandler  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID SWM_TLS_PipeConnDelNotifyProc(VOID *pvhandler)
{
    SWM_TLS_CONN_S *pstTlsConn = NULL;

     if ( NULL == pvhandler )
    {
        VOS_Printf("param error!");
        return;
    }

    pstTlsConn = (SWM_TLS_CONN_S *)pvhandler;

    /*设置本节点进入老化*/
    SWM_TLS_ConnExpireConfirm(pstTlsConn);

    /*设置BizChannel的Expire老化标记，保证后续所有节点可以通知到*/
    pstTlsConn->pstBizChannel->ulExitConfirm = VOS_TRUE;

    /*需要第一时间先删除该注册网络事件,关闭内核产生的大量错误的网络事件*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps) )
    {
        VOS_Printf("rct network event unregister error!");
    }

    return;                         
}

/*****************************************************************************
 函 数 名  : SWM_TLS_PipeConnCreate
 功能描述  : TLS的源管道节点创建
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
LONG SWM_TLS_PipeConnCreate(SWM_TLS_CONN_S *pstTlsConn)
{
    SWM_PIPE_CONN_S *pstPipeConn= NULL;

    if ( NULL == pstTlsConn)
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    pstPipeConn = &pstTlsConn->stTlsPipe;

    VOS_Mem_Zero((CHAR *)pstPipeConn,sizeof(SWM_PIPE_CONN_S));

    /*TLS的源节点创建*/
    SWM_PIPE_CONN_INIT(
            &pstTlsConn->stTlsPipe,
            SWM_PIPE_TYPE_SRC_TLS,                      /*SWM所在管道为源节点*/
            SWM_TLS_PipeConnDataUpperProc,
            SWM_TLS_PipeConnDataDownProc,
            SWM_TLS_PipeConnCtrlProc,
            SWM_TLS_PipeConnDelNotifyProc,
            pstTlsConn );
    
    /*当前的SWM管道业务节点初始化*/
    VOS_DLIST_INIT(&pstPipeConn->stNode);

    /*将当前的第一个业务节点加入到管道头节点中*/
    VOS_DLIST_ADD_TAIL(&(pstTlsConn->pstBizChannel->stPipeHead), &pstPipeConn->stNode);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_PipeConnRelease
 功能描述  : 该TLS节点所属的管道节点的摘除
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
VOID SWM_TLS_PipeConnRelease(SWM_TLS_CONN_S *pstTlsConn)
{
    SWM_PIPE_CONN_S *pstPipeNode= NULL;

    if ( NULL == pstTlsConn
        || NULL == pstTlsConn->pstBizChannel )
    {
        VOS_Printf("param error!");
        return;
    }

    /*在管道中查找TLS业务节点*/
    pstPipeNode = SWM_TLS_PipeConnFoundTlsNode(pstTlsConn->pstBizChannel);
    if ( NULL == pstPipeNode  )
    {
        VOS_Printf("Get src tls PipeNode error!");
        return;
    }

    /*在业务管道中摘除本节点*/
    VOS_DLIST_DEL(&pstPipeNode->stNode);

    return;
}


/*****************************************************************************
 函 数 名  : SWM_TLS_PipeConnFoundTlsNode
 功能描述  : 在该BizChannel的管道中查找TLS的源节点
 输入参数  : SWM_BIZ_CHANNEL_S   *pstBizChannel  
 输出参数  : 无
 返 回 值  : SWM_PIPE_CONN_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
SWM_PIPE_CONN_S *SWM_TLS_PipeConnFoundTlsNode(SWM_BIZ_CHANNEL_S   *pstBizChannel)
{
    SWM_PIPE_CONN_S *pstPipeNode = NULL;

    if ( NULL == pstBizChannel )
    {
        VOS_Printf("param error!");
        return NULL;
    }
    
    if ( VOS_OK == VOS_Node_IsEmpty(&pstBizChannel->stPipeHead) )
    {
        VOS_Printf("pipe list is empty!");
        return NULL;;
    }  

    /*查找通道的TLS管道业务节点*/
    VOS_DLIST_FOR_EACH_ENTRY(pstPipeNode, &pstBizChannel->stPipeHead, SWM_PIPE_CONN_S, stNode)
    {
        if ( NULL == pstPipeNode )
        {
            VOS_Printf("ASSERT error!");
            continue;
        }

        if ( pstPipeNode->ePipeType == SWM_PIPE_TYPE_SRC_TLS)
        {
            break;
        }
    }

    //VOS_Printf("swm tls pipe found is ok!pipe=%p!", pstPipeNode);
    return pstPipeNode;
}


/*****************************************************************************
 函 数 名  : SWM_TLS_PipeTransBufToNextPipeNode
 功能描述  : 将数据推送给后一层
 输入参数  : SWM_PIPE_CONN_S *pstPipeCurNode  
             COM_IOBUF_S *pstIobuf            
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_PipeTransBufToNextPipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, COM_IOBUF_S *pstIobuf)
{
    SWM_PIPE_CONN_S  *pstPipeNextNode = NULL;
    VOS_DLIST_NODE_S *pstNode = NULL;
    LONG lRet = SWM_PIPE_IOBUF_OK;
        
    if ( NULL == pstPipeCurNode 
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return SWM_PIPE_IOBUF_PARAM_ERR;
    }

    //VOS_Printf("pstPipeCurNode=%p, stnode=%p, next=%p, prev=%p, ioBuf=%p!",
    //    pstPipeCurNode, &pstPipeCurNode->stNode, pstPipeCurNode->stNode.next, pstPipeCurNode->stNode.prev, pstIobuf);

    /*获取推送的下一个节点，该节点必须存在一种*/
    pstPipeNextNode  = VOS_DLIST_ENTRY(pstPipeCurNode->stNode.next, SWM_PIPE_CONN_S, stNode);
    if ( NULL == pstPipeNextNode )
    {
        VOS_Printf("get next pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    pstNode = &pstPipeNextNode->stNode;
   
    /*下一个节点，不能是表头，而是实际存在的业务管道节点*/
    if ( pstNode->next == pstNode->prev )
    {
        VOS_Printf("pipe node next is empty!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    /*找到下一个的Upper上行节点的执行函数*/
    if ( NULL == pstPipeNextNode->stPipeDataUpperProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the upper function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    VOS_Printf("pipe node next  found the upper function!(%p)", pstPipeNextNode->stPipeDataUpperProc.pvcbFunc);

    /*从前一个节点过来的推送给上行的执行*/
    lRet =  ((swm_pipe_dataupperproc_cb)pstPipeNextNode->stPipeDataUpperProc.pvcbFunc)(
                    pstPipeNextNode->stPipeDataUpperProc.pvUserData, pstIobuf);
    
    return lRet;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_PipeTransBufToPrePipeNode
 功能描述  : 给前一层Pipe调用的管道发送接口
 输入参数  : SWM_PIPE_CONN_S *pstPipeCurNode  
             COM_IOBUF_S *pstIobuf            
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_PipeTransBufToPrePipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, COM_IOBUF_S *pstIobuf)
{
    SWM_PIPE_CONN_S  *pstPipePrevNode = NULL;
    LONG lRet = SWM_PIPE_IOBUF_OK;
    VOS_DLIST_NODE_S *pstNode = NULL;
        
    if ( NULL == pstPipeCurNode 
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return SWM_PIPE_IOBUF_PARAM_ERR;
    }

    VOS_Printf("pstPipeCurNode->stNode=%p, prev=%p, next[head]=%p!", 
            &pstPipeCurNode->stNode, pstPipeCurNode->stNode.prev, pstPipeCurNode->stNode.next);
    
    /*获取推送的前一个节点，该节点必须存在*/
    pstPipePrevNode  = VOS_DLIST_ENTRY(pstPipeCurNode->stNode.prev, SWM_PIPE_CONN_S, stNode);
    if ( NULL == pstPipePrevNode )
    {
        VOS_Printf("get next pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }
    pstNode = &pstPipePrevNode->stNode;
        
    if ( pstNode->next == pstNode->prev )
    {
        VOS_Printf("pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }
    
    /*取出前一个节点的DOWN接口*/
    if ( NULL == pstPipePrevNode->stPipeDataDownProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the down function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    /*执行前一个节点的DOWN接口*/
    lRet =  ((swm_pipe_datadownproc_cb)pstPipePrevNode->stPipeDataDownProc.pvcbFunc)(
                    pstPipePrevNode->stPipeDataDownProc.pvUserData, pstIobuf);
    if ( VOS_OK != lRet )
    {
        VOS_Printf("next pipe deal the iobuf error!");
        return lRet;
    }
    
    return lRet;
}


/*****************************************************************************
 函 数 名: SWM_TLS_PipeTransCtrlToNextPipeNode
 功能描述  : 通知下一个节点的控制命令
 输入参数  :   SWM_PIPE_CONN_S *pstPipeCurNode  ---当前节点
           ULONG ulCmdCode                  ---控制命令
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_PipeTransCtrlToNextPipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, ULONG ulCmdCode)
{
    SWM_PIPE_CONN_S  *pstPipeNextNode = NULL;
    VOS_DLIST_NODE_S *pstNode = NULL;
    LONG lRet = SWM_PIPE_IOBUF_OK;
        
    if ( NULL == pstPipeCurNode )
    {
        VOS_Printf("param error!");
        return SWM_PIPE_IOBUF_PARAM_ERR;
    }

    /*获取推送的下一个节点，该节点必须存在一种*/
    pstPipeNextNode  = VOS_DLIST_ENTRY(pstPipeCurNode->stNode.next, SWM_PIPE_CONN_S, stNode);
    if ( NULL == pstPipeNextNode )
    {
        VOS_Printf("get next pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    pstNode = &pstPipeNextNode->stNode;
   
    /*下一个节点，不能是表头，而是实际存在的业务管道节点*/
    if ( pstNode->next == pstNode->prev )
    {
        VOS_Printf("pipe node next is empty!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    /*找到下一个的Upper上行节点的执行函数*/
    if ( NULL == pstPipeNextNode->stPipeCtrlProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the upper function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    /*从前一个节点过来的推送给上行的执行*/
    lRet =  ((swm_pipe_ctrlproc_cb)pstPipeNextNode->stPipeCtrlProc.pvcbFunc)(
                    pstPipeNextNode->stPipeCtrlProc.pvUserData, ulCmdCode);
    
    return lRet;
}

/*****************************************************************************
 函 数 名  : SWM_TLS_PipeTransCtrlToPrePipeNode
 功能描述  : 给前一层Pipe调用的管道发送接口
 输入参数  : SWM_PIPE_CONN_S *pstPipeCurNode  
             COM_IOBUF_S *pstIobuf            
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月9日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG SWM_TLS_PipeTransCtrlToPrePipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, ULONG ulCmdCode)
{
    SWM_PIPE_CONN_S  *pstPipePrevNode = NULL;
    LONG lRet = SWM_PIPE_IOBUF_OK;
    VOS_DLIST_NODE_S *pstNode = NULL;
        
    if ( NULL == pstPipeCurNode )
    {
        VOS_Printf("param error!");
        return SWM_PIPE_IOBUF_PARAM_ERR;
    }
    
    /*获取推送的前一个节点，该节点必须存在*/
    pstPipePrevNode  = VOS_DLIST_ENTRY(pstPipeCurNode->stNode.prev, SWM_PIPE_CONN_S, stNode);
    if ( NULL == pstPipePrevNode )
    {
        VOS_Printf("get next pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }
    pstNode = &pstPipePrevNode->stNode;
        
    if ( pstNode->next == pstNode->prev )
    {
        VOS_Printf("pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }
    
    /*取出前一个节点的Ctrl接口*/
    if ( NULL == pstPipePrevNode->stPipeCtrlProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the down function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    /*执行前一个节点的DOWN接口*/
    lRet =  ((swm_pipe_ctrlproc_cb)pstPipePrevNode->stPipeCtrlProc.pvcbFunc)(
                    pstPipePrevNode->stPipeCtrlProc.pvUserData, ulCmdCode);
    if ( VOS_OK != lRet )
    {
        VOS_Printf("next pipe deal the iobuf error!");
        return lRet;
    }
    
    return lRet;
}


