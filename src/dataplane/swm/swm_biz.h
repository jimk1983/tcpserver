/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_biz.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : bizchannelҵ��ͨ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/


#define EMSDK_VERSION		0x10

typedef enum
{
	EMPTO_BIZTYPEID_UNKNOW = 0,
    EMPTO_BIZTYPEID_UPF,			/*�ļ�����ҵ������*/
	EMPTO_BIZTYPEID_NEM,			/*������չҵ������*/
	EMPTO_BIZTYPEID_PFM,			/*�˿�ת��ҵ������*/
	EMPTO_BIZTYPEID_TCM,			/*�ն˼��ҵ������*/
	EMPTO_BIZTYPEID_LNM,			/*License��������: �ṩ���ӵ���Ȩ����*/

	EMPTO_BIZTYPEID_NUMS
}EMPTO_BIZTID_E;



/*��ҵ��ͨ��δ��*/
#define SWM_BIZ_USER_UNBINDED     0
/*��ҵ��ͨ���Ѱ�*/
#define SWM_BIZ_USER_BINDED         1

/*��ͨ���Ŀͻ�����Ϣ*/
typedef struct tagSWMBizClientInfo
{
    /*�������ַ*/
    UINT32   ulClientAddr;
    /*�ͻ��˶˿�*/
    UINT32   ulClientPort;

}SWM_BIZ_CLIENT_INFO_S;


/*��ͨ���󶨵��û���������Ϣ*/
typedef struct tagSWMBizUserInfo
{
    UINT32   ulUserID;           /*�û���ID*/
    UINT32   ulSessionID;       /*�û��ĻỰID*/
    UINT32 	 uiFamilyID;		/*��ͥ��Ա��Ϣ*/
    
}SWM_BIZ_USER_INFO_S;

/*��ͨ���������������ص���Ϣ*/
typedef struct tagSWMBizVtInfo
{
    ULONG           ulVTID;                 /*�����������ص�ID*/
    CHAR            acGatewayUdpsAddr[VOS_SOCK_IPADDR_LEN]; 	/*���ص�Udps���ܱ���ת����ַ */
    UINT32          uiGatewayUdpsPort;	

}SWM_BIZ_VT_INFO_S;


/*SWMҵ��ͨ��*/
struct tagSWMBizChannel 
{
    /*ͨ���ڵ�*/
    VOS_DLIST_NODE_S    stNode;

    /*�ܵ���ͷ�ڵ�*/
    VOS_DLIST_NODE_S    stPipeHead;
    /*�ܵ��ڵ�����*/
    ULONG   ulPipeNum;

    /*��Bizͨ����ҵ������*/
    ULONG   ulBizType;
    /*�Ƿ��Ѿ���*/
    ULONG   ulBindFlags;

    /*�Ƿ������ͨ��*/
    ULONG   ulExitConfirm;
    
    /*��ͨ���Ŀͻ�����Ϣ*/
    SWM_BIZ_CLIENT_INFO_S 	stClientInfo;
    /*��ͨ�����û���Ϣ*/
    //SWM_BIZ_USER_INFO_S    	stUserInfo;
    /*��ͨ����������Ϣ*/
    //SWM_BIZ_VT_INFO_S       stVtInfo;
    
    /*���Ͷ���*/
    COM_IOBUF_QUE_S   *pstSwmSendQueue;
    /*ssl��Ϣ*/
    //SSL_S *pstSsl;
};


SWM_BIZ_CHANNEL_S *SWM_Biz_ChannelCreate(LONG lSockfd);

VOID SWM_Biz_ChannelRelease(SWM_BIZ_CHANNEL_S *pstBizChannel);

/*************************************************************************/

typedef struct tagSwmBizMatchCtx SWM_BIZ_MATCH_CTX_S;
/*�ص�����������*/
struct tagSwmBizMatchCtx
{
    ULONG      ulBizType;         /*ҵ��ʶ������*/
    VOID        *pvFunc;            /*ƥ�亯��*/
    VOID        *pvCtx;              /*ÿ��ҵ���������*/
};

typedef LONG (*swm_biz_match_func)(SWM_BIZ_CHANNEL_S *pstBizChannel, CHAR *pcData, ULONG ulDataLen);

/*SWM ҵ��������*/
typedef struct tagSwmBizMatch
{
    SWM_BIZ_MATCH_CTX_S    stArrayBizTypeMatch[EMPTO_BIZTYPEID_NUMS];     /*ҵ��ͨ����ƥ��*/
}SWM_BIZ_Match_S;


/*ͷ����ƫ�ƽṹ˵��: ͷ��12���ֽ�
*˵����
*		���ͷ����Ϣ�ڵ�¼�ȿ�����������£�������SSL��
*		����ͷ����Ϣ�����ǲ��ü��ܵģ������ֶζ�������д����
*		�������ͷ����ʹ�����ݴ��䣬������UDPS������£����Զ�EMPTO_HEADINFO_S��Ա���е��ֽڼ��ܵ�
*/

/*������չ���Ʊ���: һ�����TLV��ʽ*/
typedef struct tagSwmBizHead
{
    UINT32      uiVersion;              //�汾��
    UINT32      uiHeadSize;             //ͷ����С
	UINT32      uiMark1;                /*ҵ��ʶ��������A*/
	UINT32      uiMark2;                /*ҵ��ʶ��������B*/
    UINT32      uiDataLen;              /*���������ݳ���*/
}SWM_BIZ_HEAD_S, *PSWM_BIZ_HEAD_S;


/*ͨ�õ�Э���ʽ*/
#define BIZ_HEAD_TYPE_CTRL     0x0001        /*����Э��*/
#define BIZ_HEAD_TYPE_DATA     0x0002        /*����Э��*/


/*���峤��*/
#define SWM_BIZ_HEAD_LEN        sizeof(SWM_BIZ_HEAD_S)

LONG SWM_Biz_ChannelMatchInit();

VOID SWM_Biz_ChannelMatchUnInit();

LONG SWM_Biz_ChannelMatch(SWM_BIZ_CHANNEL_S *pstBizChannel, CHAR *pcData, ULONG ulDataLen);

LONG SWM_Biz_ChannelMatchRegister(SWM_BIZ_MATCH_CTX_S *pstBizMatch);

LONG SWM_Biz_ChannelMatchUnRegister(ULONG ulBizType);

LONG SWM_Biz_ChannelCheckLen(CHAR *pcPack, UINT32 uiPackLen);

LONG SWM_Biz_ChannelPreGetPackLen(CHAR *pcPack);


