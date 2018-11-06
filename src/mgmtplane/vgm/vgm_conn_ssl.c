/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vgm_conn_ssl.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��2��
  ����޸�   :
  ��������   : ����ssl���صĽڵ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��2��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"
#include "vgm/vgm_pub.h"



/*****************************************************************************
 �� �� ��  : VGM_SSLConn_TLSDispatch
 ��������  : �ַ���TLS��ҵ���̴߳���
 �������  :   ULONG ulVtID     --��������������ID
                            LONG lClientfd     --���յ��Ŀͻ���FD
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VGM_SSLConn_TLSDispatch(ULONG ulVtID, LONG lClientfd)
{
    VGM_CTX_S *pstVgCtx         = NULL;
    static ULONG ulPollIndex    = 0;
    ULONG        iNums          = 0;
    RCTMSG_DSTINFO_S stDstInfo  = {0};
    
    pstVgCtx = VGM_CFG_GetVTCtxByVTID(ulVtID);
    if ( NULL == pstVgCtx )
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }

    switch(pstVgCtx->uiBizType )
    {  
        case VGM_VTBIZTYPE_POLLING:
            
            /*��Ϊ��0��ʼ, ��Num-1, POLL�㷨����ѯ�Ϳ���*/
            iNums = RCT_API_EnvGetTypeCfgSubNums(RCT_TYPE_DP_SSL);
            if ( ulPollIndex >= (iNums -1)  )
            {
                ulPollIndex =0;
            }
            else
            {
                ulPollIndex++;
            }
            
            stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //һ�㶼�����
            stDstInfo.ulDstRctType = RCT_TYPE_DP_SSL;       
            stDstInfo.ulDstSubType = RCT_SUBTYPE_SSL_SWM;   //SSL�ж���ҵ��ڵ�����
            stDstInfo.ulDstPthIndex = ulPollIndex;
                
            /*ѭ���Ľ���Ӧ��clientfd���͵�������ѯ���߳���*/
            if ( VOS_ERR == SWM_MSG_AddTlsConn(&stDstInfo,lClientfd, ulVtID))
            {
                VOS_Printf("Send to swm msg add tls conn-node error!");
                VOS_SOCK_Close(lClientfd);
            }
            break;
        case VGM_VTBIZTYPE_LASTCON:
            
            break;
        case VGM_VTBIZTYPE_LASTFLOW:
            
            break;
        default:
            break;
        
    }

    return VOS_OK;
}



/*****************************************************************************
 �� �� ��  : VGM_SSLConn_AcceptCb
 ��������  : ���սڵ�
 �������  : VOID *pvHandler  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_SSLConn_AcceptCb(VOID *pvHandler)
{
    LONG lListenSockfd = 0;
    LONG lClientFd = 0;
    LONG ulErrorStatus = 0;
    USHORT usPort = 0;
    CHAR acAddrIp[VOS_SOCK_IPADDR_LEN] = {0};
    VGM_SSL_LISTEN_CONN_S *pstListenConn = NULL;
    
    if ( NULL == pvHandler )
    {
        VOS_Printf("param error!");
        return;
    }
    
    VOS_Printf("vgm listen accept Entry!");

    pstListenConn = (VGM_SSL_LISTEN_CONN_S *)pvHandler;

    lListenSockfd = pstListenConn->lListenSockfd;

    /*���մ���*/
    lClientFd = VOS_SOCK_Accept(lListenSockfd, acAddrIp, &usPort, &ulErrorStatus);
    if ( VOS_ERR == lClientFd )
    {
        if ( VOS_SOCK_ERROR == ulErrorStatus )
        {
            VOS_Printf("VOS_SOCK_Accept error !");
            VGM_SSLConn_VTDelete(pstListenConn->ulVtID);
            return;
        }
        else if( VOS_SOCK_EWOULDBLOCK == ulErrorStatus)
        {
            VOS_Printf("VOS_SOCK_Accept EWouldBlock !");
        }
    }
    else
    {
        VOS_Printf("Accepted a new clientfd=[%d], addr=[%s], port=[%d]!", lClientFd, acAddrIp, usPort );
        
        if ( VOS_ERR == VGM_SSLConn_TLSDispatch(pstListenConn->ulVtID, lClientFd))
        {
            VOS_SOCK_Close(lClientFd);
            VOS_Printf("listen add tls conn send  error !");
            return;
        }
    }
    
    return;
}

