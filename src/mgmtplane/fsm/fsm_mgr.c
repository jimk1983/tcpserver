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


#include "common.h"
#include "fsm/fsm_pub.h"

PFSM_MGR_CTX_S  g_pstFsmMgrCtx = NULL;

/*****************************************************************************
 函 数 名  : FSM_MgrFileSpliceCreate
 功能描述  : 文件片的创建
 输入参数  : FSM_FILE_INFO_S *pstFileInfo ---创建文件片 
             CHAR *pcFileContent        ---文件内容   
             UINT32 uiFileLen           ---文件长度
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月13日
    作    者   : 蒋康
    修改内容   : 新生成函数

备注： 考虑大文件，还是以块的形式来进行拷贝,通过数组快速索引

*****************************************************************************/
LONG FSM_MgrFileSpliceCreate(FSM_FILE_INFO_S *pstFileInfo)
{
    UINT32 uiIndex      = 0;
    UINT32 uiOffSet     = 0;
    UINT32 uiLeftSize   = 0;
    UINT32 uiFileSize   = 0;
    LONG   lFileSpliceNums = 0;
    COM_IOBUF_S *pstIoBuf  = NULL;
    MD5_CTX      stMd5Ctx  = {0};
    UCHAR        aucDigest[MD5_SIZE] = {0};
    
    /*不存在修改文件的情况，直接就是读取文件，所以不再添加互斥锁*/
    if ( NULL == pstFileInfo
        || NULL == pstFileInfo->pucFileContent 
        || pstFileInfo->stFileInfo.stFileResInfo.uiFileSize >= FSM_MAX_FILESIZE )
    {
        return VOS_ERR;
    }

    uiFileSize = pstFileInfo->stFileInfo.stFileResInfo.uiFileSize;

    lFileSpliceNums = uiFileSize / FSM_MAX_SPLICESIZE + 1;

    /*创建文件片*/
    for( uiIndex = 0; uiIndex < lFileSpliceNums; uiIndex++)
    {
        pstIoBuf = COM_Iobuf_Malloc(0);
        if ( NULL == pstIoBuf )
        {
            VOS_Printf("Common iobuf malloc error!");
            return VOS_ERR;
        }
        
        uiLeftSize = uiFileSize - uiOffSet;
        
        /*是否超过切片大小，并保存真实的长度*/
        if ( uiLeftSize > FSM_MAX_SPLICESIZE )
        {
            COM_IOBUF_SETINPUTED_LEN(pstIoBuf, FSM_MAX_SPLICESIZE);
            VOS_Mem_Copy_S(pstIoBuf->pcData, pstIoBuf->ulMaxLen, (CHAR *)(pstFileInfo->pucFileContent + uiOffSet), FSM_MAX_SPLICESIZE);
        }
        else
        {
            COM_IOBUF_SETINPUTED_LEN(pstIoBuf, uiLeftSize);
            VOS_Mem_Copy_S(pstIoBuf->pcData, pstIoBuf->ulMaxLen, (CHAR *)(pstFileInfo->pucFileContent + uiOffSet), uiLeftSize);
        }
        
        uiOffSet += pstIoBuf->ulDataLen;
        
        pstFileInfo->starryChunkIoBuf[uiIndex].pstIoBuf = pstIoBuf;

        MD5Init(&stMd5Ctx);
        MD5Update(&stMd5Ctx, (UCHAR *)pstIoBuf->pcData, uiFileSize);
        MD5Final(aucDigest,&stMd5Ctx);
        MD5_ValToString_s(aucDigest, FSM_VAL_LEN, (UCHAR *)pstFileInfo->starryChunkIoBuf[uiIndex].acChunkCRCVal);
        pstFileInfo->uiIoBufNums++;
        if ( uiOffSet == uiFileSize )
        {
            break;
        }
    }

    return lFileSpliceNums;
}


