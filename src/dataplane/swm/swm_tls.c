/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_tls.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : TLS�ķ��ͺͽ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"


/*****************************************************************************
 �� �� ��  : SWM_TLS_ConnExpireConfirm
 ��������  : �����ڵ�����ϻ����
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
VOID SWM_TLS_ConnExpireConfirm(SWM_TLS_CONN_S *pstTlsConn)
{
    if ( NULL == pstTlsConn )
    {
        VOS_Printf("param error");
        return;
    }

    /*����ȷ�Ͽ����ϻ�*/
    pstTlsConn->stExpireOps.ulExpireConfirm = VOS_TRUE;
    
    return;
}

/*****************************************************************************
 �� �� ��  : SWM_TLS_DelNotify
 ��������  : ֪ͨ����ҵ��ɾ��
 �������  : SWM_BIZ_CHANNEL_S  *pstBizChannel  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��6��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    /*��Ҫ��һʱ����ɾ����ע�������¼�,
    �ر��ں˲����Ĵ�������������¼�
    ��������ڵ�ҲҪ��ô����*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps) )
    {
        VOS_Printf("rct net unregister error!");
    }
    
    /*���ñ��ڵ�����ϻ�*/
    SWM_TLS_ConnExpireConfirm(pstTlsConn);
    
    /*����Ѿ��������ã��Ͳ��ü���֪ͨ�ˣ�
        ��ʾ�Ѿ������ڱ���֪ͨ�ϻ���*/
    if ( pstTlsConn->pstBizChannel->ulExitConfirm == VOS_TRUE )
    {
        return;
    }
    
    /*��������һ�£���ֹ�����ټ���֪ͨ,���濪ʼ֪ͨ����ҵ��ܵ��ڵ�*/
    pstTlsConn->pstBizChannel->ulExitConfirm = VOS_TRUE;
    
    pstListHead = &pstTlsConn->pstBizChannel->stPipeHead;
    
    /*֪ͨ���иùܵ��ڵ�����˳�*/    
    VOS_DLIST_FOR_EACH_ENTRY(pstPipeNext, pstListHead, SWM_PIPE_CONN_S, stNode)
    {
        if ( NULL == pstPipeNext )
        {
            VOS_Printf("Not found the next pipe node!");
            break;
        }

        /*���ڵ����TLSԴ�ڵ�*/
        if ( pstPipeNext->ePipeType == SWM_PIPE_TYPE_SRC_TLS )
        {
            /*�����Լ����ٽ���ú����������ݹ�ѭ��*/
            continue;
        }

        /*������һ���ڵ�, ֪ͨ����ɾ��*/
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
 �� �� ��  : SWM_TLS_ConnRecvCb
 ��������  : ���������¼�����
 �������  : VOID *pvTlsConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    /*��������´���һ��iobuf,
       iobuf�����Źܵ�һ��һ�����������ʹ���, ���һ����Ҫ�ͷ�*/
    if ( NULL == pstTlsConn->pstRecvIobuf )
    {    
        /*����TCP���͵�IOBUF�ڴ�*/
        pstTlsConn->pstRecvIobuf = COM_Iobuf_Malloc(SWM_MID_SID_TLS);
        if ( NULL == pstTlsConn->pstRecvIobuf )
        {
            VOS_Printf("iobuf create error!");
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
    }
    
    /*����Ҳ��������һ��ֻ������һ���ֽڣ�������Ҫ�����յ�
    ������ɵĻ����Ὣ���iobuf���͵���һ���ڵ�*/
    lRecvLen = COM_IOBUF_GETSPACE_LEN(pstTlsConn->pstRecvIobuf);
    if ( 0 >= lRecvLen )
    {
        VOS_Printf("iobuf get free len error,  lRecvLen=%d!", lRecvLen);
        SWM_TLS_ConnDelNotify(pstTlsConn);
        return;
    }
    

    VOS_Printf("swm tls conn recv cb entry, connfd=%d!", pstTlsConn->lConnfd);
    
    /*��ȡ����*/
    //lError = UTL_SSL_Read(pstTlsConn->pstSsl, pcData, lRecvLen, &lErrorStatus);
    /*�°���ʼ��ʱ����Ҫ�Ƚ���ͷ����Ϣ*/
    if ( pstTlsConn->pstRecvIobuf->ulPreDataLen == COM_IOBUF_LEN )
    {
        lLeftDataLen = SWM_BIZ_HEAD_LEN;
    }
    else
    {
        lLeftDataLen = pstTlsConn->pstRecvIobuf->ulPreDataLen - pstTlsConn->pstRecvIobuf->ulDataLen;
    }
    
    /*��ȡ���е����ݿռ�*/
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

        /*�����Ѿ���������һ���ڵ㣬��Ҫ֪ͨ��ʽɾ��*/
        SWM_TLS_ConnDelNotify(pstTlsConn);
        
        return;
    }
    else        
    {
         if ( 1 == lError )
         {
             /* ���յ�1���ֽ�,���ü������� */
             VOS_Printf("UTL_SSL_Read must be continue!");
             /*���ý��յ����µĳ���*/
             COM_IOBUF_SETINPUTED_LEN(pstTlsConn->pstRecvIobuf, lError);
             return;
         }
         
         /*��������*/
         COM_IOBUF_SETINPUTED_LEN(pstTlsConn->pstRecvIobuf, lError);
    }

    /* �ϸ���ݳ��ȸ��������գ���Ҫ��ȡһ�����յ�׼ȷ�ĳ�����Ϣ */
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
    
    /*�ϸ������������ȣ��Ƿ���Ҫ��������*/
    lRet = SWM_Biz_ChannelCheckLen(pstTlsConn->pstRecvIobuf->pcData, lDataLen);
    if ( VOS_OK != lRet )
    {   
        /*��ʾ��Ҫ��������*/
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
    
    /*���������յ��󣬿�ʼ����ҵ��ʶ����*/
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

    /*�����յ���Iobuf ���͸���һ���ڵ� */
    lRet = SWM_TLS_PipeTransBufToNextPipeNode(&pstTlsConn->stTlsPipe, pstTlsConn->pstRecvIobuf);
    switch(lRet)
    {
        case SWM_PIPE_IOBUF_OK:
            /*���ֶ���ʾ���ͳɹ�*/
            /*TODO: ͳ������*/
            VOS_Printf("Tls connect node pipe to next node ok!!");
            /*������ָ����Ҫ���¸�ֵΪ��*/
            pstTlsConn->pstRecvIobuf = NULL;
            break;
        case SWM_PIPE_IOBUF_EWOULDBLOCK:
            /*����Ҫ�ر������Ľ���*/
            VOS_Printf("Tls connect node pipe to next node ewouldblock!");
            (VOID)RCT_API_NetOpsEventCtrl(&pstTlsConn->stNetEvtOps, VOS_EPOLL_CTRL_INCLOSE);            
            return;
        case SWM_PIPE_IOBUF_AGAIN:
            /*�������յĻ���ʲô������*/
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
 �� �� ��  : SWM_TLS_ConnSendCb
 ��������  : �����¼�����
 �������  : VOID *pvTlsConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    /*�ȿ��������ǲ��ǿյ�,����ǿյģ���رշ���*/
    if ( VOS_TRUE == COM_Iobuf_QueIsEmpty(pstTlsConn->pstBizChannel->pstSwmSendQueue))
    {
        /*�رշ���*/
        (VOID)RCT_API_NetOpsEventCtrl(&pstTlsConn->stNetEvtOps, VOS_EPOLL_CTRL_OUTCLOSE);
        return;
    }

    /*��һ�λ�û�з��꣬��Ҫ��������*/
    if ( NULL != pstTlsConn->pstSendIobuf )
    {
        VOS_Printf("Tls send iobuf is not empty , must be send again!");
        goto SendAgain;
    }

    /*���Ͷ��е���Ŀ����Ϊ�տ϶���>0*/
    ulNums = COM_Iobuf_QueGetNums(pstTlsConn->pstBizChannel->pstSwmSendQueue);
            
    for(ulIndex = 0; ulIndex < ulNums; ulIndex++)
    {
        /*�����ȳ�������β���룬����ͷȡ��*/
        pstIobufTmp = COM_Iobuf_QuePop(pstTlsConn->pstBizChannel->pstSwmSendQueue);
        if ( NULL == pstIobufTmp )
        {
            /*��Ӧ�÷���,ϵͳ����*/
            VOS_Printf("tls pop the iobuf nums error!");
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
        
        /*���뵽��������*/
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

            /*�������������Ѿ��������,���������е�buf���з���*/
            if ( lLeftLen == 0 )
            {
                COM_Iobuf_Free(pstTlsConn->pstSendIobuf);
                pstTlsConn->pstSendIobuf = NULL;
                continue;
            }
            else if ( 0 < lLeftLen )
            {
                /*���м���ʣ��*/
                goto SendAgain;
            }
        }
        else 
        {
            /*TODO:�����������ʾ�����ˣ�������Ҫ���һ���ԵĶ�ʱ������ͣ�ĳ���*/
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
    
    /*SSL �������, ��û���������״̬*/
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
            /*SSLЭ��ʧ�ܣ�û�в�����һ���ڵ㣬ֱ���ͷ�*/
            SWM_TLS_ConnDelNotify(pstTlsConn);
            return;
        }
    }
    #endif
    return;
}

/*****************************************************************************
 �� �� ��  : SWM_TLS_ConnExpireCb
 ��������  : �ϻ�����
 �������  : VOID *pvTlsConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
        
    /*�ϻ��ڵ�Ĺ���:
       1. �����ulExpireConfirmֻ����DelNotify������
       2. Notify��Ҫ֪ͨ��һ���ڵ�����ϻ�
       3. Notify�Լ������������ϻ���ֻ�����ñ��λ
           ���е�ҵ�����ϻ�����У����BizChannel-->ulExitConfirm��ǣ�Ȼ������ϻ�
       4. ���нڵ�ľ����ϻ�������ExpireCB�ص��н��л���*/
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
 �� �� ��  : SWM_TLS_ConnCreate
 ��������  : �������յ���TLS�ڵ�
 �������  : SWM_MSG_ADDCONN_S *pstMsgInfo  
 �������  : ��
 �� �� ֵ  : SWM_TLS_CONN_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��13��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_TLS_ConnCreate(SWM_MSG_ADDCONN_S *pstMsgInfo)
{
    SWM_TLS_CONN_S *pstTlsConn      = NULL;
    

    if ( NULL == pstMsgInfo )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    
    /*����TLS�ڵ��ڴ�*/
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
    
    /*����socketע�������¼�*/
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

    /*�ϻ�ע��*/
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
 �� �� ��  : SWM_TLS_ConnRelease
 ��������  : �ͷ�TLS�ڵ�
 �������  : SWM_TLS_CONN_S *pstTlsConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��13��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG SWM_TLS_ConnRelease(SWM_TLS_CONN_S *pstTlsConn)
{
    if ( NULL == pstTlsConn )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    VOS_Printf("SWM_TLS_ConnRelease start!fd=[%d]", pstTlsConn->lConnfd);
    
    /* ������ȥע�����DelNotify��,���ٹرգ���������������0�����¼�*/
    if(VOS_ERR == RCT_API_NetOpsEventUnRegister(&pstTlsConn->stNetEvtOps) )
    {
        VOS_Printf("RCT_Reactor_NetEvtOptsUnRegister error!");
    }
    
    VOS_SOCK_Shutdown(pstTlsConn->lConnfd);
    
    /*ע��,һ��Ҫ��ע���ϻ�,������ٴν���,��������*/
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



