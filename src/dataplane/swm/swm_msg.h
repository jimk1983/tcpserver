/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_msg.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月4日
  最近修改   :
  功能描述   : SWM的消息
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月4日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/



/*根据消息类型处理对应的结构信息*/
typedef enum
{
    SWM_MSGTYPE_UNKNOW=0,
    SWM_MSGTYPE_ADDTLSCONN,        /*网关监听客户端节点添加*/

    SWM_MSGTYPE_NUMS,
}SWM_MSG_TYPE_E;


typedef struct tagSwmMsgAddConn
{
    /*每次消息交互最好都带上这个头，方便消息交互
    头部信息，通过函数:RCT_API_EnvGetPthreadConfInfo()来获取*/
    ULONG                       ulMsgCode;
    /************************************/
    /*网关监听到的客户端socket*/
    LONG                        lConnfd;
    /*本虚拟网关的ID*/
    ULONG                       ulVtID;
    /*网关的SSL上下文*/
    //SSL_CTX_S                  *pstSSLCtx;
    
}SWM_MSG_ADDCONN_S;


LONG SWM_MSG_AddTlsConn(  RCTMSG_DSTINFO_S *pstDstInfo, 
                               LONG lConnfd,
                               ULONG ulVtID);


