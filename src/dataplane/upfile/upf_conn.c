/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : pfw_conn.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2017��4��29��
  ����޸�   :
  ��������   : UPF�ڵ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include "common.h"
#include "swm/swm_pub.h"
#include "upfile/upf_pub.h"



/*****************************************************************************
 �� �� ��  : UPF_Conn_DelNotify
 ��������  : ɾ��֪ͨ
 �������  : PFW_CONN_S *pstPfwConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG UPF_Conn_DelNotify(UPF_CONN_S *pstPfwConn)
{
    SWM_PIPE_CONN_S *pstPipeNext = NULL;
    VOS_DLIST_NODE_S *pstListHead = NULL;
    
    if ( NULL == pstPfwConn )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }
    
    /*��Ҫ��һʱ����ɾ��ע��������¼�,�ر��ں˲����Ĵ�������������¼�*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstPfwConn->stNetEvtOps) )
    {
        VOS_Printf("rct net unregister error!");
    }
    
    /*���ñ��ڵ�����ϻ�*/
    pstPfwConn->stExpireOps.ulExpireConfirm = VOS_TRUE;
    
    /*����Ѿ��������ã��Ͳ��ü���֪ͨ�ˣ�
        ��ʾ�Ѿ������ڱ���֪ͨ�ϻ���*/
    if ( pstPfwConn->pstBizChannel->ulExitConfirm == VOS_TRUE )
    {
        return VOS_ERR;
    }
    
    /*��������һ�£���ֹ����ڵ��ظ�֪ͨ,���濪ʼ֪ͨ����ҵ��ܵ��ڵ�*/
    pstPfwConn->pstBizChannel->ulExitConfirm = VOS_TRUE;
    
    pstListHead = &pstPfwConn->pstBizChannel->stPipeHead;
    
    /*֪ͨ���иùܵ��ڵ�����˳�*/    
    VOS_DLIST_FOR_EACH_ENTRY(pstPipeNext, pstListHead, SWM_PIPE_CONN_S, stNode)
    {
        if ( NULL == pstPipeNext )
        {
            VOS_Printf("Not found the next pipe node!");
            break;
        }

        /*���ڵ����PFW�ڵ�, Ҫ�ų����ڵ㣬��ֹ�ݹ�*/
        if ( pstPipeNext->ePipeType == SWM_PIPE_TYPE_BIZ_PFW )
        {
            /*�����Լ����ٽ���ú����������ݹ�ѭ��,����*/
            continue;
        }

        /*������һ���ڵ�, ֪ͨ����ɾ��*/
        if ( NULL != pstPipeNext->stPipeDelNotifyProc.pvcbFunc ) 
        {   
            VOS_Printf("found the next pipe node, goto delnotify!pstPipeNode=%p,data=%p", 
                pstPipeNext, pstPipeNext->stPipeDelNotifyProc.pvUserData);
            ((swm_pipe_delnotify_cb)pstPipeNext->stPipeDelNotifyProc.pvcbFunc)(pstPipeNext->stPipeDelNotifyProc.pvUserData);
        }
    }
    return VOS_OK;
}



/*****************************************************************************
 �� �� ��  : UPF_Conn_TransBufToDownPipeNode
 ��������  : �������͸���������
 �������  :   PFW_CONN_S *pstNemConn  ---��ǰ�ڵ�
           COM_IOBUF_S *pstIobuf   ---Ҫ���͵�iobuf
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��14��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG UPF_Conn_TransBufToDownPipeNode(UPF_CONN_S *pstPfwConn, COM_IOBUF_S *pstIobuf)
{
    LONG lRet = 0;

    if ( NULL == pstPfwConn
        || NULL == pstIobuf
        || NULL == pstPfwConn->pstBizChannel )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    /*���ҵ�ǰ�ڵ��ǰһ��ڵ㣬��������*/
    lRet = SWM_TLS_PipeTransBufToPrePipeNode(&pstPfwConn->stPipe, pstIobuf);
    if (SWM_PIPE_IOBUF_EWOULDBLOCK == lRet )
    {
        /*������չ����ֱ�Ӷ�������������Ϊ���泬����ֵ������������*/    
        COM_Iobuf_Free(pstIobuf);
        VOS_Printf("ewould block error!");
        return VOS_OK;
    }
    else if (SWM_PIPE_IOBUF_OK == lRet )
    {
        /*TODO: ͳ����������*/
        VOS_Printf("pfw stastic flows!");
        return VOS_OK;
    }
    else
    {
        VOS_Printf("Tls connect node pipe to next node error, notify all pipe goto expire!");
        UPF_Conn_DelNotify(pstPfwConn);
    }
    
    return VOS_ERR;
}


/*****************************************************************************
 �� �� ��  : UPF_Conn_PipeConnDataUpperProc
 ��������  : ��������SWM�����е�����
 �������  :   VOID *pvhandler              ---���ڵ�
           COM_IOBUF_S *pstIobuf    ---���Կͻ��˵����е�iobuf
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG UPF_Conn_PipeConnDataUpperProc(VOID *pvhandler, COM_IOBUF_S *pstIobuf)
{
    UPF_CONN_S *pstPFWConn   = NULL;
    SWM_BIZ_HEAD_S  *pstBizHead = NULL;
    CHAR            *pcPacket        = NULL;
    UINT32          uiPackLen        =0 ;
    
    if ( NULL == pvhandler 
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return SWM_PIPE_IOBUF_PARAM_ERR;
    }
    
    pstPFWConn = (UPF_CONN_S *)pvhandler;

    /*�����ý��յ������ݣ� ���ܻ�Ҫ��һ������*/
    pstPFWConn->pstRcvIobuf = pstIobuf;

    pcPacket   = COM_IOBUF_GETSAVED_DATA(pstIobuf);
    uiPackLen  = COM_IOBUF_GETSAVED_LEN(pstIobuf);
    
    pstBizHead = (SWM_BIZ_HEAD_S  *)pcPacket;
    
    VOS_Printf("UPF conn upper :recv the iobuf=%p, upf conn=%p, uiPackLen=%d, pstBizHead=%p!",  
        pstIobuf, pstPFWConn, uiPackLen, pstBizHead);
    
    
   
    COM_Iobuf_Free(pstIobuf);
    return SWM_PIPE_IOBUF_OK;                         
}


