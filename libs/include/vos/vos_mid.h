/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_mid.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��7��26��
  ����޸�   :
  ��������   : ģ���MID�Ķ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��7��26��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/**********************RCTģ���TID����*********************************/
/*˵��: ��MID�Ķ�����Ҫ���������ڴ������λ����ʹ��*/
/*16-31: ��Ҫ������ģ�鶨��, ��λ0-15: ��ģ��ModuleID���µ���SubID����*/


/***************************ϵͳģ��0x00000000 ********************************/
/*VOS ��ϵͳ�ڴ�*/
#define VOS_MID                               0x00000000


/***************************RCT����ܹ�0x0001xxxx*****************************/
/*����ܹ���MID*/
#define RCT_MID                               0x00010000
#define RCT_MID_SID_SET(SID)         (RCT_MID  | SID )           
#define RCT_MID_SID_INI                 RCT_MID_SID_SET(0x0001)             /*0x00010001--��ʾINIģ��ʹ�õ��ڴ�*/
#define RCT_MID_SID_TSK                RCT_MID_SID_SET(0x0002)             /*0x00010002--��ʾTSKģ��ʹ�õ��ڴ�*/
#define RCT_MID_SID_EVT                RCT_MID_SID_SET(0x0003)             /*0x00010003--��ʾEVTģ��ʹ�õ��ڴ�*/
#define RCT_MID_SID_UTL                RCT_MID_SID_SET(0x0004)            

/***************************shareͨ�ù���0x0002xxxx***************************/
/*ͨ�ù���MID*/
#define COM_QUE_MID                               0x00020000
#define COM_QUE_MID_SID_SET(SID)         (COM_QUE_MID | SID )           
#define COM_QUE_MID_SID_INI                 COM_QUE_MID_SID_SET(0x0001)             /*0x00010001--��ʾQUEģ��ʹ�õ��ڴ�*/

/*TCPIP ģ��*/
#define TCPIP_MID                                   0x00020100                                 /*0x0003-�����ļ���01-TCPģ��*/
#define TCPIP_MID_SID_SET(SID)             (TCPIP_MID| SID )
#define TCPIP_MID_SID_INI                     TCPIP_MID_SID_SET(0x0001)         
#define TCPIP_MID_SID_PKT                    TCPIP_MID_SID_SET(0x0002)          

/**************************driversģ���MID********************************/
/*RawIp ģ��*/
#define RIP_MID                                       0x00030000
#define RIP_MID_SID_SET(SID)                (RIP_MID | SID )
#define RIP_MID_SID_INI                         RIP_MID_SID_SET(0x01)              /*0x00030001--��ʾINIģ��ʹ�õ��ڴ�*/

/*IFNETģ��*/
#define IFNET_MID                                    0x00030100
#define IFNET_MID_SID_SET(SID)              (IFNET_MID | SID )
#define IFNET_MID_SID                            IFNET_MID_SID_SET(0x01)              /*0x00030001--��ʾĬ��ʹ�õ��ڴ�*/

/*NCARDģ��*/
#define NCARD_MID                                    0x00030200
#define NCARD_MID_SID_SET(SID)              (NCARD_MID | SID )
#define NCARD_MID_SID                             NCARD_MID_SID_SET(0x01)              /*0x00030001--��ʾĬ��ʹ�õ��ڴ�*/



/**/
#define CLI_MID                                0x00040000
#define CLI_MID_SID_SET(SID)         (CLI_MID  | SID )           
#define CLI_MID_INI                         CLI_MID_SID_SET(0x01)             /*0x00010001--��ʾINIģ��ʹ�õ��ڴ�*/


/*****************************************/
/*VGM ģ��*/
#define VGM_MID                               0x10020000
#define VGM_MID_SID_SET(SID)         (VGM_MID| SID )
#define VGM_MID_SID_INI                 VGM_MID_SID_SET(0x0001)            /*0x10020001--��ʾINIģ��ʹ�õ��ڴ�*/
#define VGM_MID_SID_CFG                VGM_MID_SID_SET(0x0002)            /*0x10020002--��ʾCFGģ��ʹ�õ��ڴ�*/
#define VGM_MID_SID_SSL                VGM_MID_SID_SET(0x0003)
#define VGM_MID_SID_UDPS                VGM_MID_SID_SET(0x0004)


/*SWM ģ��*/
#define SWM_MID                               0x10030000
#define SWM_MID_SID_SET(SID)        (SWM_MID| SID )
#define SWM_MID_SID_INI                 SWM_MID_SID_SET(0x0001)          /*0x10030001--��ʾINIģ��ʹ�õ��ڴ�*/
#define SWM_MID_SID_TLS                 SWM_MID_SID_SET(0x0002)         /*0x10030002--��ʾTLSģ��ʹ�õ��ڴ�*/
#define SWM_MID_SID_BIZ                 SWM_MID_SID_SET(0x0003)          /*0x10030003--��ʾBIZģ��ʹ�õ��ڴ�*/

