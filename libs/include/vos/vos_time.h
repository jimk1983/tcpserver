/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_time.h
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳʱ���ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/


#ifndef _VOS_TIME_H_
#define _VOS_TIME_H_

typedef struct tagAbsTime
{
    struct timespec stAbsTime;
}VOS_ABS_TIME_S;

#define VOS_LOCAL_SYSTIME_BUFLEN        28

typedef struct tagLocalSysTime
{
    UINT32  uiYear;
    UINT32  uiMoth;
    UINT32  uiDay;
    UINT32  uiHour;
    UINT32  uiMinute;
    UINT32  uiSecond;
    UINT32  uiMiliSec;    
}VOS_LOCAL_SYSTEM_TIME_S;



VOID VOS_GetAbsTime(VOS_ABS_TIME_S *pstAbsTime, UINT32 ulTimeMiliSec);
VOID VOS_Sleep(ULONG ulSeconds);
VOID VOS_MSleep(ULONG ulMSleep);
VOID VOS_USleep(ULONG ulUSleep);

VOID VOS_GetLocalSystemTime(VOS_LOCAL_SYSTEM_TIME_S *pstSysTime);


#endif

