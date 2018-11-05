/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_file.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��10��22��
  ����޸�   :
  ��������   : �ļ���������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��10��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/


/*�����ļ�������*/
#define VOS_FILENAME_LEN        256
#define VOS_DIRSNAME_LEN        256

typedef struct tagVosDirsFilesInfo VOS_DIRS_FILESINFO_S;

/*��ȡĿ¼�������ļ���Ϣ*/
struct tagVosDirsFilesInfo{
    VOS_DIRS_FILESINFO_S *next;    

    CHAR    acFileName[VOS_FILENAME_LEN + 1];          /*�ļ�����*/
    INT32   uiFileSize;                                                  /*�ļ���С*/
};

LONG VOS_DirIsExist(const CHAR *pcDirPath);

LONG VOS_DirsCreate(CHAR *pcDirPath);

LONG VOS_DirsCurrentRemove(CHAR *pcDir);

LONG VOS_DirsRemove(CHAR *pcDirPath);

VOS_DIRS_FILESINFO_S *VOS_DirsGetFilesInfo(CHAR *pcDir);

VOID VOS_DirsFileInfoFree(VOS_DIRS_FILESINFO_S *pstDirFileInfo);

LONG VOS_FileRemove(CHAR *pcFilePath);

LONG VOS_FileCreate(CHAR *pcFilePath);

LONG VOS_FileIsExist(const CHAR *pcFilePath);


LONG VOS_EventfdCreate(LONG lFlags);

VOID VOS_EventfdClose(LONG lEventFd);

LONG VOS_EventfdRead(LONG lEventFd, VOID *pcBuf, UINT32 uiSize);

LONG VOS_EventfdWrite(LONG lEventFd, VOID *pcBuf, UINT32 uiSize);


