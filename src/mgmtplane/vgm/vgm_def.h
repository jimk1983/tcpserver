/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vgm_def.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月3日
  最近修改   :
  功能描述   : 有些前后有交叉依赖关系的定义，以及define
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
    
/*可以存放8个虚拟网关的信息*/
#define VGM_CFG_ARRAY_NUMS      8

/*默认监听listen的队列数量*/
#define VGM_SSLLISTEN_DEFAULT_NUMS      20

/*支持的地址池最大个数*/
#define VGM_CTX_NEMPOOL_SIZE        8

/*支持的家庭信息最大个数*/
#define VGM_CTX_FMINFO_SIZE        64


/*不开启客户端的证书校验*/
#define VGM_VTCLIENT_CERTVERIFY_DISABILE    0
/*开启客户端的证书校验*/
#define VGM_VTCLIENT_CERTVERIFY_ENABLE      1

#define VGM_HOSTNAME_MAXLEN           16

/*虚拟通道长度*/
#define VGM_VTNAME_MAXLEN      16

/*UDPS的接收回调*/
#define VDM_UDPS_RECVBUF_SIZE   72000

#define VDM_UDPPACK_MAXSIZE     65530

typedef struct tagVgmSSLListenConn VGM_SSL_LISTEN_CONN_S;

typedef struct tagVgmCtx VGM_CTX_S;

