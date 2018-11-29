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
 �� �� ��  : FSM_MgrFileSpliceCreate
 ��������  : �ļ�Ƭ�Ĵ���
 �������  : FSM_FILE_INFO_S *pstFileInfo ---�����ļ�Ƭ 
             CHAR *pcFileContent        ---�ļ�����   
             UINT32 uiFileLen           ---�ļ�����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��13��
    ��    ��   : ����
    �޸�����   : �����ɺ���

��ע�� ���Ǵ��ļ��������Կ����ʽ�����п���,ͨ�������������

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
    
    /*�������޸��ļ��������ֱ�Ӿ��Ƕ�ȡ�ļ������Բ�����ӻ�����*/
    if ( NULL == pstFileInfo
        || NULL == pstFileInfo->pucFileContent 
        || pstFileInfo->stFileInfo.stFileResInfo.uiFileSize >= FSM_MAX_FILESIZE )
    {
        return VOS_ERR;
    }

    uiFileSize = pstFileInfo->stFileInfo.stFileResInfo.uiFileSize;

    lFileSpliceNums = uiFileSize / FSM_MAX_SPLICESIZE + 1;

    /*�����ļ�Ƭ*/
    for( uiIndex = 0; uiIndex < lFileSpliceNums; uiIndex++)
    {
        pstIoBuf = COM_Iobuf_Malloc(0);
        if ( NULL == pstIoBuf )
        {
            VOS_Printf("Common iobuf malloc error!");
            return VOS_ERR;
        }
        
        uiLeftSize = uiFileSize - uiOffSet;
        
        /*�Ƿ񳬹���Ƭ��С����������ʵ�ĳ���*/
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
 �� �� ��  : FSM_MgrFileSpliceRelease
 �������  : FSM_FILE_INFO_S *pstFileInfo  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��13��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
            /*��˳���ţ�ֱ�ӿ�������*/
            break;
        }
    }
    return;
}

/*****************************************************************************
 �� �� ��: FSM_MgrFileInfoCreate
 ��������  : ������Ϣ
 �������  : CHAR *pcFileDir  
           CHAR *pcFile     
 �������  : ��
 �� �� ֵ: FSM_FILE_INFO_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��: 2018��11��9��
    ��    ��: ����
    �޸�����   : �����ɺ���

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

    /*�汾��Ϣ*/
    pstFileInfo->stFileInfo.stFileResInfo.uiFileVersion     = FSM_APP_VERSION;
    pstFileInfo->stFileInfo.stFileResInfo.uiFileSize        = 0;
    pstFileInfo->stFileInfo.stFileResInfo.uiFileCRCAlgm     = 0;
    VOS_Mem_Zero(pstFileInfo->stFileInfo.stFileResInfo.acFileCRCVal, FSM_VAL_LEN);

    /*ֱ���ļ���С�����ݻ�ȡ*/
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
 �� �� ��  : FSM_MgrFileGetInfoByIndex
 ��������  : ֱ�ӻ�ȡ���ã�ע�ⲻ�ܸ���
 �������  : UINT32 uiIndex  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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