/*****************************************************************************
 函 数 名  : FSM_MgrFileSpliceRelease
 输入参数  : FSM_FILE_INFO_S *pstFileInfo  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月13日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID FSM_MgrFileSpliceRelease(FSM_FILE_INFO_S *pstFileInfo)
{   
    UINT32          uiIndex = 0;
    
    if ( NULL == pstFileInfo )
    {
        return;
    }
    
    for(uiIndex=0; uiIndex < FSM_MAX_FILENUMS; uiIndex++)
    {
        if ( NULL != pstFileInfo->starryChunkIoBuf[uiIndex].pstIoBuf )
        {
            COM_Iobuf_Free(pstFileInfo->starryChunkIoBuf[uiIndex].pstIoBuf);
            pstFileInfo->starryChunkIoBuf[uiIndex].pstIoBuf = NULL;
        }
        else
        {
            /*按顺序存放，直接可以跳出*/
            break;
        }
    }
    return;
}

/*****************************************************************************
 函 数 名: FSM_MgrFileInfoCreate
 功能描述  : 创建信息
 输入参数  : CHAR *pcFileDir  
           CHAR *pcFile     
 输出参数  : 无
 返 回 值: FSM_FILE_INFO_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期: 2018年11月9日
    作    者: 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
FSM_FILE_INFO_S *FSM_MgrFileInfoCreate(CHAR *pcFileDir, UINT32 uiFileType)
{
    FSM_FILE_INFO_S*    pstFileInfo         = NULL;
    UINT32              uiFileSize          = 0; 
    MD5_CTX             stMd5Ctx            = {0};
    UCHAR               aucDigest[MD5_SIZE] = {0};
    CHAR                acFileName[VOS_DIRSNAME_LEN]={0};
    
    pstFileInfo = (PFSM_FILE_INFO_S)malloc(sizeof(FSM_FILE_INFO_S));
    if ( NULL ==  pstFileInfo )
    {
        return NULL;
    }

    VOS_Mem_Zero(pstFileInfo, sizeof(FSM_FILE_INFO_S));

    VOS_RWLOCK_INIT(pstFileInfo->stLock);

    VOS_StrCat(pstFileInfo->stFileInfo.acFullName, pcFileDir);

    VOS_StrCat(pstFileInfo->stFileInfo.acFullName, "/");
    
    if( VOS_ERR == FSM_Conf_GetFileName(acFileName ,VOS_DIRSNAME_LEN, (FSM_CONF_FILE_E)uiFileType))
    {
        free(pstFileInfo);
        return NULL;
    }
    
    VOS_StrCat(pstFileInfo->stFileInfo.acFullName, acFileName);

    /*版本信息*/
    pstFileInfo->stFileInfo.stFileResInfo.uiFileVersion     = FSM_APP_VERSION;
    pstFileInfo->stFileInfo.stFileResInfo.uiFileSize        = 0;
    pstFileInfo->stFileInfo.stFileResInfo.uiFileCRCAlgm     = 0;
    VOS_Mem_Zero(pstFileInfo->stFileInfo.stFileResInfo.acFileCRCVal, FSM_VAL_LEN);

    /*直接文件大小和内容获取*/
    if ( VOS_ERR == VOS_FileRead(pstFileInfo->stFileInfo.acFullName, 
                                (INT32 *)&uiFileSize,
                                &pstFileInfo->pucFileContent) )
    {
        VOS_Printf("Get file size error, file=%s", pstFileInfo->stFileInfo.acFullName);
        free(pstFileInfo);
        return NULL;
    }
    
    MD5Init(&stMd5Ctx);
    MD5Update(&stMd5Ctx, pstFileInfo->pucFileContent, uiFileSize);
    MD5Final(aucDigest,&stMd5Ctx);

    pstFileInfo->stFileInfo.stFileResInfo.uiFileSize = uiFileSize;
    pstFileInfo->stFileInfo.stFileResInfo.uiFileType = uiFileType;
    
    if ( VOS_ERR == FSM_MgrFileSpliceCreate(pstFileInfo) )
    {
        free(pstFileInfo->pucFileContent);
        free(pstFileInfo);
        return NULL;
    }

    MD5_ValToString_s(aucDigest, FSM_VAL_LEN, (UCHAR *)pstFileInfo->stFileInfo.stFileResInfo.acFileCRCVal);
    
    return pstFileInfo;
}


