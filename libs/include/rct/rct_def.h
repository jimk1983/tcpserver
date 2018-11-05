#ifndef _RCT_DEF_H_
#define _RCT_DEF_H_
/******************************************************************************

                  ��Ȩ���� (C), 2017-2020, �������޹�˾

 ******************************************************************************
  �� �� ��   : rct_def.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2018��4��17��
  ����޸�   :
  ��������   : RCT�����Ķ���ͷ�ļ�, Ҳ�ṩ���ⲿ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��4��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

#if VOS_PLAT_LINUX
#define PTHREADVAL  __thread;   /*linuxƽ̨�������̱߳���*/
#else
#define PTHREADVAL
#endif

/*ͨ�õ��ַ�������*/
#define RCT_STR_LEN             32

/*Ĭ��ÿ���̵߳�EPOLL�������Ŀ*/
#define RCT_EPOLL_WAIT_NUMS     64

/*�����ÿ���̵߳�����*/
/*ע��SSL֮������ж���߳�*/
typedef enum
{
    RCT_TYPE_MP_CLI = 0,           /*����������߳�: ������*/
    RCT_TYPE_MP_AAA,               /*����������߳�: AAA����*/
    RCT_TYPE_MP_PROXY,             /*����������߳�: DNS����ҵ��*/
    RCT_TYPE_DP_TCP,               /*�����������߳�: TCP����*/
    RCT_TYPE_DP_SSL,               /*�����������߳�: SSL����*/
    RCT_TYPE_DP_UDPS,              /*�����������߳�: UDPS����*/
    RCT_TYPE_DP_TWORK,             /*�����������߳�: Timer���ڹ�������: ̽�ⶼ���Է�������*/
    RCT_TYPE_DP_VSNAT,             /*�����������߳�: ��������Tap��SNAT�߳�*/

    RCT_TYPE_NUMS,
}RCT_TYPE_E;


/*�ڵ�һ�߳����ҵ�����࣬����16������*/
#define RCT_SUBTYPE_MAXNUMS     16

/*Ĭ�ϵ�һ�㶼��1��ҵ������Ϊ0*/
#define RCT_SUBTYPE_SINGLE    0

/*ҵ������ֻ�е����̵߳�, SSL�ж���߳�*/
#define RCT_PTHNUM_ONLYONE    0

/*SSL�̵߳�������, ����Ҫע�����г�ʼ������
��ϢҲ������������ִ���*/
typedef enum
{
    RCT_SUBTYPE_SSL_SWM=0,
    RCT_SUBTYPE_SSL_NEM,
    RCT_SUBTYPE_SSL_PFM,
    RCT_SUBTYPE_SSL_TCM,

    RCT_SUBTYPE_SSLNUMS
}RCT_SUBTYPE_SSL_E;

#define RCT_TYPE_PTHMAXNUMS     32      /*ÿ��ҵ���������߳�����*/
#define RCT_TYPE_SIGLENUMS      1       /*Ĭ�Ͼ���1���߳�*/
#define RCT_TYPE_SSL_MUTILNUMS  2       /*SSL������2���߳�*/

#define RCT_MSGLOCKPLY_NONEED      0
#define RCT_MSGLOCKPLY_NEED        1   /*�Ƿ���Ҫ�ɿ����䣬�����Ҫ����Ὣ�û�������ס���Ա��ڷ���ʱ��ȷ����*/


/**************************************************************/
/*****************       ��Ϣ�Ľṹ                *****************/
/**************************************************************/

