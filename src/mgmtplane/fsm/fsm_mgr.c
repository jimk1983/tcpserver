/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, ���޹�˾

 ******************************************************************************
  �� �� ��   : fsm_mgr.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��9��
  ����޸�   :
  ��������   : �ļ�������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/


#include "common.h"
#include "fsm/fsm_pub.h"

PFSM_MGR_CTX_S  g_pstFsmMgrCtx = NULL;


/*****************************************************************************
 �� �� ��  : FSM_MgrFileInfoCreate
 ��������  : ������Ϣ
 �������  : CHAR *pcFileDir  
             CHAR *pcFile     
 �������  : ��
 �� �� ֵ  : FSM_FILE_INFO_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
FSM_FILE_INFO_S *FSM_MgrFileInfoCreate(CHAR *pcFileDir, const CHAR *pcFileName)
{
    FSM_FILE_INFO_S*    pstFileInfo     = NULL;
    UCHAR*              pucFileContent  = NULL;
    UINT32              uiFileSize      = 0; 
    MD5_CTX             stMd5Ctx            = {0};
    UCHAR               aucDigest[MD5_SIZE] = {0};
    
    pstFileInfo = (PFSM_FILE_INFO_S)malloc(sizeof(FSM_FILE_INFO_S));
    if ( NULL ==  pstFileInfo )
    {
        return NULL;
    }

    VOS_Mem_Zero(pstFileInfo, sizeof(FSM_FILE_INFO_S));

    VOS_RWLOCK_INIT(pstFileInfo->stLock);
    VOS_DLIST_INIT(&pstFileInfo->stFileList);

    VOS_StrCat(pstFileInfo->stFileInfo.acFullName, pcFileDir);
    VOS_StrCat(pstFileInfo->stFileInfo.acFullName, pcFileName);

    /*�汾��Ϣ*/
    pstFileInfo->stFileInfo.stFileResInfo.uiFileVersion     = FSM_APP_VERSION;
    pstFileInfo->stFileInfo.stFileResInfo.uiFileSize        = 0;
    pstFileInfo->stFileInfo.stFileResInfo.uiFileCRCAlgm     = 0;
    VOS_Mem_Zero(pstFileInfo->stFileInfo.stFileResInfo.acFileCRCVal,FSM_VAL_LEN);

    /*ֱ���ļ���С�����ݻ�ȡ*/
    if(VOS_ERR == VOS_FileRead(pstFileInfo->stFileInfo.acFullName, 
                                (INT32 *)&uiFileSize,
                                &pucFileContent))
    {
        VOS_Printf("Get file size error, file=%s", pstFileInfo->stFileInfo.acFullName);
        free(pstFileInfo);
        return NULL;
    }
    
    MD5Init(&stMd5Ctx);
    MD5Update(&stMd5Ctx, pucFileContent, uiFileSize);
    MD5Final(aucDigest,&stMd5Ctx);

    free(pucFileContent);
    pstFileInfo->stFileInfo.stFileResInfo.uiFileSize = uiFileSize;

    MD5_ValToString_s(aucDigest, FSM_VAL_LEN, (UCHAR *)pstFileInfo->stFileInfo.stFileResInfo.acFileCRCVal);
    
    return pstFileInfo;
}


/*****************************************************************************
 �� �� ��  : FSM_MgrFileInfoRelease
 ��������  : ֱ���ͷ�
 �������  : PFSM_FILE_INFO_S pstFileInfo  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID    FSM_MgrFileInfoRelease(PFSM_FILE_INFO_S pstFileInfo)
{
    if ( NULL != pstFileInfo )
    {
        
        free(pstFileInfo);
    }
}


/*****************************************************************************
 �� �� ��  : FSM_MgrInit
 ��������  : ��������ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : FSM_MgrEnvUnInit
 ��������  : ������ȥ��ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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


