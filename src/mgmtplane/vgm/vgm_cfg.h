/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vgm_cfg.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��3��
  ����޸�   :
  ��������   : Vgateway������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*����ҵ�������*/
#define VGM_DEFAULT_PORT    9888
#define VGM_DEFAULT_IPADDR  "0.0.0.0"

/*����������ȫ�ֱ������������ݽϴ����������ݽ��м���*/
/*Ŀǰ�Ȱ��մ�����һ���������ؽ���*/
typedef struct tagVgmCtxLocks
{
    VOS_RW_LOCK_S  stLockListenConn;    /*ȫ�ֻ�ȡ�����ڵ���Ϣ����*/
}VGM_CTX_LOCK_S;


/*ҵ��ʹ�ܿ���*/
typedef struct tagVgmBizEnable
{
    UINT32  uiNemEnable;
    UINT32  uiPfmEnable;
    UINT32  uiTcmEnable;
}VGM_CTX_BIZENABLE_S;

/*ȫ�ֵ��������ص���������Ϣ*/
struct tagVgmCtx
{
    /*�������ݵĶ�Ӧ����*/
    VGM_CTX_LOCK_S           stVTLocks;          
    /*���������ص�ID*/
    ULONG                    ulVtID;                
    /*���������ص�����*/
    CHAR                     acVtName[VGM_VTNAME_MAXLEN];    
    /*�����Ľӿ�����*/
    CHAR                     acIfName[VOS_IFNAME_MAX_LEN];  
    /*���������صĶ�������*/
    CHAR                     acVtHostUrl[VGM_HOSTNAME_MAXLEN];   
    /*���������ص�IP��ַ: ������*/
    UINT32                   ulVTIP;          
    /*���������صĶ˿ں�*/
    UINT32                   ulVTPort;        
    /*���������ص�UDPS IP��ַ*/
    UINT32                   ulUdpsIP;          
    /*���������ص�UDPS �˿ں�*/
    UINT32                   ulUdpsPort;        
    /*ҵ���ؾ�������*/
    UINT32                   uiBizType;  
    /*�������ص�ҵ��ʹ�ܿ���*/
    VGM_CTX_BIZENABLE_S      stBizEn;
    /*���������ص�SSL����*/
    //UTL_SSL_CTX_CONFIG_S     *pstSSLCfg;      
    /*��ǰ������SSL�Ľڵ�*/
    VGM_SSL_LISTEN_CONN_S    *pstSslListenConn;
    /*��ǰ������UDPS�Ľڵ�, ���������߳��ˣ�ͳһ����*/
    //VGM_UDPS_LISTEN_CONN_S   *pstUdpListenConn;
    /*�����ص�ַ�ص�����*/
    //VGM_NEMPOOL_CONFIG_S     stPoolConf[VGM_CTX_NEMPOOL_SIZE];
    /*�����ӿ�����,������������ת��*/
    ULONG                    ulInnerIfIndex;
    /*���������Դ�����64����ͥ*/
    //VGM_FMINFO_CTX_S        *pstFmInfo[VGM_CTX_FMINFO_SIZE];

};

/*���д������������ص�����*/
typedef struct tagVgmCfgCtx{

    /*�����������ñ��������,���Ա���VGM_CFG_ARRAY_NUMS�����ص�������Ϣ*/
    VGM_CTX_S        *pastVgCfgArray[VGM_CFG_ARRAY_NUMS];   /*ʹ��ָ�������ʡ�ռ�*/
    /*��ǰ�Ѿ����ڵ�������������*/
    ULONG             ulCurrentNums;
}VGM_CFG_CTX_S;

ULONG   VGM_CFG_GetFreeVTID();

VGM_CTX_S *VGM_CFG_GetVTCtxByVTID(ULONG ulVtId);

VOID    VGM_CFG_DelVTCtxByVTID(ULONG ulVtId);

LONG    VGM_CFG_SetVTCtxByVTID(VGM_CTX_S *pstVgCtx, ULONG ulVtId);

LONG    VGM_CFG_CtxInit();

VOID    VGM_CFG_CtxUnInit();






