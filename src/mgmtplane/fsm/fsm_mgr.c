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
 函 数 名  : FSM_MgrFileInfoCreate
 功能描述  : 创建信息
 输入参数  : CHAR *pcFileDir  
             CHAR *pcFile     
 输出参数  : 无
 返 回 值  : FSM_FILE_INFO_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
FSM_FILE_INFO_S *FSM_MgrFileInfoCreate(CHAR *pcFileDir,const CHAR *pcFileName)
{
    FSM_FILE_INFO_S *pstFileInfo = NULL;

    
    

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
        
        free(pstFileInfo);
    }
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
INT32   FSM_MgrEnvInit()
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
    if ( VOS_ERR == g_pstFsmMgrCtx->uiCurNums
        || g_pstFsmMgrCtx->uiCurNums >= FSM_MAX_NUMS )
    {   
        free(g_pstFsmMgrCtx);
        g_pstFsmMgrCtx = NULL;
        return VOS_ERR;
    }
        
    for (uiCount =0; uiCount<g_pstFsmMgrCtx->uiCurNums; uiCount++)
    {
        g_pstFsmMgrCtx->pstarryFileInfo[uiCount]  = FSM_MgrFileInfoCreate(g_pstFsmMgrCtx->acCurDir, FSM_Conf_GetFileNameByIndex(uiCount));
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


