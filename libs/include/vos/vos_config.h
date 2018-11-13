/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_config.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 
  ����޸�   :
  ��������   : ����ϵͳ�Ļ�������
  �����б�   :
  �޸���ʷ   :
******************************************************************************/
#ifndef _VOS_CONFIG_H_
#define _VOS_CONFIG_H_

/*�����ƽ̨��MACϵͳ*/
#define     VOS_PLAT_MAC             0
/*�����ƽ̨��Winϵͳ*/
#define     VOS_PLAT_WIN             0
/*�����ƽ̨��Linuxϵͳ*/
#define     VOS_PLAT_LINUX          1

/*�������32λϵͳ*/
#define     VOS_PLAT_PC32           0
/*�������64λϵͳ*/
#define     VOS_PLAT_PC64           1

/*С�˶���*/
#ifndef VOS_LITTLE_ENDIAN
#define VOS_LITTLE_ENDIAN     1234
#endif

/*��˶���*/
#ifndef VOS_BIG_ENDIAN
#define VOS_BIG_ENDIAN          4321
#endif 

/*ƽ̨���ֽ���,PCһ�㶼��С�˵�*/
#define VOS_BYTE_ORDER        VOS_LITTLE_ENDIAN

/*��������openssl���ͷ�ļ�*/
/*
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>
#include <openssl/pkcs12.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
*/

/*�������Ǹ���ϵͳ������ͷ�ļ�*/
#if VOS_PLAT_WIN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#elif VOS_PLAT_LINUX
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>

#include <netdb.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>

#include <linux/rtnetlink.h>    

#include <dirent.h>

/*BPF���*/
#include <linux/if_ether.h>
#include <net/if.h>
#include <linux/filter.h>

/*֧��pcap��չ*/
//#include <pcap/pcap.h>   //suse�и�ͷ�ļ�
//#include <pcap.h>

#define atomic_inc(x) __sync_add_and_fetch((x),1)   
#define atomic_dec(x) __sync_sub_and_fetch((x),1)   
#define atomic_add(x,y) __sync_add_and_fetch((x),(y))   
#define atomic_sub(x,y) __sync_sub_and_fetch((x),(y)) 

#elif VOS_PLAT_MAC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h> 
#include <math.h>

#endif

#endif


