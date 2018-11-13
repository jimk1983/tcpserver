/******************************************************************************

                  版权所有 (C), 2018-2028, 有限公司

 ******************************************************************************
  文 件 名   : fsm_config.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月9日
  最近修改   :
  功能描述   : 文件相关的配置管理
  函数列表   :
  修改历史   :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/


/*固定的版本*/
#define FSM_APP_VERSION  0x10000001  //1.0.0.1  





/*名字不要太长*/
#define FSM_FNAME_LEN    64

typedef struct tagFsmFileConfigInfo
{
    const CHAR acFileName[FSM_FNAME_LEN];
}FSM_CONF_INFO_S, *PFSM_CONF_INFO_S;



INT32   FSM_Conf_GetFileNums();

const CHAR   *FSM_Conf_GetFileNameByIndex(UINT32 iIndex);