/*****************************************************************************
 �� �� ��  : UPF_Conn_PipeConnDataDownProc
 ��������  : ���պ�һ������У�ĿǰPFWû�к�һ�㣬
           �ṩ��NEM�ĺ�һ��ܵ��������е��ã������ʱû�и����д���
 �������  : VOID *pvhandler        
           COM_IOBUF_S *pstIobuf  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG UPF_Conn_PipeConnDataDownProc(VOID *pvhandler, COM_IOBUF_S *pstIobuf)
{
    UPF_CONN_S *pstConn = NULL;

    if ( NULL == pvhandler 
        || NULL == pstIobuf )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    pstConn = (UPF_CONN_S *)pvhandler;

    VOS_Printf("PFM_Conn_PipeConnDataDownProc Entry!pstConn=%p", pstConn);
    
   return VOS_OK;                         
}

/*****************************************************************************
 �� �� ��  : UPF_Conn_PipeConnCtrlProc
 ��������  : ���ڵ�Ŀ��ƴ���
 �������  : ULONG ulCtrlCmd  
                          VOID *pvhandler  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    VOS_Printf("PFW_Conn_PipeConnCtrlProc Entry! pstConn=%p", pstConn);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : UPF_Conn_PipeConnDelNotifyProc
 ��������  : ɾ��֪ͨ
 �������  : VOID *pvhandler  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    VOS_Printf("PFW_Conn_PipeConnDelNotifyProc Entry!");
    
    UPF_Conn_DelNotify(pstConn);

    return;                         
}

/*****************************************************************************
 �� �� ��  : UPF_Conn_ExpireCb
 ��������  : �ϻ����崦��
 �������  : VOID *pvPfwConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��14��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID UPF_Conn_ExpireCb(VOID *pvPfwConn)
{
    UPF_CONN_S *pstPFWConn = NULL;
    
    if ( NULL == pvPfwConn )
    {
        VOS_Printf("param error!");
        return;
    }

    pstPFWConn =(UPF_CONN_S *)pvPfwConn;
    
    VOS_Printf("pfw connect node goto expiring...!");
    
    if( VOS_TRUE != pstPFWConn->stExpireOps.ulExpireConfirm )
    {
        /*���ﲻӦ�÷���*/
        VOS_Printf("system error!");
        return;
    }
    
    (VOID)UPF_Conn_Release(pstPFWConn);

    return;
}


/*****************************************************************************
 �� �� ��  : UPF_Conn_Create
 ��������  : ������չ�ڵ�Ĵ���
 �������  : SWM_BIZ_CHANNEL_S *pstBizChannel  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
        
    /*��Pipe���뵽BizChannel��*/
    VOS_Mem_Zero((CHAR *)&pstConn->stPipe,sizeof(SWM_PIPE_CONN_S));

    VOS_DLIST_INIT(&pstConn->stPipe.stNode);
    
    /*��ʼ��������չ��Pipe*/
    SWM_PIPE_CONN_INIT(
            &pstConn->stPipe,
            SWM_PIPE_TYPE_BIZ_PFW,
            UPF_Conn_PipeConnDataUpperProc,             /*��������*/
            UPF_Conn_PipeConnDataDownProc,              /*��������*/
            UPF_Conn_PipeConnCtrlProc,                        /*���ܵ��Ŀ���*/
            UPF_Conn_PipeConnDelNotifyProc,                /*ɾ������*/
            pstConn);
    
    /*�������BizChannelͨ���Ĺܵ�������*/
    VOS_DLIST_ADD_TAIL(&(pstBizChannel->stPipeHead), &pstConn->stPipe.stNode);

    if ( VOS_ERR == RCT_API_ExpireOpsEventRegister(&pstConn->stExpireOps, UPF_Conn_ExpireCb, pstConn) )
    {
        VOS_Printf("expire register error!");
        VOS_Free((CHAR *)pstConn);
        return VOS_ERR;
    } 

                
    VOS_Printf("PFW Conn Create OK");

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : UPF_Conn_Release
 ��������  : �ͷ�
 �������  : NEM_CONN_S *pstNemConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��14��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG UPF_Conn_Release(UPF_CONN_S *pstConn)
{
    if ( NULL == pstConn )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }
    
    VOS_Printf("PFWConnect Release Entry!");
    
    pstConn->pstBizChannel->ulExitConfirm  = VOS_TRUE;

    /*BizChannel �����뱾�ڵ�*/
    VOS_DLIST_DEL(&pstConn->stPipe.stNode);

    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstConn->stNetEvtOps) )
    {
        VOS_Printf("rct net unregister error!");
    }

    if(VOS_ERR == RCT_API_ExpireOpsEventUnRegister(&pstConn->stExpireOps))
    {
        VOS_Printf("RCT_Reactor_ExpireOptsUnRegister error!");
    }
    
    VOS_Free((CHAR *)pstConn);

    pstConn = NULL;

    return VOS_OK;
}

