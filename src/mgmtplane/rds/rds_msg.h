/******************************************************************************

                  版权所有 (C), 2017-2020, 网络有限公司

 ******************************************************************************
  文 件 名   : rds_tminfo.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2018年8月17日
  最近修改   :
  功能描述   : redis的终端信息接口
  函数列表   :
  修改历史   :
  1.日    期   : 2018年8月17日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

/*参考UPF模块的定义即可，主要给该模块使用*/

/*设备标签长度*/
#define RDS_UPF_DEVLAB_LEN      64
#define RDS_UPF_FILELEN         32
#define RDS_UPF_FILEVALEN       36


/*根据消息类型处理对应的结构信息*/
typedef enum
{
    RDS_MSGTYPE_UNKNOW=0,
    RDS_MSGTYPE_TerminalInfoAdd,        /*添加客户端状态*/
    RDS_MSGTYPE_TerminalInfoDel,        /*删除客户端状态*/
    RDS_MSGTYPE_TerminalInfoUpdate,     /*更新客户端状态*/
    RDS_MSGTYPE_RedisServerAdd,         /*Redis服务器添加*/
    RDS_MSGTYPE_RedisServerDel,         /*Redis服务器删除*/

    RDS_MSGTYPE_NUMS,
}RDS_MSG_TYPE_E;


typedef struct tagRdsMsgTerminalInfoAdd
{
    /*固定消息头，通过函数:RCT_API_EnvGetPthreadConfInfo()*/
    ULONG                       ulMsgCode;
    /************************************/
    /*网关监听到的客户端socket*/
    CHAR        acTerminalID[RDS_UPF_DEVLAB_LEN];       /*设备标签*/
    CHAR        acTerminalDecptor[RDS_UPF_DEVLAB_LEN];       /*设备描述*/
    UINT32      uiClientAddr;                           /*客户端地址信息*/
    UINT32      uiClientPort;                           /*客户端断开信息*/
}RDS_MSG_TMINFOADD_S, *PRDS_MSG_TMINFOADD_S;

LONG RDS_MSG_TerminalInfoAdd(CHAR*  pcTerminalID, 
                                   CHAR*  pcTerminalDesptor,
                                   UINT32 uiCliAddr, 
                                   UINT32 uiCliPort);

LONG RDS_MSG_TerminalInfoDel(      CHAR*  pcTerminalID);
  
LONG RDS_MSG_TerminalInfoUpdate(    CHAR*  pcTerminalID);






