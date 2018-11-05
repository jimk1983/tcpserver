/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_ipcmsg.h
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ����ϵͳ�̼߳���Ϣ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/


#ifndef _VOS_IPCMSG_H_
#define _VOS_IPCMSG_H_

/*��Ϣ���͵����ݻ�����󳤶�*/
#define  VOS_MSG_BUF_SIZE            8192

/*��Ϣ����*/
typedef struct tagVOSMsg
{
    LONG lMsgType;          /*��Ϣ���ͣ�������Ϣ���ͽ���*/
    INTPTR iMsgPtr[4];       /*��Ϣ���ͽ��յĵ�ַ�� һ���ʹ��һ���ṹ���ַ���д���*/
}VOS_MSG_S;


INT32 VOS_GetSelfPhreadID();

INT32 VOS_MSGCreateByKeyID(INT32 ulMsgKeyId);
VOID VOS_MSGReleaseByKeyID(INT32 ulMsgKeyId);
INT32 VOS_MSGKeyIDIsExist(INT32 ulMsgKeyId);

INT32 VOS_MSGSend(INT32 ulMsgQueId, INTPTR iMsgPtr[4]);
INT32 VOS_MSGRecv(INT32 ulMsgQueId, INTPTR *iMsgPtr);

#if 0
LONG VOS_MsgQueSend(ULONG ulMsgQueId, CHAR *pcSndBuf, ULONG ulSndSize);
LONG VOS_MsgQueRecv(ULONG ulMsgQueId, CHAR *pcRcvBuf, ULONG *pulRcvSize);

LONG VOS_MsgQueSendCpy(ULONG ulMsgQueId, CHAR *pcSndBuf);
LONG VOS_MsgQueRecvCpy(ULONG ulMsgQueId, CHAR *pcRcvBuf);
#endif

#endif
