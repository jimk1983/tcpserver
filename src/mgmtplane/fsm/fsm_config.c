/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, ���޹�˾

 ******************************************************************************
  �� �� ��   : fsm_config.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��9��
  ����޸�   :
  ��������   : �ļ���ص����ù���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
    
#include "common.h"
#include "fsm/fsm_pub.h"

const FSM_CONF_INFO_S g_starryFsmConfig[]=
{
    {
        "testSmall.xml"
    },
    {
        "testBig.xml"
    },
    {
        "TerminalCollector.exe"
    },
};

/*****************************************************************************
 �� �� ��  : FSM_Conf_GetFileNums
 ��������  : ��ȡ��ǰ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32   FSM_Conf_GetFileNums()
{
    INT32 uiNums = VOS_ERR;

    uiNums = sizeof(g_starryFsmConfig)/sizeof(FSM_CONF_INFO_S);

    return uiNums;
}


/*****************************************************************************
 �� �� ��  : FSM_Conf_GetFileNameByIndex
 ��������  : ��ȡ��ǰ�������ļ�
 �������  : UINT32 iIndex  
 �������  : ��
 �� �� ֵ  : CHAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
const CHAR   *FSM_Conf_GetFileNameByIndex(UINT32 iIndex)
{
    const CHAR *pcFileName = NULL;
    
    if ( iIndex >= FSM_Conf_GetFileNums()  )
    {
        return NULL;
    }

    pcFileName = g_starryFsmConfig[iIndex].acFileName;
       
    return pcFileName;
}

