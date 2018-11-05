/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_que.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��5��21��
  ����޸�   :
  ��������   : �򵥵Ļ��ζ���ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��5��21��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*�򵥵���Ϣ�������, ͨ��UDP�������ö��������Ҫ���ں˼���Ϣ */
typedef struct tagVosMsgQue
{
    VOS_DLIST_NODE_S     stList;      /*���е�ͷ*/
    VOS_RW_LOCK_S        stLock;      /*���е���*/
    UINT32               uiNum;       /*��������Ϣ���ظ���*/
    UINT32               uiIndex;     /*�������ڵ�ȫ���߳���Ϣ����, ��ʼ��ʱȷ�������������ڲ���*/
}VOS_MSG_QUE_S;

/*������ݹ���ʹ��VOS_Malloc()�������ڴ棬�̼߳���Է��ʵ������з����ڴ淽ʽ*/
#define VOS_MSGQUE_DATA_LEN     1920        /*���ݵ���Ϣ�ĳ���,����С���ڴ�*/

/*���崫����Ϣ�Ľڵ�*/
typedef struct tagVosMsgQueNode
{
    VOS_DLIST_NODE_S    stNode;                     
    CHAR                acMsgData[VOS_MSGQUE_DATA_LEN];    /*���崫�ݵ���Ϣ*/
}VOS_MSG_QUE_NODE_S;

/*��ȡ���нڵ������*/
#define VOS_RCTMSGQUENODE_GETDATA(pstMsgNode)       (&pstMsgNode->acMsgData[0])

VOS_MSG_QUE_S *VOS_Rct_MsgQueCreate(UINT32 uiIndex);
VOID VOS_Rct_MSGQue_Release(VOS_MSG_QUE_S *pstMsgQue);

INT32 VOS_Rct_MSGQue_Push(VOS_MSG_QUE_S *pstMsgQue, VOS_MSG_QUE_NODE_S *pstMsgNode);
VOS_MSG_QUE_NODE_S * VOS_Rct_MSGQue_Pop(VOS_MSG_QUE_S *pstMsgQue);

VOS_MSG_QUE_NODE_S *VOS_Rct_MSGQueNode_Create();
VOID VOS_Rct_MSGQueNode_Release(VOS_MSG_QUE_NODE_S *pstQueNode);

BOOL VOS_Rct_MSGQue_IsEmpty(VOS_MSG_QUE_S *pstMsgQue);







