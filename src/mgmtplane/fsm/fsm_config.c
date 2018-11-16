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
    
#include "common.h"
#include "fsm/fsm_pub.h"

#define FSM_CONFIG_SERVERADDR   "update.ujon-tech01.com:9888"

const FSM_CONF_INFO_S g_starryFsmConfig[]=
{
    {
        FSM_CONF_FILE_XML,
        "/UpgrateInfo.xml"
    },
    {
        FSM_CONF_FILE_APP,
        "/TerminalCollector.exe"
    },
};


/*****************************************************************************
 函 数 名  : FSM_Conf_GetFileNums
 功能描述  : 获取当前的数量
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG   FSM_Conf_GetFileNums()
{
    LONG uiNums = VOS_ERR;

    uiNums = sizeof(g_starryFsmConfig)/sizeof(FSM_CONF_INFO_S);

    return uiNums;
}


/*****************************************************************************
 函 数 名  : FSM_Conf_GetFileNameByIndex
 功能描述  : 获取当前的配置文件
 输入参数  : UINT32 iIndex  
 输出参数  : 无
 返 回 值  : CHAR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
const CHAR   *FSM_Conf_GetFileNameByIndex(UINT32 iIndex)
{
    const CHAR *pcFileName = NULL;
    
    if ( iIndex >= FSM_Conf_GetFileNums() )
    {
        return NULL;
    }

    pcFileName = g_starryFsmConfig[iIndex].acFileName;
       
    return pcFileName;
}


/*****************************************************************************
 函 数 名: FSM_Conf_GetFileNameByIndex
 功能描述  : 文件信息
 输入参数  : UINT32 iIndex     
           CHAR *pcFileName  
 输出参数  : 无
 返 回 值: 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG FSM_Conf_GetFileName(CHAR *pcFileName, UINT32 uiLen, FSM_CONF_FILE_E eFileType)
{
    if ( eFileType >= FSM_CONF_FILE_NUMS )
    {
        return VOS_ERR;
    }

    VOS_StrCpy_S(pcFileName, uiLen, (CHAR *)g_starryFsmConfig[eFileType].acFileName);
       
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : FSM_Conf_GetServerAddrInfo
 功能描述  : 直接就是字符串
 输入参数  :    CHAR *pcAddrInfo    ---地址信息
            INT32 iLen          ---长度
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG  FSM_Conf_GetServerAddrInfo(CHAR *pcAddrInfo, INT32 iLen)
{
    if ( NULL == pcAddrInfo )    
    {
        return VOS_ERR;
    }
    
    VOS_StrCpy_S(pcAddrInfo, iLen, (CHAR *)FSM_CONFIG_SERVERADDR);
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : FSM_Conf_GetAppFileVersion
 功能描述  : 获取APP版本
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
ULONG    FSM_Conf_GetAppFileVersion()
{
    ULONG   ulVersion = FSM_APP_VERSION;

    /*TODO: 可以根据配置下发*/
    return ulVersion;
}


/*****************************************************************************
 函 数 名  : FSM_Conf_GetFileSize
 功能描述  : 文件大小获取
 输入参数  : FSM_CONF_FILE_E eFileType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
const PFSM_FILE_INFO_S FSM_Conf_GetFileInfo(FSM_CONF_FILE_E eFileType)
{
    PFSM_FILE_INFO_S    pstFileInfo  = NULL;

    if ( eFileType > FSM_CONF_FILE_NUMS )
    {
        return NULL;
    }
    
    pstFileInfo = FSM_MgrFileGetInfoByIndex((UINT32)eFileType);
    if ( NULL == pstFileInfo )
    {
        return NULL;
    }

    return pstFileInfo;
}


/*****************************************************************************
 函 数 名: FSM_Conf_GetChunkContentByIndex
 功能描述  : 获取文件Chunk内容
 输入参数  :    FSM_CONF_FILE_E eFileType   ---文件类型
            UINT32 uiChunkIndex         ---块索引
 输出参数  : 无
 返 回 值: const
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
const FSM_CHUNK_IOBUF_S *FSM_Conf_GetChunkIobufByIndex(FSM_CONF_FILE_E eFileType, UINT32 uiChunkIndex)
{
    PFSM_CHUNK_IOBUF_S  pstChunkBuf = NULL;
    PFSM_FILE_INFO_S    pstFileInfo  = NULL;

    if ( uiChunkIndex >= FSM_MAX_FILENUMS )
    {
        return NULL;   
    }
    
    pstFileInfo = FSM_MgrFileGetInfoByIndex(eFileType);
    if ( NULL == pstFileInfo )
    {
        return NULL;
    }

    pstChunkBuf = &pstFileInfo->starryChunkIoBuf[uiChunkIndex];
    
    return pstChunkBuf;
}



