/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : pfw_def.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2017��5��1��
  ����޸�   :
  ��������   : �˿�ת���Ķ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��5��1��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*SWM_BIZ_HEAD_S + UPF_HEAD_S + (TYPE) */


/*����������*/
typedef  enum
{
    UPSER_CTLCODE_UNKNOW = 0,
    UPSER_CTLCODE_KPVINFO_REQ,    //�汾��Ϣ����(������Ϣ)
    UPSER_CTLCODE_KPVINFO_RESP,   //�汾��Ϣ��Ӧ
    UPSER_CTLCODE_XMLFILE_GET,    //XML�ļ���ȡ����
    UPSER_CTLCODE_XMLFILE,        //XML�ļ�����
    UPSER_CTLCODE_APPFILE_GET,    //APP�ļ���ȡ����
    UPSER_CTLCODE_APPFILE,        //APP�ļ�����

    UPSER_CTLCODE_NUMS
}UPSER_CTLCODE_E;


/*�豸��ǩ����*/
#define UPF_DEVLAB_LEN      64
#define UPF_FILELEN         32
#define UPF_FILEVALEN       36

/*ʹ��16K��IOBUF����,����������Ҫ��ȥͷ�����ִ�С*/
#define UPF_CONTENTLEN      16000   

typedef enum
{
    UPDEV_STATUSCODE_STOP = 0,
    UPDEV_STATUSCODE_WORKING,

    UPDEV_STATUSCODE_NUMS
}UPDEV_STATUSCODE_E;


typedef enum
{
    UPDEV_CMDCODE_UNKNOW = 0,
    IPDEV_CMDCODE_UPDATA_AUTO,  /* �Զ��������� */
    UPDEV_CMDCODE_UPDATA_FORCE, /* ǿ�Ƹ��� */
    UPDEV_CMDCODE_NUMS
}UPDEV_CMDCODE_E;
    
/*������Ϣ*/
typedef struct tagUPFKVPInfo
{
    CHAR        acDevLabel[UPF_DEVLAB_LEN];         /*�豸��ǩ*/
    CHAR        acDevDecptor[UPF_DEVLAB_LEN];       /*�豸����*/
    UINT32      uiDevStatusCode;                    /*�豸״̬��*/
    UINT32      uiTerminalStatusCode;               /*�ն�״̬��*/
    UINT32      uiLastVersion;                      /*��ǰ���°汾*/
    UINT32      uiIntervalRate;                     /*���ʱ��*/
    UINT32      uiFileSize;                         /*XML�ļ���С*/
    CHAR        acAddrInfo[UPF_FILELEN];            /*��������ַ*/
    CHAR        acFileName[UPF_FILELEN];            /*�ļ�����*/
    CHAR        acFileValue[UPF_FILEVALEN];         /*�ļ�MD5ֵ*/
    USHORT      usChunkNums;                        /*������*/
    USHORT      usCmdCode;                          /*�������*/
    UINT32      uiConnNums;                         /*������������*/
}UPF_KVPINFO_S, *PUPF_KVPINFO_S;


typedef struct tagUpfFileDataInfoRequest
{
    UINT32      uiAPPVersion;                   /*��ǰ�汾��*/
    UINT32      uiFileSize;                     /*�ļ��ܴ�С*/
    CHAR        acFileName[UPF_FILELEN];        /*�ļ�����*/
    UINT32      uiChunkStart;                   /*������*/
    UINT32      uiChunkRange;                   /*�鷶Χ*/
    UINT32      uiChunkSize;                    /*���С*/
}UPF_FILEDATA_REQ_S, *PUPF_FILEDATA_REQ_S;

typedef struct tagUpfFileDataInfoResponse
{
    UINT32      uiAPPVersion;                   /*��ǰ�汾��*/
    UINT32      uiFileSize;                     /*�ļ��ܴ�С*/
    CHAR        acFileName[UPF_FILELEN];        /*�ļ�����*/
    UINT32      uiChunkEncryptType;             /*������㷨����*/
    UINT32      uiChunkCRCType;                 /*��У���㷨����*/
    CHAR        acChunkValue[UPF_FILEVALEN];    /*��У����*/
    UINT32      uiChunkNums;                    /*������*/
    UINT32      uiChunkIndex;                   /*������*/
    UINT32      uiChunkSize;                    /*���С*/
    CHAR        acChunkContent[UPF_CONTENTLEN]; /*������*/
}UPF_FILEDATA_RESP_S, *PUPF_FILEDATA_RESP_S;

/*��ʽͷ*/
typedef struct tagUPFHead
{
    UINT32       uiCtrlCode;                    /*������Ϣ��*/
    UINT32       uiCtrlLength;                  /*���ݳ���*/
}UPF_HEAD_S, *PUPF_HEAD_S;


/*****************************************************************/
typedef struct tagUPFConn                UPF_CONN_S;

/*�������״̬*/
#define UPF_TRNSTATUS_SNDEND        0
/*������״̬*/
#define UPF_TRNSTATUS_SNDING        1

/*�����ڵ�*/
struct tagUPFConn
{
    /*Nem�ܵ��ڵ�*/
    SWM_PIPE_CONN_S             stPipe;

    /*��ͨ����BizChannel����*/
    SWM_BIZ_CHANNEL_S*          pstBizChannel;

    /*�����¼�*/
    RCT_NETOPT_EVENT_S          stNetEvtOps;

    /*�ϻ��¼�*/
    RCT_EXPIROPT_EVENT_S        stExpireOps;

    COM_IOBUF_S *               pstRcvIobuf;
    
    /*��ǰҪ���͵��ļ���Ϣ*/
    FSM_RENTRY_S                stFileEntryInfo;    

    /*�Ƿ��Ѿ���Redis��������ע��*/
    UINT32                      uiRdsRegted;
   
    /*��ǰ����Ŀ�����*/
    UINT32                      uiUpfCtrlCode;
    /*�ļ�Ƭ����״̬*/
    UINT32                      uiMgrChunkStatus;
    /*�ļ�Ƭ�������ʼ����*/
    UINT32                      uiMgrChunkStartIndex;
    /*�Ѿ����͵�����*/
    UINT32                      uiMgrChunkCount;
    /*�ļ�Ƭ�ܸ���*/
    UINT32                      uiMgrChunkNums;
};


