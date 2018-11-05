/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_mpools.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��4��29��
  ����޸�   :
  ��������   : ��̬�ڴ�ص����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

/* Ĭ�ϴ����Ķ��г���*/ 
#define     VOS_LOOKSIDE_NUMS       100

/*���Ķ��з�ֵ: ������ֵ����*/
#define     VOS_THRED_NUMS          10000

/* �ڴ�ص���� */
typedef struct VOSMPoolLookSideList
{   
    UINT32           uiEntrySize;        /*���ڳص��ڴ��С*/

    VOS_RW_LOCK_S    stPoolListLock;     /*���ڴ�ص���*/

    VOS_DLIST_NODE_S stPoolListFree;     /*���еĴ��*/

    UINT32           uiNums;             /*��ǰ�ó��е�����*/
    
}VOS_PLookSideList, *PVOS_PLookSideList;



LONG    VOS_LookSideListInit(VOS_PLookSideList **ppstLookSideList,UINT32 uiEntrySize);

VOID    VOS_LookSideListUnInit(VOS_PLookSideList *pstLookSideList);

VOID   *VOS_AllcateFromLookSideList(VOS_PLookSideList *pstLookSideList);

VOID    VOS_FreeToLookSideList(VOS_PLookSideList *pstLookSideList, VOID *pvEntry);