/*****************************************************************************
 �� �� ��  : VGM_ADD_CtxLockInit
 ��������  : ���������������������ĳ�ʼ��
 �������  : VGM_CTX_LOCK_S *pstCtxLock  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��30��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
static VOID VGM_SSLConn_CtxLockInit(VGM_CTX_LOCK_S *pstCtxLock)
{
    if ( NULL ==  pstCtxLock )
    {
        VOS_Printf("param error!");
        return;
    }

    VOS_RWLOCK_INIT(pstCtxLock->stLockListenConn);
    
    return;
}


/*****************************************************************************
 �� �� ��  : VGM_SSLConn_ListenCreate
 ��������  : SSL�����ڵ㴴��
 �������  : CHAR  *pcIpaddr  
             UINT32 uiPort    
 �������  : ��
 �� �� ֵ  : VGM_SSL_LISTEN_CONN_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VGM_SSL_LISTEN_CONN_S *VGM_SSLConn_ListenCreate(CHAR  *pcIpaddr,UINT32 uiPort)
{
    VGM_SSL_LISTEN_CONN_S *pstListenConn    = NULL;
    
    if ( NULL == pcIpaddr 
        || 0 == uiPort )
    {
        VOS_Printf("param error!");
        return NULL;
    }
        
    /*����ýڵ��ڴ�*/
    pstListenConn = (VGM_SSL_LISTEN_CONN_S  *)VOS_Malloc(VGM_MID_SID_SSL, sizeof(VGM_SSL_LISTEN_CONN_S));
    if ( NULL == pstListenConn )
    {
        VOS_Printf("malloc error !");
        return NULL;
    }

    VOS_Mem_Zero((CHAR *)pstListenConn, sizeof(VGM_SSL_LISTEN_CONN_S));

    /*�ȴ���TCP SOCKET*/
    pstListenConn->lListenSockfd = VOS_SOCK_Create(VOS_SOCK_TYPE_TCP);
    
    if ( VOS_SOCKET_INVALID ==  pstListenConn->lListenSockfd )
    {
        VOS_Printf("VOS_SOCK_Create error!");
        VOS_Free((CHAR *)(pstListenConn));
        return NULL;
    }
    else
    {
        VOS_Printf("Create the vgm ssl ListenSockfd=[%d]!", pstListenConn->lListenSockfd);
    }

    if(VOS_ERR == VOS_SOCK_SetOption(pstListenConn->lListenSockfd))
    {
        VOS_Printf("VOS_SOCK_SetOption() error!");
        VOS_SOCK_Close(pstListenConn->lListenSockfd);
        VOS_Free((CHAR *)(pstListenConn));
        return NULL;
    }

    if ( VOS_ERR ==  VOS_SOCK_Bind(pstListenConn->lListenSockfd, pcIpaddr, uiPort))
    {
        VOS_Printf("VOS_SOCK_Bind() error!");
        VOS_SOCK_Close(pstListenConn->lListenSockfd);
        VOS_Free((CHAR *)(pstListenConn));
        return NULL;
    }

    /*�������У�ÿ�μ�������10���ͻ���*/
    if ( VOS_ERR == VOS_SOCK_Listen(pstListenConn->lListenSockfd, VGM_SSLLISTEN_DEFAULT_NUMS) )
    {
        VOS_Printf("VOS_SOCK_Listen() error!");
        VOS_SOCK_Close(pstListenConn->lListenSockfd);
        VOS_Free((CHAR *)(pstListenConn));
        return NULL;
    }

    /*�ü�������������սӿ�*/
    if ( VOS_ERR == RCT_API_NetOpsEventRegister(&pstListenConn->stNetOps, 
                                                pstListenConn->lListenSockfd,
                                                VOS_EPOLL_MASK_POLLIN,
                                                VGM_SSLConn_AcceptCb,
                                                NULL,
                                                pstListenConn))
    {
        VOS_Printf("VOS_SOCK_Listen() error!");
        VOS_SOCK_Close(pstListenConn->lListenSockfd);
        VOS_Free((CHAR *)(pstListenConn));
        return NULL;
    }
    
    VOS_Printf("vgm create the listen socket [%d] is ok!", pstListenConn->lListenSockfd);

    return pstListenConn;
}



