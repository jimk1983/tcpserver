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
    

/*挂载的管道起始数据接收节点*/
struct tagSwmTlsConn
{
    /*当前节点, 挂载在pipe上*/
    VOS_DLIST_NODE_S            stNode;
    /*当前起始TLS节点的管道处理*/
    SWM_PIPE_CONN_S             stTlsPipe;
    
    /*定义本节点的网络事件*/
    RCT_NETOPT_EVENT_S         stNetEvtOps;
    /*定义本节点的老化事件*/
    RCT_EXPIROPT_EVENT_S      stExpireOps;
    /*定义本节点的定时器事件*/
    //RCT_TIMEROPT_EVENT_S      stTimerOps;
    
    /*业务通道*/
    SWM_BIZ_CHANNEL_S         *pstBizChannel;

    /*当前的socketfd*/
    LONG                        lConnfd;
    /*当前的ssl指针*/
    //SSL_S                       *pstSsl;
    /*SSL握手的状态*/
    LONG                        lHandShakeStatus;

    /*记录当前的socket iobuf*/
    COM_IOBUF_S                    *pstRecvIobuf;
    /*推送回客户端的*/
    COM_IOBUF_S                    *pstSendIobuf;
    
};


LONG    SWM_TLS_ConnCreate(SWM_MSG_ADDCONN_S *pstMsgInfo);

LONG    SWM_TLS_ConnRelease(SWM_TLS_CONN_S *pstTlsConn);

VOID    SWM_TLS_ConnExpireConfirm(SWM_TLS_CONN_S *pstTlsConn);

