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
    
    
/*����ļ���·������*/
#define  FSM_CONFIG_BINDIR  "bins"

/*�ļ����õ���������ܶ࣬����ܶ���Ҫ����Hash�㷨����ȡ*/
#define  FSM_MAX_NUMS   64

/*�����ļ���Ϣ*/
typedef struct tagFsmFileEntryInfo
{
    VOS_RW_LOCK_S    stLock;        /*�ļ�Ƭ��*/
    VOS_DLIST_NODE_S stFileList;    /*�ļ���*/   
    FSM_RENTRY_S     stFileInfo;    /*�ļ���Ϣ*/
}FSM_FILE_INFO_S, *PFSM_FILE_INFO_S;

/*�����ļ���Ϣ*/
typedef struct tagFsmMgrCtx
{
    CHAR                acCurDir[VOS_DIRSNAME_LEN];    /*��ǰ��·��*/
    PFSM_FILE_INFO_S    pstarryFileInfo[FSM_MAX_NUMS]; /*�����ļ��������������ܳ���64��Ч�ʽ���*/
    UINT32              uiCurNums;                     /*��ǰ�ļ��ĸ���*/
}FSM_MGR_CTX_S, *PFSM_MGR_CTX_S;

INT32   FSM_MgrEnvInit();

VOID    FSM_MgrEnvUnInit();




