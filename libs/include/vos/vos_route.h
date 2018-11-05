/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_route.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��6��28��
  ����޸�   :
  ��������   : ϵͳ·�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��6��28��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

#define VOS_RTF_UP                        0x0001      /*ioctl��ǣ���ʾ���õ�*/
#define VOS_RTF_GATEWAY             0x0002      
#define VOS_RTF_HOST                    0x0004

#define VOS_PATH_PROCROUTE        "/proc/net/route"
#define VOS_DEVNAME_LEN              32                  /*�豸�ӿ����Ƴ���*/
#define VOS_ROUTETBL_SIZE           32                  /*��ϵͳ·�ɱ���������*/


/*·�ɱ���*/
typedef struct VOSRouteEntry
{
    VOS_IPADDR_T      uiDstip;                /*Ŀ��IP��ַ*/
    VOS_IPADDR_T      uiMask;                /*����*/
    VOS_IPADDR_T      uiGateway;          /*��Ŀ�ĳ��ӿ����ص�ַ*/
    UINT32      ulFlags;                          /*����Ƿ����*/
    UINT32      uiMetric;                         /*���ȼ�*/
    UINT32      uiMtu;                            /*��·�ɽӿڵ�MTU*/
    CHAR        acDevName[VOS_DEVNAME_LEN];     /*�ýӿڵ�����*/
}VOS_ROUTE_ENTRY_S;

/*��ϵͳ��·�ɱ�*/
typedef struct tagVOSSysRouteTable
{
    VOS_ROUTE_ENTRY_S   astArrayRouteTbl[VOS_ROUTETBL_SIZE];
}VOS_SYS_ROUTE_TBL_S;

INT32   VOS_GetCurrentRouteTable(VOS_SYS_ROUTE_TBL_S *pstRouteTbl);

INT32   VOS_GetDefaultGatewayRouteEntry(VOS_ROUTE_ENTRY_S *pstRouteEntry);

INT32   VOS_GetDefaultGateway(VOS_IPADDR_T *puiDeaultGateway);

LONG    VOS_GetDstIpBestRoute(VOS_IPADDR_T uiDstIp, VOS_ROUTE_ENTRY_S *pstRouteEntry);

LONG    VOS_RouteEntryAdd( VOS_IPADDR_T uiDstip,
                                                   VOS_IPADDR_T uiMask,
                                                   VOS_IPADDR_T uiGateway,
                                                   UINT32   uiMetric,
                                                   CHAR *pcifName);

LONG VOS_RouteEntryDel( VOS_IPADDR_T uiDstip,
                                                  VOS_IPADDR_T uiMask,
                                                  VOS_IPADDR_T uiGateway);


//-----------------------------ARP ����: /proc/net/arp----------------------------

#define VOS_PATH_PROCARP        "/proc/net/arp"
#define VOS_ARPTBL_SIZE           128                  /*��ϵͳARP��֧�ֵ���������*/

/*·�ɱ���*/
typedef struct VOSARPEntry
{
    CHAR         acIpAddr[VOS_IPV4STR_MAXLEN];             /*����IP��ַ*/
    UINT32      uiHwType;                                              /*HW Type*/
    UINT32      uiFlags;         
    CHAR         acMacAddr[32];          /*��IP��Ӧ��Mac��ַ�ַ���*/
    CHAR        acDevName[VOS_DEVNAME_LEN];     /*�ýӿڵ�����*/
}VOS_ARP_ENTRY_S;

/*��ϵͳ��ARP��*/
typedef struct tagVOSSysARPTable
{
    VOS_ARP_ENTRY_S   astArrayArpTbl[VOS_ARPTBL_SIZE];
}VOS_SYS_ARP_TBL_S;

