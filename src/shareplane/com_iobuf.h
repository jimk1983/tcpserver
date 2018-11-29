/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : com_iobuf.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年10月28日
  最近修改   :
  功能描述   : IOBUF的通用方法实现
  函数列表   :
  修改历史   :
  1.日    期   : 2016年10月28日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

/*Iobuf主要是网络首发使用，目前根据经验，使用16K大小进行收发*/
#define COM_IOBUF_LEN              16384

/*设计一个IOBUF, 可以挂链*/
typedef struct tagComIobuf
{
    VOS_DLIST_NODE_S    stNode;           /*挂链用的节点*/
    ULONG               ulMagic;          /*本节点最大的数据长度*/
    ULONG               ulMaxLen;         /*接收包的数据最大的长度*/  
    ULONG               ulPreDataLen;     /*预先获取的要接收的包的大小*/
    ULONG               ulDataLen;        /*本次的接收到的包的大小或者待发送的大小*/
    ULONG               ulOffset;         /*数据偏移*/
    ULONG               lLeftLen;         /*剩余未有使用的缓存长度*/
    CHAR                pcData[0];        /*数据起始地址*/
}COM_IOBUF_S, *PCOM_IOBUF_S;

/**************************iOBUF内的未使用数据使用*************************************/
/*获取未使用的数据*/
#define COM_IOBUF_GETSAPCE_DATA(_pstIobuf) (CHAR *)((_pstIobuf)->pcData +(_pstIobuf)->ulDataLen)

/*获取未使用的BUF长度*/
#define COM_IOBUF_GETSPACE_LEN(_pstIobuf) ((_pstIobuf)->lLeftLen)

/*数据接收更新长度,使用者需要自己保证不能越界*/
#define COM_IOBUF_SETINPUTED_LEN(_pstIobuf, ulInputLen) \
do{\
    if(ulInputLen <= (_pstIobuf)->lLeftLen)\
    {\
        (_pstIobuf)->ulDataLen += ulInputLen;\
        (_pstIobuf)->lLeftLen  = (_pstIobuf)->ulMaxLen - (_pstIobuf)->ulDataLen;\
    }\
}while(0);

/****************************IOBUF内部已经保存使用的数据使用********************************/
/*获取已经存放的未发送的数据*/
#define COM_IOBUF_GETSAVED_DATA(_pstIobuf) (CHAR *)((_pstIobuf)->pcData + (_pstIobuf)->ulOffset)
/*获取已经存放的未使用数据总长度*/
#define COM_IOBUF_GETSAVED_LEN(_pstIobuf)    ((_pstIobuf)->ulDataLen - (_pstIobuf)->ulOffset)
/*更新发送的(ulOffset 表示已经使用了)数据长度*/
#define COM_IOBUF_SETOUTPUTED_LEN(_pstIobuf, ulSndLen) \
do{\
    ((_pstIobuf)->ulOffset +=ulSndLen );\
}while(0);

/*数据结构大小*/
#define COM_IOBUF_SIZE              sizeof(COM_IOBUF_S)

/*T数据包的申请内存的大小*/
#define COM_IOBUF_MALLOC_SIZE   COM_IOBUF_SIZE + COM_IOBUF_LEN

/*SWM踩内存的Magic*/
#define COM_IOBUF_MAGIC       0xAABBCCDD

COM_IOBUF_S *COM_Iobuf_Malloc(ULONG ulMid);

VOID COM_Iobuf_Free(COM_IOBUF_S *pstIobuf);



/*************************
IOBUF对列，基本都在同一个线程，不加锁了
因此如果是跨线程的队列，需要用户自己注意
**************************/
/*iobuf的队列*/
typedef struct tagComIobufQue
{
    VOS_DLIST_NODE_S stList;            /*IOBUF的队列*/
    INT32                     uiNum;          /*队列中的数目*/
}COM_IOBUF_QUE_S;

COM_IOBUF_QUE_S *COM_Iobuf_QueCreate();

VOID COM_Iobuf_QueFree(COM_IOBUF_QUE_S *pstIobufQue);

INT32 COM_Iobuf_QuePush(COM_IOBUF_QUE_S *pstIobufQue, COM_IOBUF_S *pstIobuf);

COM_IOBUF_S * COM_Iobuf_QuePop(COM_IOBUF_QUE_S *pstIobufQue);

/***************************************************/
UINT32 COM_Iobuf_QueGetNums(COM_IOBUF_QUE_S *pstIobufQue);

LONG    COM_Iobuf_QueIsEmpty(COM_IOBUF_QUE_S *pstIobufQue);

