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


/*�̶��İ汾*/
#define FSM_APP_VERSION  0x01000004  //1.0.0.1  


/*���ֲ�Ҫ̫��*/
#define FSM_FNAME_LEN    64

/*��Ӧ���ļ�����*/
typedef enum
{
    FSM_CONF_FILE_XML = 0,  
    FSM_CONF_FILE_APP,
    

    FSM_CONF_FILE_NUMS
}FSM_CONF_FILE_E;


typedef struct tagFsmFileConfigInfo
{
    FSM_CONF_FILE_E     eFileIndexType;                 /*��������*/
    const   CHAR        acFileName[FSM_FNAME_LEN];      /*�ļ�����*/
}FSM_CONF_INFO_S, *PFSM_CONF_INFO_S;



ULONG    FSM_Conf_GetFileNums();

LONG    FSM_Conf_GetServerAddrInfo(CHAR *pcAddrInfo, INT32 iLen);

LONG    FSM_Conf_GetFileName(CHAR *pcFileName, UINT32 uiLen, FSM_CONF_FILE_E eFileType);

ULONG   FSM_Conf_GetAppFileVersion();

const PFSM_FILE_INFO_S FSM_Conf_GetFileInfo(FSM_CONF_FILE_E eFileType);

const CHAR   *FSM_Conf_GetFileNameByIndex(UINT32 iIndex);

const FSM_CHUNK_IOBUF_S *FSM_Conf_GetChunkIobufByIndex(FSM_CONF_FILE_E eFileType, UINT32 uiChunkIndex);



