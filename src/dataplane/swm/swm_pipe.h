/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_pipe.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : ҵ��ܵ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*SWM �ܵ�����ˮλ����*/
/*��ˮλ, ���Ͷ��г�����ˮλ����ζ��Ҫ�ر��������գ�
���Ҵ�������ͣ�ĳ��Է���*/
#define SWM_PIPE_IOBUF_THLD_HIGH        10    

/*��ˮλ, �����ˮλʱ������������ռ���*/
#define SWM_PIPE_IOBUF_THLD_LOW         2         


/*SSL�̵߳Ĺܵ��ڵ��ҵ������*/
typedef enum SwmPipeType
{
    SWM_PIPE_TYPE_SRC_TLS = 0,          /*TLSԴ�ڵ�*/
    SWM_PIPE_TYPE_SRC_DTLS,             /*UDPSԴ�ڵ�*/
    SWM_PIPE_TYPE_FILTER,               /*�м���˽ڵ�*/
    SWM_PIPE_TYPE_BIZ_UPF,              /*������չ��ҵ��ڵ�*/
    SWM_PIPE_TYPE_BIZ_NEM,              /*������չ��ҵ��ڵ�*/
    SWM_PIPE_TYPE_BIZ_PFW,              /*�˿�ת����ҵ��ڵ�*/
    SWM_PIPE_TYPE_BIZ_TCM,              /*�ն˼���ҵ��ڵ�*/
    
    SWM_PIPE_TYPE_END
}SWM_PIPE_TYPE_E;


/*���ݴ�������*/
typedef LONG (*swm_pipe_dataupperproc_cb)(VOID *pvhandler, COM_IOBUF_S *pstIobuf);

/*���ݴ�������*/
typedef LONG (*swm_pipe_datadownproc_cb)(VOID *pvhandler, COM_IOBUF_S *pstIobuf);

/*������Ϣ����*/
typedef LONG (*swm_pipe_ctrlproc_cb)(VOID *pvhandler, ULONG ulCtrlCmd);

/*ɾ����Ϣ����*/
typedef VOID (*swm_pipe_delnotify_cb)(VOID *pvhandler);


typedef struct tagSwmPipeCallBack SWM_PIPE_CALLBACK_S;

/*�ܵ���ִ�к����ṹ*/
struct tagSwmPipeCallBack
{
    VOID *pvcbFunc;
    VOID *pvUserData;
};


/*�ص�������ʼ��*/
#define SWM_PIPE_CALLBACK_INIT(pstCallBack_, pfFunc_, pvUserData_)\
do{\
    (pstCallBack_)->pvcbFunc = (VOID *)pfFunc_;\
    (pstCallBack_)->pvUserData = (VOID *)pvUserData_;\
}while(0);

/*SWMģ��Ĺܵ��ڵ㣬��Ӧ��ÿһ��SSL����*/
typedef struct tagSwmPipeConn
{
    VOS_DLIST_NODE_S          stNode;                        /*���ܵ��ڵ�*/
    SWM_PIPE_TYPE_E            ePipeType;                   /*�ܵ�����*/
    
    SWM_PIPE_CALLBACK_S     stPipeDataUpperProc;   /*���ܵ���ִ�к�������������*/
    SWM_PIPE_CALLBACK_S     stPipeDataDownProc;    /*���ܵ���ִ�к�������������*/
    SWM_PIPE_CALLBACK_S     stPipeCtrlProc;              /*���ܵ��Ŀ��ƺ���, �رջ��ߴ򿪱��ܵ����շ�*/
    SWM_PIPE_CALLBACK_S     stPipeDelNotifyProc;      /*���ܵ���ɾ��֪ͨ����*/
}SWM_PIPE_CONN_S;

/*�ܵ��ڵ�ĳ�ʼ��*/
#define SWM_PIPE_CONN_INIT(pstPipeConns_,ePipeType_, pfDataUpper, pfDataDown,pfCtrl,pfDelNotify,pvData_) do{\
    VOS_DLIST_INIT(&(pstPipeConns_)->stNode);\
    (pstPipeConns_)->ePipeType  =  (ePipeType_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeDataUpperProc, pfDataUpper, pvData_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeDataDownProc, pfDataDown, pvData_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeCtrlProc, pfCtrl, pvData_);\
    SWM_PIPE_CALLBACK_INIT(&(pstPipeConns_)->stPipeDelNotifyProc, pfDelNotify, pvData_);\
}while(0);

LONG    SWM_TLS_PipeConnCreate(SWM_TLS_CONN_S *pstTlsConn);

VOID    SWM_TLS_PipeConnRelease(SWM_TLS_CONN_S *pstTlsConn);


#define SWM_PIPE_IOBUF_OK                               0
#define SWM_PIPE_IOBUF_UNKNOW_ERR             -1        /*-1: δ֪����*/
#define SWM_PIPE_IOBUF_EWOULDBLOCK            -2       /*-2: ������������*/
#define SWM_PIPE_IOBUF_PARAM_ERR                -3       /*-3: ����������*/
#define SWM_PIPE_IOBUF_PIPENODE_ERR           -4       /*-4: �Ҳ����ܵ��ڵ�*/
#define SWM_PIPE_IOBUF_AGAIN                         -5       /*-5, ��Ҫ��������*/
#define SWM_PIPE_IOBUF_PIPEFUNC_ERR           -6       /*-6, �ص�����δע��*/

LONG    SWM_TLS_PipeTransBufToNextPipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, COM_IOBUF_S *pstIobuf);

LONG    SWM_TLS_PipeTransBufToPrePipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, COM_IOBUF_S *pstIobuf);

SWM_PIPE_CONN_S *SWM_TLS_PipeConnFoundTlsNode(SWM_BIZ_CHANNEL_S   *pstBizChannel);

LONG    SWM_TLS_PipeTransCtrlToNextPipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, ULONG ulCmdCode);

LONG    SWM_TLS_PipeTransCtrlToPrePipeNode(SWM_PIPE_CONN_S *pstPipeCurNode, ULONG ulCmdCode);




