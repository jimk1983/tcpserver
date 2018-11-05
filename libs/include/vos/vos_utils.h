/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_utils.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��2��
  ����޸�   :
  ��������   : ͨ�ù���ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��2��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/*----------------------------------------------*
 * ���õĺ궨��                                       *
 *----------------------------------------------*/
/*IPV4ȫ����ַ�ַ�����󳤶�*/
#define VOS_IPV4STR_MAXLEN 16
/*IPV4��ַ�����ַ�������*/
#define VOS_IPADDR_DOILEN  4
     
/*����32λ�����ַ�������*/
#define VOS_INT32STR_MAXLEN    12

/*һ�㳣���ļ����ĳ���*/
#define VOS_FILE_MAXLEN     256

/*��������*/
#define VOS_URL_MAXLEN      128

typedef VOID (*VOS_pfDataHandleCb)(CHAR *pvData, UINT32 uiDataSize);

/*----------------------------------------------*
 * ���õĻص���������                                       *
 *----------------------------------------------*/
/*�����Ļص���������1*/
typedef VOID (*VOS_CALLBACK)(VOID *pvData);

typedef VOID (*VOS_CALLBACK_UNINIT)();

typedef struct tagVOS_Callback VOS_CALLBACK_S;
struct tagVOS_Callback
{
      VOID *pvcbFunc;
      VOID *pvData;
};
#define VOS_CALLBACK_INIT(pstCallBack_, pfFunc_, pvUserData_)\
do{\
    (pstCallBack_)->pvcbFunc = (VOID *)pfFunc_;\
    (pstCallBack_)->pvData   = (VOID *)pvUserData_;\
}while(0);



/*�����Ļص���������2*/
typedef VOID (*VOS_ARGV_CALLBACK)(VOID *pvArgv1, VOID *pvArgv2);

typedef struct tagVOS_ArgvCallback VOS_ARGV_CALLBACK_S;

struct tagVOS_ArgvCallback
{
  VOID *pvcbFunc;
  VOID *pvArgv1;
  VOID *pvArgv2;
};

#define VOS_ARGV_CALLBACK_INIT(pstArgvCallBack_, pfFunc_, pvArgv1_, pvArgv2_)\
do{\
    (pstArgvCallBack_)->pvcbFunc = (VOID *)pfFunc_;\
    (pstArgvCallBack_)->pvArgv1  = (VOID *)pvArgv1_;\
    (pstArgvCallBack_)->pvArgv2  = pvArgv2_;\
}while(0);

BOOL VOS_CpuIsBigEndian();

ULONG VOS_GetSelfPId();


UINT32 VOS_atoi(CHAR *pcStr);
UINT32 VOS_itoa(CHAR *pcStr, UINT32 iVal);

USHORT VOS_Random16();
UINT32  VOS_Random32();


