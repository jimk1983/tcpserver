/******************************************************************************

                  版权所有 (C), 2017-2020, 网络有限公司

 ******************************************************************************
  文 件 名   : rds_ctx.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2018年8月17日
  最近修改   :
  功能描述   : redis的上下文接口
  函数列表   :
  修改历史   :
  1.日    期   : 2018年8月17日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

/*定义RDS数组的最大数量*/
#define RDS_ARRY_NUMS   8

/*默认的Redis的服务器的端口*/
#define RDS_SERVER_PORT     6379

typedef enum
{
    RDS_LBALGM_TYPE_LEASTCONN = 0,  /*最小连接数*/
    
    RDS_LBALGM_TYPE_NUMS
}RDS_LBALGM_TYPE_E;

/*RDS的连接信息*/
typedef struct tagRdsConnInfo
{
    CHAR            acSevAddr[VOS_IPV4ADDR_STRLEN]; /*redis的服务器地址*/
    int             iSevPort;                       /*redis的服务器端口*/
    REDIS_CONN_S*   pstRdsConn;                     /*redis的连接句柄*/  
}RDS_CONN_INFO_S, *PRDS_CONN_INFO_S;

/*RDS的上下文信息*/
typedef struct tagRdsCtx
{
    RDS_CONN_INFO_S     stArryRDSConn[RDS_ARRY_NUMS];   /*RDS数组*/
    ULONG               ulNums;                         /*当前数量*/
    RDS_LBALGM_TYPE_E   eRdsLBAlgmType;                 /*负载均衡算法*/
}RDS_CTX_S, *PRDS_CTX_S;

LONG RDS_CtxInit();

VOID RDS_CtxUnInit();



