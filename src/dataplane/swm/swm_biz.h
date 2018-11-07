/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_biz.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月4日
  最近修改   :
  功能描述   : bizchannel业务通道管理
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月4日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/


#define EMSDK_VERSION		0x10

typedef enum
{
	EMPTO_BIZTYPEID_UNKNOW = 0,
    EMPTO_BIZTYPEID_UPF,			/*文件升级业务请求*/
	EMPTO_BIZTYPEID_NEM,			/*网络扩展业务请求*/
	EMPTO_BIZTYPEID_PFM,			/*端口转发业务请求*/
	EMPTO_BIZTYPEID_TCM,			/*终端检查业务请求*/
	EMPTO_BIZTYPEID_LNM,			/*License控制请求: 提供复杂的授权控制*/

	EMPTO_BIZTYPEID_NUMS
}EMPTO_BIZTID_E;



/*该业务通道未绑定*/
#define SWM_BIZ_USER_UNBINDED     0
/*该业务通道已绑定*/
#define SWM_BIZ_USER_BINDED         1

/*本通道的客户端信息*/
typedef struct tagSWMBizClientInfo
{
    /*主机序地址*/
    UINT32   ulClientAddr;
    /*客户端端口*/
    UINT32   ulClientPort;

}SWM_BIZ_CLIENT_INFO_S;


/*本通道绑定的用户的所属信息*/
typedef struct tagSWMBizUserInfo
{
    UINT32   ulUserID;           /*用户的ID*/
    UINT32   ulSessionID;       /*用户的会话ID*/
    UINT32 	 uiFamilyID;		/*家庭成员信息*/
    
}SWM_BIZ_USER_INFO_S;

/*本通道所属的虚拟网关的信息*/
typedef struct tagSWMBizVtInfo
{
    ULONG           ulVTID;                 /*所在虚拟网关的ID*/
    CHAR            acGatewayUdpsAddr[VOS_SOCK_IPADDR_LEN]; 	/*网关的Udps加密报文转发地址 */
    UINT32          uiGatewayUdpsPort;	

}SWM_BIZ_VT_INFO_S;


/*SWM业务通道*/
struct tagSWMBizChannel 
{
    /*通道节点*/
    VOS_DLIST_NODE_S    stNode;

    /*管道的头节点*/
    VOS_DLIST_NODE_S    stPipeHead;
    /*管道节点数量*/
    ULONG   ulPipeNum;

    /*本Biz通道的业务类型*/
    ULONG   ulBizType;
    /*是否已经绑定*/
    ULONG   ulBindFlags;

    /*是否结束本通道*/
    ULONG   ulExitConfirm;
    
    /*本通道的客户端信息*/
    SWM_BIZ_CLIENT_INFO_S 	stClientInfo;
    /*本通道的用户信息*/
    //SWM_BIZ_USER_INFO_S    	stUserInfo;
    /*本通道的网关信息*/
    //SWM_BIZ_VT_INFO_S       stVtInfo;
    
    /*发送队列*/
    COM_IOBUF_QUE_S   *pstSwmSendQueue;
    /*ssl信息*/
    //SSL_S *pstSsl;
};


SWM_BIZ_CHANNEL_S *SWM_Biz_ChannelCreate(LONG lSockfd);

VOID SWM_Biz_ChannelRelease(SWM_BIZ_CHANNEL_S *pstBizChannel);

/*************************************************************************/

typedef struct tagSwmBizMatchCtx SWM_BIZ_MATCH_CTX_S;
/*回调函数的数据*/
struct tagSwmBizMatchCtx
{
    ULONG      ulBizType;         /*业务识别类型*/
    VOID        *pvFunc;            /*匹配函数*/
    VOID        *pvCtx;              /*每个业务的上下文*/
};

typedef LONG (*swm_biz_match_func)(SWM_BIZ_CHANNEL_S *pstBizChannel, CHAR *pcData, ULONG ulDataLen);

/*SWM 业务上下文*/
typedef struct tagSwmBizMatch
{
    SWM_BIZ_MATCH_CTX_S    stArrayBizTypeMatch[EMPTO_BIZTYPEID_NUMS];     /*业务通道的匹配*/
}SWM_BIZ_Match_S;


/*头部的偏移结构说明: 头部12个字节
*说明：
*		这个头部信息在登录等控制流程情况下，都是走SSL的
*		所以头部信息，都是不用加密的，部分字段都不用填写完整
*		但是这个头部在使用数据传输，或者是UDPS的情况下，可以对EMPTO_HEADINFO_S成员进行单字节加密等
*/

/*网络扩展控制报文: 一般采用TLV方式*/
typedef struct tagSwmBizHead
{
    UINT32      uiVersion;              //版本号
    UINT32      uiHeadSize;             //头部大小
	UINT32      uiMark1;                /*业务识别特征码A*/
	UINT32      uiMark2;                /*业务识别特征码B*/
    UINT32      uiDataLen;              /*后续的数据长度*/
}SWM_BIZ_HEAD_S, *PSWM_BIZ_HEAD_S;


/*通用的协议格式*/
#define BIZ_HEAD_TYPE_CTRL     0x0001        /*控制协议*/
#define BIZ_HEAD_TYPE_DATA     0x0002        /*数据协议*/


/*定义长度*/
#define SWM_BIZ_HEAD_LEN        sizeof(SWM_BIZ_HEAD_S)

LONG SWM_Biz_ChannelMatchInit();

VOID SWM_Biz_ChannelMatchUnInit();

LONG SWM_Biz_ChannelMatch(SWM_BIZ_CHANNEL_S *pstBizChannel, CHAR *pcData, ULONG ulDataLen);

LONG SWM_Biz_ChannelMatchRegister(SWM_BIZ_MATCH_CTX_S *pstBizMatch);

LONG SWM_Biz_ChannelMatchUnRegister(ULONG ulBizType);

LONG SWM_Biz_ChannelCheckLen(CHAR *pcPack, UINT32 uiPackLen);

LONG SWM_Biz_ChannelPreGetPackLen(CHAR *pcPack);


