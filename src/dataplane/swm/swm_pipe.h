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

/*SWM 管道进行水位控制*/
/*高水位, 发送队列超过高水位，意味着要关闭内网接收，
并且打开外网不停的尝试发送*/
#define SWM_PIPE_IOBUF_THLD_HIGH        10    

/*低水位, 进入低水位时候，则打开内网接收即可*/
#define SWM_PIPE_IOBUF_THLD_LOW         2         


/*SSL线程的管道节点的业务类型*/
typedef enum SwmPipeType
{
    SWM_PIPE_TYPE_SRC_TLS = 0,          /*TLS源节点*/
    SWM_PIPE_TYPE_SRC_DTLS,             /*UDPS源节点*/
    SWM_PIPE_TYPE_FILTER,               /*中间过滤节点*/
    SWM_PIPE_TYPE_BIZ_UPF,              /*网络扩展的业务节点*/
    SWM_PIPE_TYPE_BIZ_NEM,              /*网络扩展的业务节点*/
    SWM_PIPE_TYPE_BIZ_PFW,              /*端口转发的业务节点*/
    SWM_PIPE_TYPE_BIZ_TCM,              /*终端检查的业务节点*/
    
    SWM_PIPE_TYPE_END
}SWM_PIPE_TYPE_E;


/*数据处理上行*/
typedef LONG (*swm_pipe_dataupperproc_cb)(VOID *pvhandler, COM_IOBUF_S *pstIobuf);

/*数据处理下行*/
typedef LONG (*swm_pipe_datadownproc_cb)(VOID *pvhandler, COM_IOBUF_S *pstIobuf);

/*控制消息处理*/
typedef LONG (*swm_pipe_ctrlproc_cb)(VOID *pvhandler, ULONG ulCtrlCmd);

/*删除消息处理*/
typedef VOID (*swm_pipe_delnotify_cb)(VOID *pvhandler);


typedef struct tagSwmPipeCallBack SWM_PIPE_CALLBACK_S;

/*管道的执行函数结构*/
struct tagSwmPipeCallBack
{
    VOID *pvcbFunc;
    VOID *pvUserData;
};


/*回调函数初始化*/
#define SWM_PIPE_CALLBACK_INIT(pstCallBack_, pfFunc_, pvUserData_)\
do{\
    (pstCallBack_)->pvcbFunc = (VOID *)pfFunc_;\
    (pstCallBack_)->pvUserData = (VOID *)pvUserData_;\
}while(0);

/*SWM模块的管道节点，对应的每一个SSL链接*/
typedef struct tagSwmPipeConn
{
    VOS_DLIST_NODE_S          stNode;                        /*本管道节点*/
    SWM_PIPE_TYPE_E            ePipeType;                   /*管道类型*/
    
    SWM_PIPE_CALLBACK_S     stPipeDataUpperProc;   /*本管道的执行函数，数据上行*/
    SWM_PIPE_CALLBACK_S     stPipeDataDownProc;    /*本管道的执行函数，数据下行*/
    SWM_PIPE_CALLBACK_S     stPipeCtrlProc;              /*本管道的控制函数, 关闭或者打开本管道的收发*/
    SWM_PIPE_CALLBACK_S     stPipeDelNotifyProc;      /*本管道的删除通知函数*/
}SWM_PIPE_CONN_S;

/*管道节点的初始化*/
#define SWM_PIPE_CONN_INIT(pstPipeConns_,ePipeType_, pfDataUpper, pfDataDown,pfCtrl,pfDelNotify,pvData_) do{\
    VOS_DLIST_INIT(&(pstPipeConns_)->stNode);\
    (pstPipeConns_)->ePipeType  =  (ePipeType_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeDataUpperProc, pfDataUpper, pvData_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeDataDownProc, pfDataDown, pvData_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeCtrlProc, pfCtrl, pvData_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeDelNotifyProc, pfDelNotify, pvData_);\
}while(0);

LONG    SWM_TLS_PipeConnCreate(SWM_TLS_CONN_S *pstTlsConn);

VOID    SWM_TLS_PipeConnRelease(SWM_TLS_CONN_S *pstTlsConn);


#define SWM_PIPE_IOBUF_OK                               0
#define SWM_PIPE_IOBUF_UNKNOW_ERR             -1        /*-1: 未知错误*/
#define SWM_PIPE_IOBUF_EWOULDBLOCK            -2       /*-2: 正常，非阻塞*/
#define SWM_PIPE_IOBUF_PARAM_ERR                -3       /*-3: 参数检查出错*/
#define SWM_PIPE_IOBUF_PIPENODE_ERR           -4       /*-4: 找不到管道节点*/
#define SWM_PIPE_IOBUF_AGAIN                         -5       /*-5, 需要继续接收*/
#define SWM_PIPE_IOBUF_PIPEFUNC_ERR           -6       /*-6, 回调函数未注册*/

LONG    SWM_TLS_PipeTransBufToNextPipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, COM_IOBUF_S *pstIobuf);

LONG    SWM_TLS_PipeTransBufToPrePipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, COM_IOBUF_S *pstIobuf);

SWM_PIPE_CONN_S *SWM_TLS_PipeConnFoundTlsNode(SWM_BIZ_CHANNEL_S   *pstBizChannel);

LONG    SWM_TLS_PipeTransCtrlToNextPipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, ULONG ulCmdCode);

LONG    SWM_TLS_PipeTransCtrlToPrePipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, ULONG ulCmdCode);




