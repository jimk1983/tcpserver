/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_pipe.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : ҵ��ܵ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"




/*****************************************************************************
 �� �� ��  : SWM_TLS_PipeConnDataUpperProc
 ��������  : ���ڵ�����еĲ�������, SWMԴ�ڵ��ǰ���Ѿ�û�нڵ���
                          Ŀǰ�ú�������û��ʹ��
 �������  :   VOID *pvhandler              
                            COM_IOBUF_S *pstIobuf  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
 �� �� ��  : SWM_TLS_PipeConnDataDownProc
 ��������  : ���ڵ���������ݲ�������
                         (��һ�����ã� ����: NEM��������������������)
 �������  :  VOID *pvhandler                   ---��ǰTLS�ڵ�
                            COM_IOBUF_S *pstIobuf        ---Iobuf����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
        /*�����ȷ��ظ�����ֵ, �Ȳ�������*/
        VOS_Printf("iobuf send exceed the high threshold!");
        
        /*NEM�ڵ㽫���ݷ��͹����󣬽��뷢�͵Ķ���*/
        (VOID)COM_Iobuf_QuePush(pstTlsConn->pstBizChannel->pstSwmSendQueue, pstIobuf);
        return SWM_PIPE_IOBUF_EWOULDBLOCK;
    }
    
    /*NEM�ڵ㽫���ݷ��͹����󣬽��뷢�͵Ķ���*/
    (VOID)COM_Iobuf_QuePush(pstTlsConn->pstBizChannel->pstSwmSendQueue, pstIobuf);
    
    /*�򿪶��з���*/
    RCT_API_NetOpsEventCtrl(&pstTlsConn->stNetEvtOps,  VOS_EPOLL_CTRL_OUTOPEN);
    
   return VOS_OK;                         
}


/*****************************************************************************
 �� �� ��  : SWM_TLS_PipeConnCtrlProc
 ��������  : ���ڵ�Ŀ��Ʋ�������
 �������  :   ULONG ulCtrlCmd      --��������
                             VOID *pvhandler  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
 �� �� ��  : SWM_TLS_PipeConnDelNotifyProc
 ��������  : ���ڵ��ɾ��֪ͨ��������
 �������  : VOID *pvhandler  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

    /*���ñ��ڵ�����ϻ�*/
    SWM_TLS_ConnExpireConfirm(pstTlsConn);

    /*����BizChannel��Expire�ϻ���ǣ���֤�������нڵ����֪ͨ��*/
    pstTlsConn->pstBizChannel->ulExitConfirm = VOS_TRUE;

    /*��Ҫ��һʱ����ɾ����ע�������¼�,�ر��ں˲����Ĵ�������������¼�*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps) )
    {
        VOS_Printf("rct network event unregister error!");
    }

    return;                         
}

/*****************************************************************************
 �� �� ��  : SWM_TLS_PipeConnCreate
 ��������  : TLS��Դ�ܵ��ڵ㴴��
 �������  : SWM_TLS_CONN_S *pstTlsConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

    /*TLS��Դ�ڵ㴴��*/
    SWM_PIPE_CONN_INIT(
            &pstTlsConn->stTlsPipe,
            SWM_PIPE_TYPE_SRC_TLS,                      /*SWM���ڹܵ�ΪԴ�ڵ�*/
            SWM_TLS_PipeConnDataUpperProc,
            SWM_TLS_PipeConnDataDownProc,
            SWM_TLS_PipeConnCtrlProc,
            SWM_TLS_PipeConnDelNotifyProc,
            pstTlsConn );
    
    /*��ǰ��SWM�ܵ�ҵ��ڵ��ʼ��*/
    VOS_DLIST_INIT(&pstPipeConn->stNode);

    /*����ǰ�ĵ�һ��ҵ��ڵ���뵽�ܵ�ͷ�ڵ���*/
    VOS_DLIST_ADD_TAIL(&(pstTlsConn->pstBizChannel->stPipeHead), &pstPipeConn->stNode);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : SWM_TLS_PipeConnRelease
 ��������  : ��TLS�ڵ������Ĺܵ��ڵ��ժ��
 �������  : SWM_TLS_CONN_S *pstTlsConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

    /*�ڹܵ��в���TLSҵ��ڵ�*/
    pstPipeNode = SWM_TLS_PipeConnFoundTlsNode(pstTlsConn->pstBizChannel);
    if ( NULL == pstPipeNode  )
    {
        VOS_Printf("Get src tls PipeNode error!");
        return;
    }

    /*��ҵ��ܵ���ժ�����ڵ�*/
    VOS_DLIST_DEL(&pstPipeNode->stNode);

    return;
}


