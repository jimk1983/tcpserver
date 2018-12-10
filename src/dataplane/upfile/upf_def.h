/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : pfw_def.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2017年5月1日
  最近修改   :
  功能描述   : 端口转发的定义
  函数列表   :
  修改历史   :
  1.日    期   : 2017年5月1日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

/*SWM_BIZ_HEAD_S + UPF_HEAD_S + (TYPE) */


/*控制码类型*/
typedef  enum
{
    UPSER_CTLCODE_UNKNOW = 0,
    UPSER_CTLCODE_KPVINFO_REQ,    //版本信息请求(保活信息)
    UPSER_CTLCODE_KPVINFO_RESP,   //版本信息响应
    UPSER_CTLCODE_XMLFILE_GET,    //XML文件获取请求
    UPSER_CTLCODE_XMLFILE,        //XML文件内容
    UPSER_CTLCODE_APPFILE_GET,    //APP文件获取请求
    UPSER_CTLCODE_APPFILE,        //APP文件内容

    UPSER_CTLCODE_NUMS
}UPSER_CTLCODE_E;


/*设备标签长度*/
#define UPF_DEVLAB_LEN      64
#define UPF_FILELEN         32
#define UPF_FILEVALEN       36

/*使用16K的IOBUF传输,所以内容需要减去头部部分大小*/
#define UPF_CONTENTLEN      16000   

typedef enum
{
    UPDEV_STATUSCODE_STOP = 0,
    UPDEV_STATUSCODE_WORKING,

    UPDEV_STATUSCODE_NUMS
}UPDEV_STATUSCODE_E;


typedef enum
{
    UPDEV_CMDCODE_UNKNOW = 0,
    IPDEV_CMDCODE_UPDATA_AUTO,  /* 自动开机更新 */
    UPDEV_CMDCODE_UPDATA_FORCE, /* 强制更新 */
    UPDEV_CMDCODE_NUMS
}UPDEV_CMDCODE_E;
    
/*保活信息*/
typedef struct tagUPFKVPInfo
{
    CHAR        acDevLabel[UPF_DEVLAB_LEN];         /*设备标签*/
    CHAR        acDevDecptor[UPF_DEVLAB_LEN];       /*设备描述*/
    UINT32      uiDevStatusCode;                    /*设备状态码*/
    UINT32      uiTerminalStatusCode;               /*终端状态码*/
    UINT32      uiLastVersion;                      /*当前最新版本*/
    UINT32      uiIntervalRate;                     /*间隔时间*/
    UINT32      uiFileSize;                         /*XML文件大小*/
    CHAR        acAddrInfo[UPF_FILELEN];            /*服务器地址*/
    CHAR        acFileName[UPF_FILELEN];            /*文件名称*/
    CHAR        acFileValue[UPF_FILEVALEN];         /*文件MD5值*/
    USHORT      usChunkNums;                        /*块总数*/
    USHORT      usCmdCode;                          /*命令代码*/
    UINT32      uiConnNums;                         /*服务器连接数*/
}UPF_KVPINFO_S, *PUPF_KVPINFO_S;


typedef struct tagUpfFileDataInfoRequest
{
    UINT32      uiAPPVersion;                   /*当前版本号*/
    UINT32      uiFileSize;                     /*文件总大小*/
    CHAR        acFileName[UPF_FILELEN];        /*文件名称*/
    UINT32      uiChunkStart;                   /*块索引*/
    UINT32      uiChunkRange;                   /*块范围*/
    UINT32      uiChunkSize;                    /*块大小*/
}UPF_FILEDATA_REQ_S, *PUPF_FILEDATA_REQ_S;

typedef struct tagUpfFileDataInfoResponse
{
    UINT32      uiAPPVersion;                   /*当前版本号*/
    UINT32      uiFileSize;                     /*文件总大小*/
    CHAR        acFileName[UPF_FILELEN];        /*文件名称*/
    UINT32      uiChunkEncryptType;             /*块加密算法类型*/
    UINT32      uiChunkCRCType;                 /*块校验算法类型*/
    CHAR        acChunkValue[UPF_FILEVALEN];    /*块校验码*/
    UINT32      uiChunkNums;                    /*块总数*/
    UINT32      uiChunkIndex;                   /*块索引*/
    UINT32      uiChunkSize;                    /*块大小*/
    CHAR        acChunkContent[UPF_CONTENTLEN]; /*块内容*/
}UPF_FILEDATA_RESP_S, *PUPF_FILEDATA_RESP_S;

/*格式头*/
typedef struct tagUPFHead
{
    UINT32       uiCtrlCode;                    /*控制消息码*/
    UINT32       uiCtrlLength;                  /*数据长度*/
}UPF_HEAD_S, *PUPF_HEAD_S;


/*****************************************************************/
typedef struct tagUPFConn                UPF_CONN_S;

/*发送完成状态*/
#define UPF_TRNSTATUS_SNDEND        0
/*发送中状态*/
#define UPF_TRNSTATUS_SNDING        1

/*升级节点*/
struct tagUPFConn
{
    /*Nem管道节点*/
    SWM_PIPE_CONN_S             stPipe;

    /*该通道的BizChannel保存*/
    SWM_BIZ_CHANNEL_S*          pstBizChannel;

    /*网络事件*/
    RCT_NETOPT_EVENT_S          stNetEvtOps;

    /*老化事件*/
    RCT_EXPIROPT_EVENT_S        stExpireOps;

    COM_IOBUF_S *               pstRcvIobuf;
    
    /*当前要发送的文件信息*/
    FSM_RENTRY_S                stFileEntryInfo;    

    /*是否已经在Redis服务器上注册*/
    UINT32                      uiRdsRegted;
   
    /*当前传输的控制码*/
    UINT32                      uiUpfCtrlCode;
    /*文件片发送状态*/
    UINT32                      uiMgrChunkStatus;
    /*文件片请求的起始索引*/
    UINT32                      uiMgrChunkStartIndex;
    /*已经发送的数量*/
    UINT32                      uiMgrChunkCount;
    /*文件片总个数*/
    UINT32                      uiMgrChunkNums;
};


