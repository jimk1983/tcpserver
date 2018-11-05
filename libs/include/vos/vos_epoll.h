/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_epoll.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��12��
  ����޸�   :
  ��������   : EPOLL��װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��12��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_EPOLL_H_
#define _VOS_EPOLL_H_


/*EPOLL����-���*/
#define VOS_EPOLL_CTL_ADD   0x01
/*EPOLL����-�޸�*/
#define VOS_EPOLL_CTL_MOD   0x02
/*EPOLL����-ɾ��*/
#define VOS_EPOLL_CTL_DEL   0x04

/*EPOLL�¼�����--IN�¼�*/
#define VOS_EPOLL_MASK_POLLIN                 0x01
/*EPOLL�¼�����--OUT�¼�*/
#define VOS_EPOLL_MASK_POLLOUT              0x02
#define VOS_EPOLL_MASK_POLLNOTIFY         0x04

/*�򿪽���*/
#define VOS_EPOLL_CTRL_INOPEN               0x00
/*�رս���*/
#define VOS_EPOLL_CTRL_INCLOSE              0x01
/*�򿪷���*/
#define VOS_EPOLL_CTRL_OUTOPEN            0x02
/*�رշ���*/
#define VOS_EPOLL_CTRL_OUTCLOSE           0x03

/*EPOLL�Ƿ�ֵ*/
#define VOS_EPOLL_INVALID                       -1

/*�ɹ�������*/
#define VOS_EPOLL_OK                                0
/*ʧ�ܷ�����*/
#define VOS_EPOLL_ERR                              -1

/*����EPOLL�����*/
#define VOS_EPOLL_EVENT_MAXNUM          20000 

/*EPOLL����ȴ�*/
#define VOS_EPOLL_TIME_INFININ             -1
#define VOS_EPOLL_TIME_200S                  200000


/*���εȴ�EPOLL��Ӧ���¼���*/
#define VOS_EPOLL_EVTWAIT_NUM           48

/*����EPOLL��Ԫ*/
typedef struct tagVosEPollEvent
{
    INT32    lSockFd;               /*socket fd*/ 
    UINT32  ulEventMask;        /*�¼�����*/
}VOS_EPOLL_EVENT_S;


/*���η�����EPOLL�¼�*/
typedef struct tagVosEPollWaitEvt
{
    VOS_EPOLL_EVENT_S astEevent[VOS_EPOLL_EVTWAIT_NUM];     /*���η����������¼�*/
    UINT32                    ulEvtNum;                                               /*EPOLL�¼�����*/
}VOS_EPOLL_WAIT_EVT_S;

INT32 VOS_EPOLL_Create(UINT32 ulEpollNum);

VOID  VOS_EPOLL_Release(INT32 lEpollId);

INT32 VOS_EPOLL_Ctrl(INT32 lEpollFd, INT32 lSockfd, UINT32 ulEpollCtl, UINT32 ulEpollMask);

INT32 VOS_EPOLL_Wait(INT32 lEpollFd, VOS_EPOLL_WAIT_EVT_S *pstEvent, INT32 lTimeout);


#endif