typedef struct tagRctMsgHead
{
    /*���ڻ�ȡͨ�ž���*/
    UINT32  ulSrcRctType;               /*�̵߳�����*/
    /*��Ϣ��ҵ�������ص������Ĵ���*/
    UINT32  ulSrcRctSubType;            /*�����̰߳������ģ�飬���磺 SSL������SWM/NEM/TCM��*/
    UINT32  ulSrcPthIndex;              /*ĳ�����µĵڼ����߳������� ����:SSL�������߳�*/
    UINT32  ulSrcMsgEventFd;            /*������Ϣ���أ�ֱ�Ӵ�������*/
    
    
    /*���ڻ�ȡͨ�ž���*/
    UINT32  ulDstRctType;               /*�̵߳�����*/
    /*��Ϣ��ҵ�������ص������Ĵ���*/
    UINT32  ulDstRctSubType;            /*�����̰߳������ģ�飬���磺 SSL������SWM/NEM/TCM��*/
    UINT32  ulDstPthIndex;              /*ĳ�����µĵڼ����߳������� ����:SSL�������߳�*/
    
    /*�ڵ����ȷ����Ϣ����ʱ�򣬽ڵ��Ѿ��ͷ�, ����������ڷ��������Ϣ�������������Ϣ�������øñ��*/
    /*�Ƿ���Ҫ�ɿ��ظ�: RCT_MSGLOCKPLY_NEED:
    ���ڵ�ǰ�̺߳ͱ���̵߳����ݲ�ѯ�ظ���Ϣ:
    ���磺 NEM������Ϣ��AAA��ѯ��¼��AAA��������أ���Ҫ��סConnon�ڵ�*/
    UINT32  ulBackFlag;            
    /*ָ��ǿ��ת��*/
    UINT32  ulLockRplyId;               /*��Ϣ���ID: 0: ��ʾ����Ϣ��>0��ʾ�ظ���Ϣ����Ҫ���*/
}RCT_MSG_HEAD_S;


/*ͨ�õĻص�����: ������Ϣ����, ��Buf���ܳ�������Vos�Ķ��нڵ㳤��*/
/*��Ϣ���ܷ��ʹ��������ݽ�����֪ͨ��*/
typedef void (*pfComMsgHandlerCb)(char *pcBuf);

typedef void (*pfComMsgHandlerCallBack)(RCT_MSG_HEAD_S *pstHeadInfo, CHAR *pcMsg, UINT32 uiLen);


/*�ڲ��Ĵ�����������*/
typedef struct  tagRctEvtReactor    RCT_EVTREACTOR_S;


/*��ǰ�����߳�������ÿ���߳̿��԰�CPU�����һ�����ָCPU�ĸ���*/
#define RCT_PTHMAXNUMS      128

/*�̵߳�������Ϣ*/
typedef struct tagRctIndexInfo
{
    ULONG      ulRctType;               /*Rct���߳�����*/
    ULONG      ulPthSubIndex;           /*��ǰ�����߳�����, SSL�ж���̣߳�����Ҫ�������*/
    ULONG      ulArryIndex;             /*��¼��ǰ�̴߳��ȫ�������������ڵ�������*/
    ULONG      ulMsgEventFd;            /*������Ϣ��¼����*/
}RCT_INDEXINFO_S, *PRCT_INDEXINFO_S;




/*��Ϣ���͵�ʱ��Ŀ����Ϣ�ķ�װ�ṹ*/
typedef struct tagRctMsgDstInfo
{
    ULONG ulSrcSubType;     /*Դ������: �߳������Ϳ����Զ���ȡ��������������Ҫ�û���д, �󲿷�RCT_SUBTYPE_SINGLE
                             *Ŀǰ��SSL���ڵ����߳����ж����ҵ�����ͣ������߳�ҵ�񶼺ܵ�һ*/
    ULONG ulDstRctType;     /*Ŀ������: RCT_TYPE_E*/
    ULONG ulDstSubType;     /*Ŀ�������ͣ� �󲿷�RCT_SUBTYPE_SINGLE*/
    ULONG ulDstPthIndex;    /*Ŀ���߳������� �����̣߳�SSL�ж�����󲿷�ֻ��һ��: 0����:RCT_PTHNUM_ONLYONE*/
}RCTMSG_DSTINFO_S, *PRCTMSG_DSTINFO_S;

#endif