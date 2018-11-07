/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : com_iobuf.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年10月28日
  最近修改   :
  功能描述   : 通用的IOBUF
  函数列表   :
  修改历史   :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "libcomm.h"
#include "shareplane/share_comm.h"


/*****************************************************************************
 函 数 名  : COM_Iobuf_Malloc
 功能描述  : IOBUF申请
 输入参数  : ULONG ulMid  
 输出参数  : 无
 返 回 值  : COM_IOBUF_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
COM_IOBUF_S *COM_Iobuf_Malloc(ULONG ulMid)
{
    COM_IOBUF_S *pstIobuf  =    NULL;
    ULONG ulBufSize             =    COM_IOBUF_MALLOC_SIZE;

    pstIobuf = (COM_IOBUF_S *)VOS_Malloc(ulMid, ulBufSize);
    if ( NULL == pstIobuf )
    {
        VOS_Printf("malloc error!");
        return NULL;
    }

    VOS_Mem_Zero((CHAR *)pstIobuf, ulBufSize);

    VOS_Node_Init(&pstIobuf->stNode);

    pstIobuf->ulMagic     = COM_IOBUF_MAGIC;
    pstIobuf->lLeftLen    = COM_IOBUF_LEN;
    pstIobuf->ulPreDataLen  = COM_IOBUF_LEN;
    pstIobuf->ulMaxLen  = COM_IOBUF_LEN;

    return pstIobuf;
}

/*****************************************************************************
 函 数 名  : COM_Iobuf_Free
 功能描述  : IOBUF释放
 输入参数  : COM_IOBUF_S *pstIobuf  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID COM_Iobuf_Free(COM_IOBUF_S *pstIobuf)
{
    if ( NULL == pstIobuf  )
    {
        return;
    }
    
    VOS_Free((CHAR *)pstIobuf);
}

/*****************************************************************************
 函 数 名  : COM_Iobuf_QueCreate
 功能描述  : IOBUF的队列创建, 提高性能不加锁，
                          注意: 需要在同一个线程内进行收发，不能多线程使用
                           
 输入参数  : 无
 输出参数  : 无
 返 回 值  : COM_IOBUF_QUE_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
COM_IOBUF_QUE_S *COM_Iobuf_QueCreate()
{
    COM_IOBUF_QUE_S *pstIobufQue = NULL;

    pstIobufQue = (COM_IOBUF_QUE_S *)VOS_Malloc(COM_QUE_MID, sizeof(COM_IOBUF_QUE_S));
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("malloc error");
        return NULL;
    }

    VOS_Mem_Zero((CHAR *)pstIobufQue, sizeof(COM_IOBUF_QUE_S));

    /*链表初始化,头节点*/
    VOS_DLIST_INIT(&pstIobufQue->stList);

    pstIobufQue->uiNum = 0;
    
    return pstIobufQue;
}


/*****************************************************************************
 函 数 名  : COM_Iobuf_QueFree
 功能描述  : IOBUF的队列释放
 输入参数  : COM_IOBUF_QUE_S *pstIobufQue  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID COM_Iobuf_QueFree(COM_IOBUF_QUE_S *pstIobufQue)
{
    COM_IOBUF_S *pstIobuf         =  NULL;
    COM_IOBUF_S *pstIobufTmp  =  NULL;
    
    if ( NULL == pstIobufQue )
    {
        return;
    }

    /*如果有多余的节点，则遍历删除，避免内存泄漏*/
    if ( 0 < pstIobufQue->uiNum )
    {
        VOS_DLIST_FOR_EACH_ENTRY(pstIobuf, &pstIobufQue->stList, COM_IOBUF_S, stNode)
        {
            /*先将下个节点保留*/
            pstIobufTmp = VOS_DLIST_ENTRY(pstIobuf->stNode.next, COM_IOBUF_S, stNode);
            
            /*上面已经保存了，因此可以直接脱链*/
            VOS_Node_Remove((&pstIobuf->stNode));
            /*消息节点使用VOS_Malloc()申请的，便于内存监听*/
            VOS_Free((CHAR *)pstIobuf);
            /*进入下一个节点的判断*/
            pstIobuf = pstIobufTmp;
        }
    }   
    VOS_Free((CHAR *)pstIobufQue);  

}


/*****************************************************************************
 函 数 名  : COM_Iobuf_QuePush
 功能描述  : 入队列
 输入参数  : COM_IOBUF_QUE_S *pstIobufQue  
             COM_IOBUF_S *pstIobuf         
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
INT32 COM_Iobuf_QuePush(COM_IOBUF_QUE_S *pstIobufQue, COM_IOBUF_S *pstIobuf)
{
    if ( NULL == pstIobufQue
            || NULL == pstIobuf )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    /*加入队列*/
    VOS_DLIST_ADD_TAIL(&pstIobufQue->stList, &pstIobuf->stNode);

    pstIobufQue->uiNum++;
    
    return VOS_OK;
}



/*****************************************************************************
 函 数 名  : COM_Iobuf_QuePop
 功能描述  : 从队列中获取消息节点，先入先出，队列头取出
 输入参数  : COM_IOBUF_QUE_S *pstIobufQue  
 输出参数  : 无
 返 回 值  : COM_IOBUF_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
COM_IOBUF_S * COM_Iobuf_QuePop(COM_IOBUF_QUE_S *pstIobufQue)
{
    VOS_DLIST_NODE_S        *pstNode       = NULL;
    COM_IOBUF_S                 *pstIobuf       = NULL;
    
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("param error");
        return NULL;
    }

    /*空队列*/
    if ( 0 == pstIobufQue->uiNum )
    {
        /*不可能发生的，检查*/
        if(VOS_OK != VOS_Node_IsEmpty(&pstIobufQue->stList))
        {
            VOS_ASSERT(0);
            VOS_Printf("system error: iobuf queue error!");
        }
        return NULL;
    }
    
    /*取出队列头节点*/
    if(VOS_ERR == VOS_Node_HeadGet(&pstIobufQue->stList, &pstNode))
    {
        VOS_Printf("system error: iobuf queue get node error!");
    }
    
    pstIobufQue->uiNum--;

    pstIobuf = (COM_IOBUF_S  *)pstNode;

    return pstIobuf;
}

/*****************************************************************************
 函 数 名  : COM_Iobuf_QueGetNums
 功能描述  : 获取队列中的iobuf数量
 输入参数  : COM_IOBUF_QUE_S *pstIobufQue  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
UINT32 COM_Iobuf_QueGetNums(COM_IOBUF_QUE_S *pstIobufQue)
{
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("param error");
        return 0;
    }

    return pstIobufQue->uiNum;
}

/*****************************************************************************
 函 数 名  : COM_Iobuf_QueIsEmpty
 功能描述  : 判断队列是否为空
 输入参数  : COM_IOBUF_QUE_S *pstIobufQue  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG COM_Iobuf_QueIsEmpty(COM_IOBUF_QUE_S *pstIobufQue)
{
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("param error");
        return VOS_FALSE;
    }
    
    if ( 0 == pstIobufQue->uiNum )
    {
        if(VOS_OK != VOS_Node_IsEmpty(&pstIobufQue->stList))
        {
            VOS_ASSERT(0);
            VOS_Printf("system error: iobuf queue error!");
            return VOS_FALSE;
        }
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


