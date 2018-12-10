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
#define FSM_APP_VERSION  0x01000004  //1.0.0.1  


/*名字不要太长*/
#define FSM_FNAME_LEN    64

/*对应的文件索引*/
typedef enum
{
    FSM_CONF_FILE_XML = 0,  
    FSM_CONF_FILE_APP,
    

    FSM_CONF_FILE_NUMS
}FSM_CONF_FILE_E;


typedef struct tagFsmFileConfigInfo
{
    FSM_CONF_FILE_E     eFileIndexType;                 /*快速索引*/
    const   CHAR        acFileName[FSM_FNAME_LEN];      /*文件名称*/
}FSM_CONF_INFO_S, *PFSM_CONF_INFO_S;



ULONG    FSM_Conf_GetFileNums();

LONG    FSM_Conf_GetServerAddrInfo(CHAR *pcAddrInfo, INT32 iLen);

LONG    FSM_Conf_GetFileName(CHAR *pcFileName, UINT32 uiLen, FSM_CONF_FILE_E eFileType);

ULONG   FSM_Conf_GetAppFileVersion();

const PFSM_FILE_INFO_S FSM_Conf_GetFileInfo(FSM_CONF_FILE_E eFileType);

const CHAR   *FSM_Conf_GetFileNameByIndex(UINT32 iIndex);

const FSM_CHUNK_IOBUF_S *FSM_Conf_GetChunkIobufByIndex(FSM_CONF_FILE_E eFileType, UINT32 uiChunkIndex);