/*****************************************************************************
 �� �� ��  : VGM_SSLConn_ListenRelease
 ��������  : �ͷż����ڵ�
 �������  : VGM_SSL_LISTEN_CONN_S *pstListenConn  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��30��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_SSLConn_ListenRelease(VGM_SSL_LISTEN_CONN_S *pstListenConn)
{
    if ( NULL != pstListenConn )
    {   
        RCT_API_NetOpsEventUnRegister(&pstListenConn->stNetOps);
        VOS_SOCK_Close(pstListenConn->lListenSockfd);
        VOS_Free((CHAR *)(pstListenConn));
    }
}



/*****************************************************************************
 �� �� ��  : VGM_SSLConn_VTCreate
 ��������  : �������ش���
 �������  :   CHAR  *pcVgName      --������������������ 
           CHAR  *pcIpaddr      --�������������ؼ�����IP��ַ
           UINT32 uiPort        --�������������ؼ����Ķ˿�
           UINT32 uiBizType     --�������������ؿͻ��˷ַ��㷨
                                  (��ѯ����С����������С������)
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VGM_SSLConn_VTCreate(USHORT usPort)
{
    VGM_CTX_S           *pstVgmCtx = NULL;
    
    pstVgmCtx = (VGM_CTX_S *)VOS_Malloc(VGM_MID_SID_CFG, sizeof(VGM_CTX_S));
    if ( NULL == pstVgmCtx )
    {
        VOS_Printf("malloc error!");
        return VOS_ERR;
    }
    
    VOS_Mem_Zero((CHAR *)pstVgmCtx, sizeof(VGM_CTX_S));

    /*��ȡ���е���������ָ��*/
    pstVgmCtx->ulVtID       = VGM_CFG_GetFreeVTID();
    pstVgmCtx->ulVTPort     = usPort;
    /*Ĭ��ʹ����ѯ��ʽ*/
    pstVgmCtx->uiBizType  	= VGM_VTBIZTYPE_POLLING;
    
    /*С�ķ�ֹ�ڴ�Խ��*/
    if ( pstVgmCtx->ulVtID >= VGM_CFG_ARRAY_NUMS )
    {
        VOS_Printf("get free vt id error!");
        VOS_Free((CHAR *)pstVgmCtx);
        return VOS_ERR; 
    }
    
    VGM_SSLConn_CtxLockInit(&pstVgmCtx->stVTLocks);
    

    /*SSL�����ڵ��ʼ��*/
    pstVgmCtx->pstSslListenConn = VGM_SSLConn_ListenCreate(VGM_DEFAULT_IPADDR, usPort);
    if ( NULL == pstVgmCtx->pstSslListenConn )
    {
        VOS_Printf("VGM_SSLConn_ListenCreate failed!");
        VOS_Free((CHAR *)pstVgmCtx);
        return VOS_ERR;   
    }

    pstVgmCtx->pstSslListenConn->ulVtID = pstVgmCtx->ulVtID;

    /*���浽������*/
    if( VOS_ERR == VGM_CFG_SetVTCtxByVTID(pstVgmCtx, pstVgmCtx->ulVtID) )
    {
        VOS_Printf("set vt ctx failed!");
        VGM_SSLConn_ListenRelease(pstVgmCtx->pstSslListenConn);
        VOS_Free((CHAR *)pstVgmCtx);
        return VOS_ERR;  
    }
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VGM_CFG_VTFree
 ��������  : ͨ��VTID���ͷŶ�Ӧ��������Դ
 �������  : ULONG ulVTid  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_SSLConn_VTDelete(ULONG ulVTid)
{
    VGM_CTX_S *pstVgmCtx = NULL;
    
    pstVgmCtx = VGM_CFG_GetVTCtxByVTID(ulVTid);
    if ( NULL != pstVgmCtx )
    {
        VGM_SSLConn_ListenRelease(pstVgmCtx->pstSslListenConn);
        VGM_CFG_DelVTCtxByVTID(ulVTid);  
        pstVgmCtx = NULL;
    }
}






