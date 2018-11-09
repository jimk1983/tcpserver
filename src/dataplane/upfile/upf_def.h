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

/*控制码类型*/
typedef  enum
{
    UPSER_CTLCODE_UNKNOW=0,
    UPSER_CTLCODE_KPVINFO_REQ,      //---版本信息请求(保活信息)
    UPSER_CTLCODE_KPVINFO_RESP,  //---版本信息响应
    UPSER_CTLCODE_XMLFILE_GET,    //XML文件获取请求
    UPSER_CTLCODE_XMLFILE,        //XML文件内容
    UPSER_CTLCODE_APPFILE_GET,    //APP文件获取请求
    UPSER_CTLCODE_APPFILE,        //APP文件内容
    
    UPSER_CTLCODE_NUMS
}UPSER_CTLCODE_E;


/*设备标签长度*/
#define UPF_DEVLAB_LEN      32

#define UPF_FILELEN         32
#define UPF_FILEVALEN       32
/*保活信息*/
typedef struct tagUpfKVPInfo
{
	UINT32      uiLastVersion;				    /*当前最新版本*/
	UINT32      uiIntervalRate;				    /*间隔时间*/
    CHAR 		acFileAddress[UPF_FILELEN];		/*XML文件地址：192.168.1.1：8080/aaa.xml*/
    UINT32  	uiFileSize;			            /*XML文件大小*/
    CHAR 		acFileValue [UPF_FILEVALEN];	/*XML的MD5值*/
}UPF_KVPINFO_S, *PUPF_KVPINFO_S;

#define UPF_FTRNKID_BEGIN       "--BEGIN--"
#define UPF_FTRNKID_TRANING     "--TNFER--"
#define UPF_FTRNKID_END         "--END--"

/*使用16K的IOBUF传输,所以内容需要减去头部部分大小*/
#define UPF_CONTENTLEN      16000   

typedef struct tagUpfFileDataInfo
{
    UINT32 		uiAPPVersion;			        /*当前版本号*/
    CHAR 		acFileAddress[UPF_FILELEN];		/*XML文件地址：192.168.1.1：8080/aaa.xml*/
    UINT32  	uiFileSize;			            /*XML文件大小*/
    CHAR 		acFileValue [UPF_FILEVALEN];	/*XML的MD5值*/
    CHAR 		acTunkID[UPF_FILEVALEN];	    /*---Begin---/---transfer---/---END---*/
    UINT32 		uiTunkLength;			        /*单片文件长度*/
    CHAR 		acFileContent[UPF_CONTENTLEN];	/*文件内容*/
}UPF_FILEDATA_S, *PUPF_FILEDATA_S;


/*格式头*/
typedef struct tagUPFHead
{
    UINT32          uiCtrlCode;             /*控制消息码*/
    UINT32          uiCtrlLength;           /*数据长度*/
    CHAR	        acDeviceLab[UPF_DEVLAB_LEN];	/*设备标签*/
}UPF_HEAD_S, *PUPF_HEAD_S;

/*****************************************************************/
typedef struct tagUPFConn                UPF_CONN_S;

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

    /*文件片: 目前仅支持小文件，16K*n
      大文件不能采用该做法，会占内存太高, 可以考虑分多块获取*/
    VOS_DLIST_NODE_S            stFileSlice;
};


