/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vgm_def.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��3��
  ����޸�   :
  ��������   : ��Щǰ���н���������ϵ�Ķ��壬�Լ�define
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    
/*���Դ��8���������ص���Ϣ*/
#define VGM_CFG_ARRAY_NUMS      8

/*Ĭ�ϼ���listen�Ķ�������*/
#define VGM_SSLLISTEN_DEFAULT_NUMS      20

/*֧�ֵĵ�ַ��������*/
#define VGM_CTX_NEMPOOL_SIZE        8

/*֧�ֵļ�ͥ��Ϣ������*/
#define VGM_CTX_FMINFO_SIZE        64


/*�������ͻ��˵�֤��У��*/
#define VGM_VTCLIENT_CERTVERIFY_DISABILE    0
/*�����ͻ��˵�֤��У��*/
#define VGM_VTCLIENT_CERTVERIFY_ENABLE      1

#define VGM_HOSTNAME_MAXLEN           16

/*����ͨ������*/
#define VGM_VTNAME_MAXLEN      16

/*UDPS�Ľ��ջص�*/
#define VDM_UDPS_RECVBUF_SIZE   72000

#define VDM_UDPPACK_MAXSIZE     65530

typedef struct tagVgmSSLListenConn VGM_SSL_LISTEN_CONN_S;

typedef struct tagVgmCtx VGM_CTX_S;

