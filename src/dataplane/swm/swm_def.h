/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : swm_def.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月6日
  最近修改   :
  功能描述   : 交叉头文件定义
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月6日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

/*ssl握手的状态*/
typedef enum tagSwmTslStatus
{
    SWM_TLS_SSL_STATUS_INIT = 0,            /*初始化状态*/
    SWM_TLS_SSL_STATUS_ACCEPTING,           /*握手中*/
    SWM_TLS_SSL_STATUS_ACCEPTED,            /*握手完成*/

    SWM_TLS_SSL_STATUS_NUMS
}SWM_TLS_SSL_STATUS_E;

/*控制管道的码*/
typedef enum tagSwmCtrlCmdCode
{
    SWM_CTRLCMD_UNKNOW = 0,
    SWM_CTRLCMD_SNDOUT_COMPELETED,   /*发送完成，队列已经为空*/

    SWM_CTRLCMD_NUMS
}SWM_CTRL_CMDCODE_E;


/*SWM业务TLS节点*/
typedef struct tagSwmTlsConn            SWM_TLS_CONN_S;

/*SWM业务通道*/
typedef struct tagSWMBizChannel         SWM_BIZ_CHANNEL_S;


