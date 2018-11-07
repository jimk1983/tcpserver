/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vgm_ctx.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��2��
  ����޸�   :
  ��������   : VGateway�������������ʼ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��2��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include "common.h"
#include "vgm/vgm_pub.h"


/*****************************************************************************
 �� �� ��  : VGM_Ini_CtxTaskInit
 ��������  : �߳������ʼ��
 �������  : VOID *pvArg  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_TCP_CtxTaskInit(VOID *pvArg)
{
     /*ֱ�Ӵ������ؼ���*/
     if ( VOS_ERR == VGM_SSLConn_VTCreate(VGM_DEFAULT_PORT) )
     {
        VOS_Printf("vgm vt-create error!\n");
        return;
     }
     
     VOS_Printf("vgm tcp ctx task init ok");
}

/*****************************************************************************
 �� �� ��  : VGM_TCP_CtxTaskUnInit
 ��������  : �������ʱ����Դ�ͷ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_TCP_CtxTaskUnInit()
{

}


/*****************************************************************************
 �� �� ��  : VGM_TCP_CtxInit
 ��������  : TCP�߳����ؼ����������ʼ��ע��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VGM_TCP_CtxInit()
{
    /*������������������RCT_TASKINII_ARRYINDEX_TCP�У����һ���RCT_TYPE_TCP���͵��߳��Ͻ��г�ʼ��*/
    if ( VOS_ERR == RCT_API_EnvTaskInitRegister(VGM_TCP_CtxTaskInit, NULL, RCT_TYPE_DP_TCP,RCT_SUBTYPE_SINGLE, VGM_TCP_CtxTaskUnInit) )
    {
        VOS_Printf("VGM tcp ctx init register error!!");
        return VOS_ERR;
    }
    
    VOS_Printf("VGM Ctx init ........................OK!");
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VGM_TCP_CtxUnInit
 ��������  : ������Դ�ͷ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_TCP_CtxUnInit()
{
    VGM_TCP_CtxTaskUnInit();
}

/*****************************************************************************
 �� �� ��: VGM_CtxInit
 ��������  :    Vgateway�������ĳ�ʼ����������TCP��UDPS�����̵߳�����
                           ��Դ��ʼ���������ڱ��ļ���ʵ���ˣ���Ҫ��ʵ����Ϣ����
                           ��Ϣ�������Ƿֿ������ˣ���Ϊ��������ͬ���̣߳��������ǲ�һ����
                           ���TCP�������ĺ���Ϣ���Լ�UDPS�������ĺ���Ϣ�����ֿ����д�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��2��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VGM_CtxInit()
{
    /*������Դ��ʼ��*/
    if ( VOS_ERR == VGM_CFG_CtxInit() )
    {
        VOS_Printf("VGM cfg ctx init error!!");
        return VOS_ERR;
    }
    
    /*��ʼ��TCP�������߳�
    ע��: ���Ҫ���TCP�߳�,���Ҽ���ͬһ��socketfd, ��ôҪע��epoll��Ⱥ����:
    �������Ϊ����ģʽ����ô�����ö���߳�accept��������־�Ⱥ����
    ��ʱ�������߳̾Ͳ��ܲ���EPOLL�ļܹ��ˣ�������Ϣ�ܹ����Կ�����
    (��Ҫ��Ϊ����߲���)*/
    if ( VOS_ERR ==  VGM_TCP_CtxInit() )
    {
        VGM_CFG_CtxUnInit();
        VOS_Printf("VGM udps ctx init error!!");
        return VOS_ERR;
    }
    
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VGM_CtxUninit
 ��������  : ������������Դ�ͷ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��2��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_CtxUnInit()
{
    VGM_TCP_CtxUnInit();
    VGM_CFG_CtxUnInit();
}
