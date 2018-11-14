/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, ���޹�˾

 ******************************************************************************
  �� �� ��   : fsm_def.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��9��
  ����޸�   :
  ��������   : �ļ���ض���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/


/*����ֵ�ĳ���*/
#define FSM_VAL_LEN    36

#define FSM_CRCALGM_MD5     0
#define FSM_CRCALGM_SHA1    1


/*��Դ�ļ���Ϣ*/
typedef struct tagFsmFileResourceInfo
{
    UINT32 uiFileVersion;                   /*�ļ��汾��Ϣ*/
    UINT32 uiFileSize;                      /*�ļ���С*/
    UINT32 uiFileCRCAlgm;                   /*�ļ�У���㷨����*/
    CHAR   acFileCRCVal[FSM_VAL_LEN];       /*�ļ�У��ֵ*/
}FSM_RFILEINFO_S, *PFSM_RFILEINFO_S;


/*�ļ�����Դ*/
typedef struct tagFsmFileResourceEntry
{
    CHAR acFullName[VOS_DIRSNAME_LEN];      /*�ļ����·��*/
    FSM_RFILEINFO_S    stFileResInfo;       /*�ļ���Ϣ*/
}FSM_RENTRY_S, *PFSM_RENTRY_S;