/*****************************************************************************
 �� �� ��  : SWM_TLS_PipeConnFoundTlsNode
 ��������  : �ڸ�BizChannel�Ĺܵ��в���TLS��Դ�ڵ�
 �������  : SWM_BIZ_CHANNEL_S   *pstBizChannel  
 �������  : ��
 �� �� ֵ  : SWM_PIPE_CONN_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

    /*����ͨ����TLS�ܵ�ҵ��ڵ�*/
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
 �� �� ��  : SWM_TLS_PipeTransBufToNextPipeNode
 ��������  : ���������͸���һ��
 �������  : SWM_PIPE_CONN_S *pstPipeCurNode  
             COM_IOBUF_S *pstIobuf            
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

    /*��ȡ���͵���һ���ڵ㣬�ýڵ�������һ��*/
    pstPipeNextNode  = VOS_DLIST_ENTRY(pstPipeCurNode->stNode.next, SWM_PIPE_CONN_S, stNode);
    if ( NULL == pstPipeNextNode )
    {
        VOS_Printf("get next pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    pstNode = &pstPipeNextNode->stNode;
   
    /*��һ���ڵ㣬�����Ǳ�ͷ������ʵ�ʴ��ڵ�ҵ��ܵ��ڵ�*/
    if ( pstNode->next == pstNode->prev )
    {
        VOS_Printf("pipe node next is empty!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    /*�ҵ���һ����Upper���нڵ��ִ�к���*/
    if ( NULL == pstPipeNextNode->stPipeDataUpperProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the upper function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    VOS_Printf("pipe node next  found the upper function!(%p)", pstPipeNextNode->stPipeDataUpperProc.pvcbFunc);

    /*��ǰһ���ڵ���������͸����е�ִ��*/
    lRet =  ((swm_pipe_dataupperproc_cb)pstPipeNextNode->stPipeDataUpperProc.pvcbFunc)(
                    pstPipeNextNode->stPipeDataUpperProc.pvUserData, pstIobuf);
    
    return lRet;
}

/*****************************************************************************
 �� �� ��  : SWM_TLS_PipeTransBufToPrePipeNode
 ��������  : ��ǰһ��Pipe���õĹܵ����ͽӿ�
 �������  : SWM_PIPE_CONN_S *pstPipeCurNode  
             COM_IOBUF_S *pstIobuf            
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    /*��ȡ���͵�ǰһ���ڵ㣬�ýڵ�������*/
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
    
    /*ȡ��ǰһ���ڵ��DOWN�ӿ�*/
    if ( NULL == pstPipePrevNode->stPipeDataDownProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the down function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    /*ִ��ǰһ���ڵ��DOWN�ӿ�*/
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
 �� �� ��: SWM_TLS_PipeTransCtrlToNextPipeNode
 ��������  : ֪ͨ��һ���ڵ�Ŀ�������
 �������  :   SWM_PIPE_CONN_S *pstPipeCurNode  ---��ǰ�ڵ�
           ULONG ulCmdCode                  ---��������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

    /*��ȡ���͵���һ���ڵ㣬�ýڵ�������һ��*/
    pstPipeNextNode  = VOS_DLIST_ENTRY(pstPipeCurNode->stNode.next, SWM_PIPE_CONN_S, stNode);
    if ( NULL == pstPipeNextNode )
    {
        VOS_Printf("get next pipe node error!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    pstNode = &pstPipeNextNode->stNode;
   
    /*��һ���ڵ㣬�����Ǳ�ͷ������ʵ�ʴ��ڵ�ҵ��ܵ��ڵ�*/
    if ( pstNode->next == pstNode->prev )
    {
        VOS_Printf("pipe node next is empty!");
        return SWM_PIPE_IOBUF_PIPENODE_ERR;
    }

    /*�ҵ���һ����Upper���нڵ��ִ�к���*/
    if ( NULL == pstPipeNextNode->stPipeCtrlProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the upper function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    /*��ǰһ���ڵ���������͸����е�ִ��*/
    lRet =  ((swm_pipe_ctrlproc_cb)pstPipeNextNode->stPipeCtrlProc.pvcbFunc)(
                    pstPipeNextNode->stPipeCtrlProc.pvUserData, ulCmdCode);
    
    return lRet;
}

/*****************************************************************************
 �� �� ��  : SWM_TLS_PipeTransCtrlToPrePipeNode
 ��������  : ��ǰһ��Pipe���õĹܵ����ͽӿ�
 �������  : SWM_PIPE_CONN_S *pstPipeCurNode  
             COM_IOBUF_S *pstIobuf            
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��9��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    /*��ȡ���͵�ǰһ���ڵ㣬�ýڵ�������*/
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
    
    /*ȡ��ǰһ���ڵ��Ctrl�ӿ�*/
    if ( NULL == pstPipePrevNode->stPipeCtrlProc.pvcbFunc )
    {
        VOS_Printf("pipe node next not found the down function!");
        return SWM_PIPE_IOBUF_PIPEFUNC_ERR;
    }
    
    /*ִ��ǰһ���ڵ��DOWN�ӿ�*/
    lRet =  ((swm_pipe_ctrlproc_cb)pstPipePrevNode->stPipeCtrlProc.pvcbFunc)(
                    pstPipePrevNode->stPipeCtrlProc.pvUserData, ulCmdCode);
    if ( VOS_OK != lRet )
    {
        VOS_Printf("next pipe deal the iobuf error!");
        return lRet;
    }
    
    return lRet;
}


