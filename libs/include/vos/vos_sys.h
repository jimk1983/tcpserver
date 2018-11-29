/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_sys.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��5��23��
  ����޸�   :
  ��������   : ϵͳ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��5��23��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/


#if VOS_PLAT_LINUX
typedef pthread_t VOS_THREAD_T;
#elif VOS_PLAT_WIN
#ifndef HANDLE
#define HANDLE (VOID *)
#endif
typedef HANDLE  VOS_THREAD_T;
#endif

VOS_THREAD_T VOS_Thread_Create(CHAR *pcName, VOID *(* pfvThread)(VOID *arg), VOID *pvArg, INT32 iStacksize, INT32 iPrio);

LONG VOS_GetLastError();

CHAR *VOS_GetStrLastError();


