/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_ifnet.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��4��17��
  ����޸�   :
  ��������   : ����ӿ�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��4��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/*�����ֽ����ַ*/
typedef UINT32    VOS_IPADDR_T;

#define VOS_IPV4ADDR_LEN            4       /*IPV4��ַ����*/
#define VOS_IPV4MACADDR_LEN         6       /*MAC��ַ����*/
/*Ӳ����ַ*/
typedef UCHAR   MACADDR[6];

#define VOS_IPV4ADDR_STRLEN          16

/*�����ӿ�����*/
#define VOS_IFNAME_MAX_LEN     16

/*���������*/
#if VOS_PLAT_LINUX
#define VOS_ATTRIBATE_PCKALGN    __attribute__((packed, aligned(1)))
#elif VOS_PLAT_WIN
#define VOS_ATTRIBATE_PCKALGN    #program pack(1)
#endif

#define VOS_IPV4_ETH_LEN            14          /*��·�㳤��*/
#define VOS_IPV4_IPHEAD_LEN       20          /*IPV4������ѡ��ͷ����*/



INT32   VOS_IPV4StrToN(CHAR *pcStr);

INT32   VOS_NToIPV4Str(UINT32 iNAddr, CHAR *pcStr);

LONG    VOS_GetMACAddrByIfName(CHAR *pcIfName, UCHAR *pcMacAddr);

LONG    VOS_GetIPAddrByIfName(CHAR *pcIfName, VOS_IPADDR_T *piIPAddr);

LONG    VOS_GetIPMaskByIfName(CHAR *pcIfName, VOS_IPADDR_T *piIPMask);

LONG    VOS_GetIndexByIfName(CHAR *pcIfName, UINT32 *puiIndex);

//LONG    VOS_GetGateway(CHAR *pcGateway);

LONG    VOS_GetDNSServerIp(VOS_IPADDR_T uiDNSSev[4]);


USHORT  VOS_IPV4Checksum(void *buf, UINT32 size);

UCHAR        *VOS_mtoa (UCHAR *pucMacAddr);
UINT32      VOS_htonl(UINT32 ulHValue);
UINT32      VOS_ntohl(UINT32 ulHValue);
USHORT     VOS_htons(USHORT usHValue);
USHORT     VOS_ntohs(USHORT usHValue);
UINT32      VOS_aton_s(CHAR *pcStr);
UINT32      VOS_aton(const CHAR *pcStr);
UINT32      VOS_ntoa(UINT32 iNAddr, CHAR *pcStr);

