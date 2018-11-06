/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vgm_cfg.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月3日
  最近修改   :
  功能描述   : Vgateway的配置
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

/*升级业务的网关*/
#define VGM_DEFAULT_PORT    9888
#define VGM_DEFAULT_IPADDR  "0.0.0.0"

/*虚拟网关是全局变量，由于内容较大，因此针对内容进行加锁*/
/*目前先按照创建出一个虚拟网关进行*/
typedef struct tagVgmCtxLocks
{
    VOS_RW_LOCK_S  stLockListenConn;    /*全局获取监听节点信息的锁*/
}VGM_CTX_LOCK_S;


/*业务使能开关*/
typedef struct tagVgmBizEnable
{
    UINT32  uiNemEnable;
    UINT32  uiPfmEnable;
    UINT32  uiTcmEnable;
}VGM_CTX_BIZENABLE_S;

/*全局的虚拟网关的上下文信息*/
struct tagVgmCtx
{
    /*各个内容的对应的锁*/
    VGM_CTX_LOCK_S           stVTLocks;          
    /*该虚拟网关的ID*/
    ULONG                    ulVtID;                
    /*该虚拟网关的名称*/
    CHAR                     acVtName[VGM_VTNAME_MAXLEN];    
    /*监听的接口名称*/
    CHAR                     acIfName[VOS_IFNAME_MAX_LEN];  
    /*该虚拟网关的对外域名*/
    CHAR                     acVtHostUrl[VGM_HOSTNAME_MAXLEN];   
    /*该虚拟网关的IP地址: 主机序*/
    UINT32                   ulVTIP;          
    /*该虚拟网关的端口号*/
    UINT32                   ulVTPort;        
    /*该虚拟网关的UDPS IP地址*/
    UINT32                   ulUdpsIP;          
    /*该虚拟网关的UDPS 端口号*/
    UINT32                   ulUdpsPort;        
    /*业务负载均衡类型*/
    UINT32                   uiBizType;  
    /*虚拟网关的业务使能开关*/
    VGM_CTX_BIZENABLE_S      stBizEn;
    /*该虚拟网关的SSL配置*/
    //UTL_SSL_CTX_CONFIG_S     *pstSSLCfg;      
    /*当前监听的SSL的节点*/
    VGM_SSL_LISTEN_CONN_S    *pstSslListenConn;
    /*当前监听的UDPS的节点, 单独放置线程了，统一处理*/
    //VGM_UDPS_LISTEN_CONN_S   *pstUdpListenConn;
    /*该网关地址池的配置*/
    //VGM_NEMPOOL_CONFIG_S     stPoolConf[VGM_CTX_NEMPOOL_SIZE];
    /*内网接口索引,用于内网报文转发*/
    ULONG                    ulInnerIfIndex;
    /*网关最多可以创建出64个家庭*/
    //VGM_FMINFO_CTX_S        *pstFmInfo[VGM_CTX_FMINFO_SIZE];

};

/*所有创建的虚拟网关的配置*/
typedef struct tagVgmCfgCtx{

    /*虚拟网关配置保存的数组,可以保存VGM_CFG_ARRAY_NUMS个网关的配置信息*/
    VGM_CTX_S        *pastVgCfgArray[VGM_CFG_ARRAY_NUMS];   /*使用指针数组节省空间*/
    /*当前已经存在的虚拟网关数量*/
    ULONG             ulCurrentNums;
}VGM_CFG_CTX_S;

ULONG   VGM_CFG_GetFreeVTID();

VGM_CTX_S *VGM_CFG_GetVTCtxByVTID(ULONG ulVtId);

VOID    VGM_CFG_DelVTCtxByVTID(ULONG ulVtId);

LONG    VGM_CFG_SetVTCtxByVTID(VGM_CTX_S *pstVgCtx, ULONG ulVtId);

LONG    VGM_CFG_CtxInit();

VOID    VGM_CFG_CtxUnInit();






