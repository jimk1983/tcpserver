/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : main.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��5��
  ����޸�   :
  ��������   : ����������
  �����б�   :
              main
  �޸���ʷ   :
  1.��    ��   : 2018��11��5��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#include "common.h"
#include "init.h"

int main()
{
    VOS_SM_T stVsmStop={0};
    
    /*ֹͣ�ź�*/
    if( VOS_ERR == VOS_SM_Init(&stVsmStop) )
    {
        VOS_PrintDebug("VOS Semaphore stop error");
        return -1;
    }
    
    /*1. ����˵Ļ�����ʼ��*/
    if( VOS_ERR == Main_EnvInit())
    {
        VOS_Printf("Main enviroment initialize error!\n");
        return VOS_ERR;
    }

    /*2. ��������*/
    if( VOS_ERR == RCT_API_EnvRun())
    {
        VOS_Printf("Reactor enviroment running error!\n");
        return VOS_ERR;
    }
    
    /*˳�����ʹ��VOS�ź���*/
    VOS_SM_P(&stVsmStop,0);
    Main_EnvUnInit();
    VOS_SM_Destroy(&stVsmStop);
    VOS_Printf("Main pthread End!\n");
    
    return VOS_OK;
}