/*****************************************************************************
 函 数 名  : FSM_MgrFileInfoRelease
 功能描述  : 直接释放
 输入参数  : PFSM_FILE_INFO_S pstFileInfo  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID    FSM_MgrFileInfoRelease(PFSM_FILE_INFO_S pstFileInfo)
{
    if ( NULL != pstFileInfo )
    {
        if ( NULL == pstFileInfo->pucFileContent )
        {
            FSM_MgrFileSpliceRelease(pstFileInfo);
            free(pstFileInfo->pucFileContent);
            pstFileInfo->pucFileContent = NULL;
        }
        free(pstFileInfo);
    }
}


/*****************************************************************************
 函 数 名  : FSM_MgrFileGetInfoByIndex
 功能描述  : 直接获取配置，注意不能更改
 输入参数  : UINT32 uiIndex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
PFSM_FILE_INFO_S FSM_MgrFileGetInfoByIndex(UINT32 uiIndex)
{
    PFSM_FILE_INFO_S pstFileInfo = NULL;

    if ( NULL == g_pstFsmMgrCtx )
    {
        return NULL;
    }

    pstFileInfo = (PFSM_FILE_INFO_S)g_pstFsmMgrCtx->pstarryFileInfo[uiIndex];
    
    return pstFileInfo;
}


/*****************************************************************************
 函 数 名  : FSM_MgrInit
 功能描述  : 管理器初始化
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
LONG   FSM_MgrEnvInit()
{
    UINT32  uiCount = 0;
    
    g_pstFsmMgrCtx = (PFSM_MGR_CTX_S)malloc(sizeof(FSM_MGR_CTX_S));
    if ( NULL == g_pstFsmMgrCtx )
    {
        VOS_Printf("File small server manager malloc context failed!");
        return VOS_ERR;
    }

    VOS_Mem_Zero(g_pstFsmMgrCtx, sizeof(FSM_MGR_CTX_S));

    for(uiCount = 0; uiCount < FSM_MAX_NUMS; uiCount++)
    {
        g_pstFsmMgrCtx->pstarryFileInfo[uiCount] = NULL;
    }
    
    g_pstFsmMgrCtx->uiCurNums = 0;

    if( VOS_ERR == VOS_DirGetCurrentPath(g_pstFsmMgrCtx->acCurDir, VOS_DIRSNAME_LEN) )
    {
        free(g_pstFsmMgrCtx);
        g_pstFsmMgrCtx = NULL;
        return VOS_ERR;
    }
    else
    {
        VOS_StrCat(g_pstFsmMgrCtx->acCurDir, FSM_CONFIG_BINDIR);
    }

    g_pstFsmMgrCtx->uiCurNums = FSM_Conf_GetFileNums();
    if ( 0 == g_pstFsmMgrCtx->uiCurNums
        || g_pstFsmMgrCtx->uiCurNums >= FSM_MAX_NUMS )
    {   
        free(g_pstFsmMgrCtx);
        g_pstFsmMgrCtx = NULL;
        return VOS_ERR;
    }
        
    for (uiCount =0; uiCount<g_pstFsmMgrCtx->uiCurNums; uiCount++)
    {
        g_pstFsmMgrCtx->pstarryFileInfo[uiCount]  = FSM_MgrFileInfoCreate(g_pstFsmMgrCtx->acCurDir, uiCount);
        if ( NULL == g_pstFsmMgrCtx->pstarryFileInfo[uiCount] )
        {
            VOS_Printf("Fsm create new file info failed!\n");
            FSM_MgrEnvUnInit();
            return VOS_ERR;
        }
    }
    
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : FSM_MgrEnvUnInit
 功能描述  : 管理器去初始化
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
VOID    FSM_MgrEnvUnInit()
{
    INT32 iIndex = 0;
    
    if ( NULL != g_pstFsmMgrCtx )
    {
        for(iIndex = 0; iIndex < FSM_MAX_NUMS; iIndex++)
        {
            if ( NULL != g_pstFsmMgrCtx->pstarryFileInfo[iIndex] )
            {
                FSM_MgrFileInfoRelease(g_pstFsmMgrCtx->pstarryFileInfo[iIndex]);
                g_pstFsmMgrCtx->pstarryFileInfo[iIndex] = NULL;
            }
        }
        
        free(g_pstFsmMgrCtx);
        g_pstFsmMgrCtx = NULL;
    }
}


