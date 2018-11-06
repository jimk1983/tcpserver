/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vgm_conn_ssl.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月2日
  最近修改   :
  功能描述   : 创建ssl网关的节点
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月2日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"
#include "vgm/vgm_pub.h"



/*****************************************************************************
 函 数 名  : VGM_SSLConn_TLSDispatch
 功能描述  : 分发到TLS的业务线程处理
 输入参数  :   ULONG ulVtID     --所属的虚拟网关ID
                            LONG lClientfd     --接收到的客户端FD
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

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
            
            /*因为从0开始, 到Num-1, POLL算法，轮询就可以*/
            iNums = RCT_API_EnvGetTypeCfgSubNums(RCT_TYPE_DP_SSL);
            if ( ulPollIndex >= (iNums -1)  )
            {
                ulPollIndex =0;
            }
            else
            {
                ulPollIndex++;
            }
            
            stDstInfo.ulSrcSubType = RCT_SUBTYPE_SINGLE;    //一般都是这个
            stDstInfo.ulDstRctType = RCT_TYPE_DP_SSL;       
            stDstInfo.ulDstSubType = RCT_SUBTYPE_SSL_SWM;   //SSL有多种业务节点类型
            stDstInfo.ulDstPthIndex = ulPollIndex;
                
            /*循环的将对应的clientfd发送到负载轮询的线程中*/
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
 函 数 名  : VGM_SSLConn_AcceptCb
 功能描述  : 接收节点
 输入参数  : VOID *pvHandler  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*接收处理*/
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
 函 数 名  : VGM_ADD_CtxLockInit
 功能描述  : 该虚拟网关上下文中锁的初始化
 输入参数  : VGM_CTX_LOCK_S *pstCtxLock  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月30日
    作    者   : jimk
    修改内容   : 新生成函数

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
 函 数 名  : VGM_SSLConn_ListenCreate
 功能描述  : SSL监听节点创建
 输入参数  : CHAR  *pcIpaddr  
             UINT32 uiPort    
 输出参数  : 无
 返 回 值  : VGM_SSL_LISTEN_CONN_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

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
        
    /*申请该节点内存*/
    pstListenConn = (VGM_SSL_LISTEN_CONN_S  *)VOS_Malloc(VGM_MID_SID_SSL, sizeof(VGM_SSL_LISTEN_CONN_S));
    if ( NULL == pstListenConn )
    {
        VOS_Printf("malloc error !");
        return NULL;
    }

    VOS_Mem_Zero((CHAR *)pstListenConn, sizeof(VGM_SSL_LISTEN_CONN_S));

    /*先创建TCP SOCKET*/
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

    /*监听队列，每次监听处理10个客户端*/
    if ( VOS_ERR == VOS_SOCK_Listen(pstListenConn->lListenSockfd, VGM_SSLLISTEN_DEFAULT_NUMS) )
    {
        VOS_Printf("VOS_SOCK_Listen() error!");
        VOS_SOCK_Close(pstListenConn->lListenSockfd);
        VOS_Free((CHAR *)(pstListenConn));
        return NULL;
    }

    /*该监听仅仅处理接收接口*/
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
 函 数 名  : VGM_SSLConn_ListenRelease
 功能描述  : 释放监听节点
 输入参数  : VGM_SSL_LISTEN_CONN_S *pstListenConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月30日
    作    者   : jimk
    修改内容   : 新生成函数

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
 函 数 名  : VGM_SSLConn_VTCreate
 功能描述  : 虚拟网关创建
 输入参数  :   CHAR  *pcVgName      --创建的虚拟网关名称 
           CHAR  *pcIpaddr      --创建的虚拟网关监听的IP地址
           UINT32 uiPort        --创建的虚拟网关监听的端口
           UINT32 uiBizType     --创建的虚拟网关客户端分发算法
                                  (轮询、最小连接数、最小流量等)
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*获取空闲的虚拟网关指针*/
    pstVgmCtx->ulVtID       = VGM_CFG_GetFreeVTID();
    pstVgmCtx->ulVTPort     = usPort;
    /*默认使用轮询方式*/
    pstVgmCtx->uiBizType  	= VGM_VTBIZTYPE_POLLING;
    
    /*小心防止内存越界*/
    if ( pstVgmCtx->ulVtID >= VGM_CFG_ARRAY_NUMS )
    {
        VOS_Printf("get free vt id error!");
        VOS_Free((CHAR *)pstVgmCtx);
        return VOS_ERR; 
    }
    
    VGM_SSLConn_CtxLockInit(&pstVgmCtx->stVTLocks);
    

    /*SSL监听节点初始化*/
    pstVgmCtx->pstSslListenConn = VGM_SSLConn_ListenCreate(VGM_DEFAULT_IPADDR, usPort);
    if ( NULL == pstVgmCtx->pstSslListenConn )
    {
        VOS_Printf("VGM_SSLConn_ListenCreate failed!");
        VOS_Free((CHAR *)pstVgmCtx);
        return VOS_ERR;   
    }

    pstVgmCtx->pstSslListenConn->ulVtID = pstVgmCtx->ulVtID;

    /*保存到配置中*/
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
 函 数 名  : VGM_CFG_VTFree
 功能描述  : 通过VTID来释放对应的网关资源
 输入参数  : ULONG ulVTid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

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






