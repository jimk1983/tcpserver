/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, ���޹�˾

 ******************************************************************************
  �� �� ��   : fsm_config.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��9��
  ����޸�   :
  ��������   : �ļ���ص����ù���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ļ�

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
 �� �� ��  : FSM_Conf_GetFileNums
 ��������  : ��ȡ��ǰ������
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
LONG   FSM_Conf_GetFileNums()
{
    LONG uiNums = VOS_ERR;

    uiNums = sizeof(g_starryFsmConfig)/sizeof(FSM_CONF_INFO_S);

    return uiNums;
}


/*****************************************************************************
 �� �� ��  : FSM_Conf_GetFileNameByIndex
 ��������  : ��ȡ��ǰ�������ļ�
 �������  : UINT32 iIndex  
 �������  : ��
 �� �� ֵ  : CHAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��: FSM_Conf_GetFileNameByIndex
 ��������  : �ļ���Ϣ
 �������  : UINT32 iIndex     
           CHAR *pcFileName  
 �������  : ��
 �� �� ֵ: 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : FSM_Conf_GetServerAddrInfo
 ��������  : ֱ�Ӿ����ַ���
 �������  :    CHAR *pcAddrInfo    ---��ַ��Ϣ
            INT32 iLen          ---����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : FSM_Conf_GetAppFileVersion
 ��������  : ��ȡAPP�汾
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
ULONG    FSM_Conf_GetAppFileVersion()
{
    ULONG   ulVersion = FSM_APP_VERSION;

    /*TODO: ���Ը��������·�*/
    return ulVersion;
}


/*****************************************************************************
 �� �� ��  : FSM_Conf_GetFileSize
 ��������  : �ļ���С��ȡ
 �������  : FSM_CONF_FILE_E eFileType  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��: FSM_Conf_GetChunkContentByIndex
 ��������  : ��ȡ�ļ�Chunk����
 �������  :    FSM_CONF_FILE_E eFileType   ---�ļ�����
            UINT32 uiChunkIndex         ---������
 �������  : ��
 �� �� ֵ: const
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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



