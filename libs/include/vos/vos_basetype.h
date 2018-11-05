/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_basetype.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��2��
  ����޸�   :
  ��������   : ϵͳ�Ļ������Ͷ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��2��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_BASETYPE_H_
#define _VOS_BASETYPE_H_


/*char ��ʾ�޷��ŵ�����, Unicode��ռ2���ֽ�*/
#ifndef CHAR
typedef char    CHAR;
#endif

/*1���ֽ�, �з��ŵ�8λ*/
#ifndef BYTE
typedef signed char   BYTE;
#endif
    
#ifndef SHORT
typedef short   SHORT;
#endif
       
#ifndef LONG
typedef long     LONG;
#endif
    
#ifndef VOID
typedef void     VOID;
#endif
    
#ifndef UCHAR
typedef unsigned char   UCHAR;
#endif
    
#ifndef USHORT
typedef unsigned short  USHORT;
#endif

    
#ifndef ULONG
typedef unsigned long    ULONG;
#endif
    
#ifndef VULONG
typedef volatile unsigned long  VULONG;
#endif


#ifndef INT8
typedef signed char       INT8;
#endif
#ifndef UINT8
typedef unsigned char   UINT8;
#endif
#ifndef INT16
typedef short               INT16;
#endif
#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef INT32
typedef int                  INT32;
#endif
#ifndef UINT32
typedef unsigned int    UINT32;
#endif

#ifndef SIZE_T
typedef unsigned int    SIZE_T;
#endif
    
#ifndef BOOL
typedef unsigned char BOOL;
#endif
    
#ifndef VOID
#define VOID void
#endif
    
#ifndef NULL
#define NULL ((VOID*)0)
#endif

/*����: ֱ�ӳ��쳣ջ*/
#define VOS_COREDUMP        {char *ptr=0x00;*(ptr)=0x00;}
#define VOS_ASSERT(expr)     if(!(expr))VOS_COREDUMP


#ifndef VOS_OK
#define VOS_OK               0
#endif
    
#ifndef VOS_ERR
#define VOS_ERR             (~(0))
#endif

#define VOS_INVALID_VAL32   0xFFFFFFFF

#define VOS_INVALID_IPADDR  0xFFFFFFFF

/*ϵͳ����*/
#define VOS_SYS_ERR                          -1
/*��������*/
#define VOS_SYS_PARAM_INVALID       -2
/*��ʱ����*/
#define VOS_SYS_TIMEOUT                  -3
/*�ź�����ʱ*/
#define VOS_SYS_SM_TIMEOUT            -4
/*����*/
#define VOS_SYS_EWOULDBLOCK         -5
/*�Ѿ�����*/
#define VOS_SYS_EXIST         -6


#ifndef VOS_TRUE
#define VOS_TRUE 1
#endif
    
#ifndef VOS_FALSE
#define VOS_FALSE 0
#endif

    
 /*�Ƿ���У�0: ��ʾ����,1:��ʾ�ǿ���*/
#define VOS_IDLE    0
#define VOS_NIDLE   1
    
    
#ifndef PVOID
typedef VOID *PVOID;
#endif
    
#if VOS_PLAT_PC32
typedef int               	INTPTR;
#elif  VOS_PLAT_PC64
typedef long int        	INTPTR;
#endif
    
/*%llu*/
typedef unsigned long long  ULONG64;
typedef unsigned long int   UINT64;
    
/*%lld*/
typedef long long LONG64;
    
/*���*/
#define VOS_PLAT_BIG_ENDIAN     0

#ifndef DULONG
typedef struct tagDulong
{
    struct tagulong_ul
    {
        UINT32 ulHi_l;
        UINT32 ulLo_l;
    }x;
    struct tagulong_us
    {
    #if VOS_PLAT_BIG_ENDIAN
        USHORT usHiHi_s;
        USHORT usHiLo_s;
        USHORT usLoHi_s;
        USHORT usLoLo_s;
    #else
        USHORT usHiLo_s;
        USHORT usHiHi_s;
        USHORT usLoLo_s;
        USHORT usLoHi_s;
    #endif
    }y;    
#define ulHi         x.ulHi_l
#define ulLo        x.ulLo_l
}DULONG;
#endif
    
/*��DULONG תΪULONG64*/
#define VOS_DULONG_TO_ULONG64(dulSrc, ullDst64) (ullDst64) = ((ULONG64)((dulSrc).ulHi) << 32 | ((ULONG64)((dulSrc).ulLo)) );

/*��ULONGת��ΪDULONG*/
#define VOS_ULONG64_TO_DULONG(ullSrc64, dulDst) \
    (dulDst).ulHi  =  (UINT32)((ullSrc64)>>32;\
    (dulDst).ulLo =  (UINT32)((ullSrc64);


#endif
