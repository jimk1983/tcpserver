/******************************************************************************

                  版权所有 (C), 2018-2028, 有限公司

 ******************************************************************************
  文 件 名   : fsm_def.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月9日
  最近修改   :
  功能描述   : 文件相关定义
  函数列表   :
  修改历史   :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/


/*部分值的长度*/
#define FSM_VAL_LEN    36

#define FSM_CRCALGM_MD5     0
#define FSM_CRCALGM_SHA1    1


/*资源文件信息*/
typedef struct tagFsmFileResourceInfo
{
    UINT32 uiFileVersion;                   /*文件版本信息*/
    UINT32 uiFileSize;                      /*文件大小*/
    UINT32 uiFileCRCAlgm;                   /*文件校验算法类型*/
    CHAR   acFileCRCVal[FSM_VAL_LEN];       /*文件校验值*/
}FSM_RFILEINFO_S, *PFSM_RFILEINFO_S;


/*文件的资源*/
typedef struct tagFsmFileResourceEntry
{
    CHAR acFullName[VOS_DIRSNAME_LEN];      /*文件存放路径*/
    FSM_RFILEINFO_S    stFileResInfo;       /*文件信息*/
}FSM_RENTRY_S, *PFSM_RENTRY_S;