/*NEM ģ��*/
#define NEM_MID                               0x10040000
#define NEM_MID_SID_SET(SID)        (NEM_MID| SID )
#define NEM_MID_SID_INI                 NEM_MID_SID_SET(0x0001)          /*0x10040001--��ʾINIģ��ʹ�õ��ڴ�*/
#define NEM_MID_SID_CON               NEM_MID_SID_SET(0x0002)          /*0x10040002--��ʾCONģ��ʹ�õ��ڴ�*/
#define NEM_MID_SID_CTL               NEM_MID_SID_SET(0x0003)          /*0x10040003--��ʾCTLģ��ʹ�õ��ڴ�*/
#define NEM_MID_SID_SES               NEM_MID_SID_SET(0x0004)          /*0x10040004--��ʾSESģ��ʹ�õ��ڴ�*/
#define NEM_MID_SID_UDPS            NEM_MID_SID_SET(0x0005)          /*0x10040005--��ʾUDPSģ��ʹ�õ��ڴ�*/


/*PFW ģ��*/
#define PFW_MID                               0x10050000
#define PFW_MID_SID_SET(SID)        (PFW_MID| SID )
#define PFW_MID_SID_INI                 PFW_MID_SID_SET(0x0001)          /*0x10040001--��ʾINIģ��ʹ�õ��ڴ�*/
#define PFW_MID_SID_CON               PFW_MID_SID_SET(0x0002)          /*0x10040002--��ʾCONģ��ʹ�õ��ڴ�*/
#define PFW_MID_SID_CTL               PFW_MID_SID_SET(0x0003)          /*0x10040003--��ʾCTLģ��ʹ�õ��ڴ�*/
#define PFW_MID_SID_SES               PFW_MID_SID_SET(0x0004)          /*0x10040004--��ʾSESģ��ʹ�õ��ڴ�*/
#define PFW_MID_SID_INNER            PFW_MID_SID_SET(0x0005)          /*0x10040005--��ʾINNERģ��ʹ�õ��ڴ�*/


/*APY ģ��*/
#define APY_MID                               0x10060000
#define APY_MID_SID_SET(SID)        (APY_MID| SID )
#define APY_MID_SID_INI                 APY_MID_SID_SET(0x0001)          /*0x10040001--��ʾINIģ��ʹ�õ��ڴ�*/
#define APY_MID_SID_PKT               APY_MID_SID_SET(0x0002)          /*0x10040002--��ʾCONģ��ʹ�õ��ڴ�*/
#define APY_MID_SID_CON               APY_MID_SID_SET(0x0003)          /*0x10040002--��ʾCONģ��ʹ�õ��ڴ�*/


/*VDM ģ��*/
#define VDM_MID                               0x10070000
#define VDM_MID_SID_SET(SID)        (VDM_MID| SID )
#define VDM_MID_SID_INI                 VDM_MID_SID_SET(0x0001)        
#define VDM_MID_SID_MSG               VDM_MID_SID_SET(0x0002)          


/*AAA ģ��*/
#define AAA_MID                               0x10080000
#define AAA_MID_SID_SET(SID)        (AAA_MID| SID )
#define AAA_MID_SID_INI                 AAA_MID_SID_SET(0x0001)          
#define AAA_MID_SID_MSG               AAA_MID_SID_SET(0x0002)          


/*DBM ģ��*/
#define DBM_MID                           0x10090000
#define DBM_MID_SID_SET(SID)        (DBM_MID| SID )
#define DBM_MID_SID_INI                 DBM_MID_SID_SET(0x0001)          
#define DBM_MID_SID_MSG               DBM_MID_SID_SET(0x0002)          


/*PROXY ģ��*/
#define PROXY_MID                               0x100A0000
#define PROXY_MID_SID_SET(SID)        (PROXY_MID| SID )
#define PROXY_MID_SID_INI                 PROXY_MID_SID_SET(0x0001)          
#define PROXY_MID_SID_MSG               PROXY_MID_SID_SET(0x0002)          

/*VSNAT ģ��*/
#define VSNAT_MID                               0x100B0000
#define VSNAT_MID_SID_SET(SID)        (VSNAT_MID| SID )
#define VSNAT_MID_SID_INI                 VSNAT_MID_SID_SET(0x0001)          
#define VSNAT_MID_SID_MSG               VSNAT_MID_SID_SET(0x0002)          


