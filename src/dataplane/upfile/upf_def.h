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

/*����������*/
typedef  enum
{
    UPSER_CTLCODE_UNKNOW=0,
    UPSER_CTLCODE_KPVINFO_REQ,      //---�汾��Ϣ����(������Ϣ)
    UPSER_CTLCODE_KPVINFO_RESP,  //---�汾��Ϣ��Ӧ
    UPSER_CTLCODE_XMLFILE_GET,    //XML�ļ���ȡ����
    UPSER_CTLCODE_XMLFILE,        //XML�ļ�����
    UPSER_CTLCODE_APPFILE_GET,    //APP�ļ���ȡ����
    UPSER_CTLCODE_APPFILE,        //APP�ļ�����
    
    UPSER_CTLCODE_NUMS
}UPSER_CTLCODE_E;


/*�豸��ǩ����*/
#define UPF_DEVLAB_LEN      32

#define UPF_FILELEN         32
#define UPF_FILEVALEN       32
/*������Ϣ*/
typedef struct tagUpfKVPInfo
{
	UINT32      uiLastVersion;				    /*��ǰ���°汾*/
	UINT32      uiIntervalRate;				    /*���ʱ��*/
    CHAR 		acFileAddress[UPF_FILELEN];		/*XML�ļ���ַ��192.168.1.1��8080/aaa.xml*/
    UINT32  	uiFileSize;			            /*XML�ļ���С*/
    CHAR 		acFileValue [UPF_FILEVALEN];	/*XML��MD5ֵ*/
}UPF_KVPINFO_S, *PUPF_KVPINFO_S;

#define UPF_FTRNKID_BEGIN       "--BEGIN--"
#define UPF_FTRNKID_TRANING     "--TNFER--"
#define UPF_FTRNKID_END         "--END--"

/*ʹ��16K��IOBUF����,����������Ҫ��ȥͷ�����ִ�С*/
#define UPF_CONTENTLEN      16000   

typedef struct tagUpfFileDataInfo
{
    UINT32 		uiAPPVersion;			        /*��ǰ�汾��*/
    CHAR 		acFileAddress[UPF_FILELEN];		/*XML�ļ���ַ��192.168.1.1��8080/aaa.xml*/
    UINT32  	uiFileSize;			            /*XML�ļ���С*/
    CHAR 		acFileValue [UPF_FILEVALEN];	/*XML��MD5ֵ*/
    CHAR 		acTunkID[UPF_FILEVALEN];	    /*---Begin---/---transfer---/---END---*/
    UINT32 		uiTunkLength;			        /*��Ƭ�ļ�����*/
    CHAR 		acFileContent[UPF_CONTENTLEN];	/*�ļ�����*/
}UPF_FILEDATA_S, *PUPF_FILEDATA_S;


/*��ʽͷ*/
typedef struct tagUPFHead
{
    UINT32          uiCtrlCode;             /*������Ϣ��*/
    UINT32          uiCtrlLength;           /*���ݳ���*/
    CHAR	        acDeviceLab[UPF_DEVLAB_LEN];	/*�豸��ǩ*/
}UPF_HEAD_S, *PUPF_HEAD_S;

/*****************************************************************/
typedef struct tagUPFConn                UPF_CONN_S;

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

    /*�ļ�Ƭ: Ŀǰ��֧��С�ļ���16K*n
      ���ļ����ܲ��ø���������ռ�ڴ�̫��, ���Կ��Ƿֶ���ȡ*/
    VOS_DLIST_NODE_S            stFileSlice;
};


