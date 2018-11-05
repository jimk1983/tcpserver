/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_sock.h
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : SOCK��װ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef _VOS_SOCK_H_
#define _VOS_SOCK_H_

//Create TCP type socket , AF_INET
#define VOS_SOCK_TYPE_TCP   0 
//Create UDP type socket , AF_INET
#define VOS_SOCK_TYPE_UDP   1
//Create RAW type socket , AF_INET
#define VOS_SOCK_TYPE_RAW   2
//Create ARP type socket , PF_INET
#define VOS_SOCK_TYPE_ARP   3

//INVALID Socket defined
#define VOS_SOCKET_INVALID  0

//Socket Status
#define VOS_SOCK_OK                             0
#define VOS_SOCK_ERROR                      -1
#define VOS_SOCK_EWOULDBLOCK         -2
#define VOS_SOCK_TCP_CONNECTING     -3
#define VOS_SOCK_TCP_CONNECTED       -4
#define VOS_SOCK_SSL_CONNECTING      -5
#define VOS_SOCK_SSL_ACCEPTING         -6
#define VOS_SOCK_SSL_ACCEPTED           -7
#define VOS_SOCK_SSL_EWOULDBLOCK   -8
#define VOS_SOCK_SSL_RECONNECTING  -9
#define VOS_SOCK_SSL_ERROR               -10
#define VOS_SOCK_FINISH                      -11
#define VOS_SOCK_SSL_CONNECTED       -12

//Socket windows size
#define VOS_SOCK_SNDWINSIZE         65535
#define VOS_SOCK_RCVWINSIZE         65535

/*MacOS���������Ϊ6M, ��˶�λ6M�� ������ô���ԭ����
�����㹻��Ļ��棬��ֹ127.0.0.1���ڲ����ͳ��ֶ���*/
#define VOS_SOCK_MSGUDP_RCVWINSIZE    6*1024*1024   

#define VOS_SOCK_IPADDR_LEN         16

/*����ϵͳsocket�ţ�linux�Ǵ�С�����������ӵ�*/
#define VOS_SOCK_FDSIZE_MAX     20000

#define VOS_SOCK_LOCALHOST_STR     "127.0.0.1"


#define VOS_SOCK_INVALID        -1

LONG VOS_SOCK_Init();
LONG VOS_SOCK_UnInit();
LONG VOS_SOCK_Send(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *peErrorStatus);
LONG VOS_SOCK_SendTo(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, CHAR *pcPeerAddress, ULONG ulPeerPort, LONG *peErrorStatus);
LONG VOS_SOCK_SendToN(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, VOS_IPADDR_T uiPeerAddress, ULONG ulPeerPort, LONG *plErrorStatus);

LONG VOS_SOCK_Recv(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *peErrorStatus);
LONG VOS_SOCK_Recvfrom(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, CHAR *pcPeerAddress, ULONG *ulPeerPort, LONG *peErrorStatus);
LONG VOS_SOCK_RecvfromN(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, VOS_IPADDR_T *puiPeerAddress, ULONG *pulPeerPort, LONG *plErrorStatus);
LONG VOS_SOCK_Bind(LONG lSockfd, CHAR *pcLocalAddress, ULONG ulPort);
LONG VOS_SOCK_Listen(LONG lSockfd, ULONG ulNum);
LONG VOS_SOCK_Accept(LONG lListenfd, CHAR *pcClientAddr, USHORT *pusClientPort, LONG *peErrorStatus);
LONG VOS_SOCK_Connect(LONG lSockfd, CHAR *pcServAddr, USHORT usServPort, LONG *peErrorStatus);
LONG VOS_SOCK_GetHostSockAddr(INT32 lSockfd, CHAR *pcHostAddress, UINT32 *pulPort);
LONG VOS_SOCK_GetPeerSockAddr(LONG lSockfd, CHAR *pcPeerAddress, UINT32 *pulPort);
LONG VOS_SOCK_Create(ULONG ulType);
LONG VOS_SOCK_Close(LONG lSockfd);
LONG VOS_SOCK_Shutdown(LONG lSockfd);
LONG VOS_SOCK_SetOption(LONG lSockfd);
LONG VOS_SOCK_UDPSetOption(LONG lUdpSockfd);
LONG VOS_SOCK_SetBPFOption(LONG lSockfd);
LONG VOS_SOCK_SetProMiscMode(LONG lSockfd, CHAR *pcIfName);

LONG VOS_SOCK_RAW_SendTo(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, CHAR *acIfName, LONG *plErrorStatus);
LONG VOS_SOCK_RAW_Recvfrom(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *plErrorStatus);

VOS_IPADDR_T  VOS_SOCK_GetHostIpByName(CHAR *acDomain);
LONG VOS_SOCK_ConnectNetAddr(LONG lSockfd, VOS_IPADDR_T uiIpaddr, USHORT usServPort, LONG *plErrorStatus);

#endif

