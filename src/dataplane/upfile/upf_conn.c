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
#include "vgm/vgm_pub.h"
#include "swm/swm_pub.h"
#include "fsm/fsm_pub.h"
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
LONG UPF_Conn_DelNotify(UPF_CONN_S *pstUpfConn)
{
    SWM_PIPE_CONN_S *pstPipeNext = NULL;
    VOS_DLIST_NODE_S *pstListHead = NULL;
    
    if ( NULL == pstUpfConn )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }
    
    /*��Ҫ��һʱ����ɾ��ע��������¼�,�ر��ں˲����Ĵ�������������¼�*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstUpfConn->stNetEvtOps) )
    {
        VOS_Printf("rct net unregister error!");
    }
    
    /*���ñ��ڵ�����ϻ�*/
    pstUpfConn->stExpireOps.ulExpireConfirm = VOS_TRUE;
    
    /*����Ѿ��������ã��Ͳ��ü���֪ͨ�ˣ�
        ��ʾ�Ѿ������ڱ���֪ͨ�ϻ���*/
    if ( pstUpfConn->pstBizChannel->ulExitConfirm == VOS_TRUE )
    {
        return VOS_ERR;
    }
    
    /*��������һ�£���ֹ����ڵ��ظ�֪ͨ,���濪ʼ֪ͨ����ҵ��ܵ��ڵ�*/
    pstUpfConn->pstBizChannel->ulExitConfirm = VOS_TRUE;
    
    pstListHead = &pstUpfConn->pstBizChannel->stPipeHead;
    
    /*֪ͨ���иùܵ��ڵ�����˳�*/    
    VOS_DLIST_FOR_EACH_ENTRY(pstPipeNext, pstListHead, SWM_PIPE_CONN_S, stNode)
    {
        if ( NULL == pstPipeNext )
        {
            VOS_Printf("Not found the next pipe node!");
            break;
        }

        /*���ڵ����PFW�ڵ�, Ҫ�ų����ڵ㣬��ֹ�ݹ�*/
        if ( pstPipeNext->ePipeType == SWM_PIPE_TYPE_BIZ_UPF )
        {
            /*�����Լ����ٽ���ú����������ݹ�ѭ��,����*/
            continue;
        }

        /*������һ���ڵ�, ֪ͨ����ɾ��*/
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

    /*���ҵ�ǰ�ڵ��ǰһ��ڵ㣬��������*/
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
 �� �� ��: UPF_Conn_PipeConnDataDownProc
 ��������  : ���պ�һ������У�ĿǰPFWû�к�һ�㣬
           �ṩ��NEM�ĺ�һ��ܵ��������е��ã������ʱû�и����д���
 �������  : VOID *pvhandler        
           COM_IOBUF_S *pstIobuf  
 �������  : ��
 �� �� ֵ: 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    //VOS_Printf("UPF_Conn_PipeConnCtrlProc: TerminalID=[%s], continue out!", pstConn->acTerminalID);

    /*����Ѿ������ϻ����Ͳ�Ҫ�ټ���������*/
    if( VOS_TRUE == pstConn->pstBizChannel->ulExitConfirm )
    {
        /*���ﲻӦ�÷���*/
        VOS_Printf("system error!");
        return VOS_ERR;
    }

    switch(ulCtrlCmd)
    {
        case SWM_CTRLCMD_SNDOUT_COMPELETED:
        {
            /*�鿴��ǰ�ķ���������Ƿ���Ҫ��������*/
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
    
    //VOS_Printf("UPF_Conn_PipeConnDelNotifyProc Entry!");
    
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
        /*���ﲻӦ�÷���*/
        VOS_Printf("system error!");
        return;
    }
    
    (VOID)UPF_Conn_Release(pstUPFConn);

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
            SWM_PIPE_TYPE_BIZ_UPF,
            UPF_Conn_PipeConnDataUpperProc,             /*��������*/
            UPF_Conn_PipeConnDataDownProc,              /*��������*/
            UPF_Conn_PipeConnCtrlProc,                  /*���ܵ��Ŀ���*/
            UPF_Conn_PipeConnDelNotifyProc,             /*ɾ������*/
            pstConn);
    
    /*�������BizChannelͨ���Ĺܵ�������*/
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

    /*��Ҫ: BizChannel �����뱾�ڵ�*/
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


