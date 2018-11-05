/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_pevt.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��24��
  ����޸�   :
  ��������   : ����EPOLL���¼�����֪ͨ���ƣ�����socket�������¼���ʽ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��24��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*�ö������Ƴ���*/
#define VOS_PTHEVT_NAME_LEN              32

/*��ഴ���Ķ�������*/
#define VOS_PTHEVT_MAX_NUMS             256

/*�¼����е��δ�����������*/
#define VOS_PTHEVT_DPTCH_NUMS         64

/*д���¼����У���֪ͨ*/
typedef LONG (*VOS_PTHEVT_WRITE)(UINT32 uiQueID, UINT32 uiEvtype, VOID *pcData);

/*��ȡ��������*/
typedef LONG (*VOS_PTHEVT_READ)(UINT32 uiQueID, VOS_DLIST_NODE_S *pstEvts);

#if VOS_PLAT_WIN
typedef  HANDLE      VOS_PTHEVT_T;
#else 
typedef  UINT32       VOS_PTHEVT_T;
#endif


/*�߳�֮�����Ϣͨ�Ŷ��У��¼�֪ͨ��ʽ
������ڿͻ�����Ϊ��Ҫ*/
typedef struct tagVosPthEvtQue
{
    CHAR        acQueName[VOS_PTHEVT_NAME_LEN];     /*�ö��е�����*/
    UINT32     uiQueID;                                               /*�ö��е�ID, Ҳ����������*/
    UINT32     uiFreeFlag;                                            /*���п��б��,���ڶ��еĶ�дȷ��
                                                                                 *ֻ�е�Ϊ0ʱ�򣬲ſ���д��*/
    UINT32     uiEventNums;                                        /*��ǰ�����ڲ��¼���������
                                                                                 *����ܳ���VOS_PTHEVT_DPTCH_NUMS(64)*/
    VOS_PTHEVT_T         stEventHandle;                      /*�ö��е�ʱ����*/
                                                                                 
    VOS_DLIST_NODE_S  stEvtList;                               /*�¼������е��¼�����*/
                                                                                 
    VOS_PTHEVT_WRITE pfEvtWrite;                            /*�¼�֪ͨ*/
                                                                                 
    VOS_PTHEVT_READ   pfEvtRead;                            /*�¼���ȡ*/
                                                                                 
}VOS_PTHEVT_QUE_S;



/*************************�û����������¼�������ע����Ӧ�Ĵ�����**************************************/
#define VOS_PTHEVT_TYPE_PACKIN 0x00000001              /*���ݰ��¼�*/
#define VOS_PTHEVT_TYPE_TIMER  0x00000002              /*��ʱ���¼�*/

/*���¶�Ӧ������ҲҪ�޸�*/
typedef enum
{
    VOS_PTHEVT_TYPE_INDEX_PACKIN =0,
    VOS_PTHEVT_TYPE_INDEX_TIMER,

    
}VOS_PTHEVT_TYPE_E;

    
/*ÿ��������¼��ڵ�*/
typedef struct tagVosPthEvtNode
{
    VOS_DLIST_NODE_S stNode;            /*����ڵ�*/
    
    UINT32          uiEvttype;                  /*�¼�����*/

    VOID*           pvData;                     /*�û�����*/
    
}VOS_PTHEVT_NODE_S;


