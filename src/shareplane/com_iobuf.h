/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : com_iobuf.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��10��28��
  ����޸�   :
  ��������   : IOBUF��ͨ�÷���ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*Iobuf��Ҫ�������׷�ʹ�ã�Ŀǰ���ݾ��飬ʹ��16K��С�����շ�*/
#define COM_IOBUF_LEN              16384

/*���һ��IOBUF, ���Թ���*/
typedef struct tagComIobuf
{
    VOS_DLIST_NODE_S    stNode;           /*�����õĽڵ�*/
    ULONG               ulMagic;          /*���ڵ��������ݳ���*/
    ULONG               ulMaxLen;         /*���հ����������ĳ���*/  
    ULONG               ulPreDataLen;     /*Ԥ�Ȼ�ȡ��Ҫ���յİ��Ĵ�С*/
    ULONG               ulDataLen;        /*���εĽ��յ��İ��Ĵ�С���ߴ����͵Ĵ�С*/
    ULONG               ulOffset;         /*����ƫ��*/
    ULONG               lLeftLen;         /*ʣ��δ��ʹ�õĻ��泤��*/
    CHAR                pcData[0];        /*������ʼ��ַ*/
}COM_IOBUF_S, *PCOM_IOBUF_S;

/**************************iOBUF�ڵ�δʹ������ʹ��*************************************/
/*��ȡδʹ�õ�����*/
#define COM_IOBUF_GETSAPCE_DATA(_pstIobuf) (CHAR *)((_pstIobuf)->pcData +(_pstIobuf)->ulDataLen)

/*��ȡδʹ�õ�BUF����*/
#define COM_IOBUF_GETSPACE_LEN(_pstIobuf) ((_pstIobuf)->lLeftLen)

/*���ݽ��ո��³���,ʹ������Ҫ�Լ���֤����Խ��*/
#define COM_IOBUF_SETINPUTED_LEN(_pstIobuf, ulInputLen) \
do{\
    if(ulInputLen <= (_pstIobuf)->lLeftLen)\
    {\
        (_pstIobuf)->ulDataLen += ulInputLen;\
        (_pstIobuf)->lLeftLen  = (_pstIobuf)->ulMaxLen - (_pstIobuf)->ulDataLen;\
    }\
}while(0);

/****************************IOBUF�ڲ��Ѿ�����ʹ�õ�����ʹ��********************************/
/*��ȡ�Ѿ���ŵ�δ���͵�����*/
#define COM_IOBUF_GETSAVED_DATA(_pstIobuf) (CHAR *)((_pstIobuf)->pcData + (_pstIobuf)->ulOffset)
/*��ȡ�Ѿ���ŵ�δʹ�������ܳ���*/
#define COM_IOBUF_GETSAVED_LEN(_pstIobuf)    ((_pstIobuf)->ulDataLen - (_pstIobuf)->ulOffset)
/*���·��͵�(ulOffset ��ʾ�Ѿ�ʹ����)���ݳ���*/
#define COM_IOBUF_SETOUTPUTED_LEN(_pstIobuf, ulSndLen) \
do{\
    ((_pstIobuf)->ulOffset +=ulSndLen );\
}while(0);

/*���ݽṹ��С*/
#define COM_IOBUF_SIZE              sizeof(COM_IOBUF_S)

/*T���ݰ��������ڴ�Ĵ�С*/
#define COM_IOBUF_MALLOC_SIZE   COM_IOBUF_SIZE + COM_IOBUF_LEN

/*SWM���ڴ��Magic*/
#define COM_IOBUF_MAGIC       0xAABBCCDD

COM_IOBUF_S *COM_Iobuf_Malloc(ULONG ulMid);

VOID COM_Iobuf_Free(COM_IOBUF_S *pstIobuf);



/*************************
IOBUF���У���������ͬһ���̣߳���������
�������ǿ��̵߳Ķ��У���Ҫ�û��Լ�ע��
**************************/
/*iobuf�Ķ���*/
typedef struct tagComIobufQue
{
    VOS_DLIST_NODE_S stList;            /*IOBUF�Ķ���*/
    INT32                     uiNum;          /*�����е���Ŀ*/
}COM_IOBUF_QUE_S;

COM_IOBUF_QUE_S *COM_Iobuf_QueCreate();

VOID COM_Iobuf_QueFree(COM_IOBUF_QUE_S *pstIobufQue);

INT32 COM_Iobuf_QuePush(COM_IOBUF_QUE_S *pstIobufQue, COM_IOBUF_S *pstIobuf);

COM_IOBUF_S * COM_Iobuf_QuePop(COM_IOBUF_QUE_S *pstIobufQue);

/***************************************************/
UINT32 COM_Iobuf_QueGetNums(COM_IOBUF_QUE_S *pstIobufQue);

LONG    COM_Iobuf_QueIsEmpty(COM_IOBUF_QUE_S *pstIobufQue);

