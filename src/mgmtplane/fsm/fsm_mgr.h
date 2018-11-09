/******************************************************************************

                  版权所有 (C), 2018-2028, 有限公司

 ******************************************************************************
  文 件 名   : fsm_mgr.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月9日
  最近修改   :
  功能描述   : 文件管理器
  函数列表   :
  修改历史   :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
    
    
/*存放文件的路径名称*/
#define  FSM_CONFIG_BINDIR  "bins"

/*文件配置的数量不会很多，如果很多需要考虑Hash算法来获取*/
#define  FSM_MAX_NUMS   64

/*单个文件信息*/
typedef struct tagFsmFileEntryInfo
{
    VOS_RW_LOCK_S    stLock;        /*文件片锁*/
    VOS_DLIST_NODE_S stFileList;    /*文件链*/   
    FSM_RENTRY_S     stFileInfo;    /*文件信息*/
}FSM_FILE_INFO_S, *PFSM_FILE_INFO_S;

/*所有文件信息*/
typedef struct tagFsmMgrCtx
{
    CHAR                acCurDir[VOS_DIRSNAME_LEN];    /*当前的路径*/
    PFSM_FILE_INFO_S    pstarryFileInfo[FSM_MAX_NUMS]; /*最多的文件个数，遍历不能超过64，效率降低*/
    UINT32              uiCurNums;                     /*当前文件的个数*/
}FSM_MGR_CTX_S, *PFSM_MGR_CTX_S;

INT32   FSM_MgrEnvInit();

VOID    FSM_MgrEnvUnInit();





