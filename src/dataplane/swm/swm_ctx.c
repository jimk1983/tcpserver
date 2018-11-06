/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_ctx.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : SWM����ģ���������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    
#include "common.h"
#include "swm/swm_pub.h"


/*****************************************************************************
 �� �� ��  : Swm_CtxMsgHandler
 ��������  : switchҵ��ʶ���л�ģ�����Ϣ����
 �������  : CHAR *pcMgtData  ---���յ�����Ϣ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��31��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID Swm_CtxMsgHandler(RCT_MSG_HEAD_S *pstHead, CHAR *pcMgtData, UINT32 uiLen)
{
    ULONG   ulMsgType = SWM_MSGTYPE_UNKNOW;
    if ( NULL == pstHead
        || NULL == pcMgtData)
    {
        VOS_Printf("param error!");
        return;
    }

    /*��ȡ��Ϣ��*/
    ulMsgType = *((ULONG *)pcMgtData);

    VOS_Printf("SWM ctx handler msg type=%d!", ulMsgType);
   
    /*���ش�����ص���Ϣ*/
    switch(ulMsgType)
   {
        /*���SSLҵ��ڵ�*/
        case SWM_MSGTYPE_ADDTLSCONN:
            if ( VOS_ERR == SWM_TLS_ConnCreate((SWM_MSG_ADDCONN_S *)pcMgtData) )
            {
                VOS_Printf("Vgm listen swm conn-node create error!");
            }
            break;
        default:
            break;
    }
}

/*****************************************************************************
 �� �� ��  : SWM_Ini_CtxInit
 ��������  : switchҵ��ʶ���л�ģ���ʼ��ע��
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
VOID SWM_CtxTaskInit(VOID *pvArg)
{
   
    /*ҵ��ͨ��ע�ᴴ��, �������ʼ����Ҫ��NEM/TCM��֮ǰ����*/
    if ( VOS_ERR == SWM_Biz_ChannelMatchInit()  )
    {
        VOS_Printf("SWM channel match init error!!");
        return;
    }

    /*ע����Ϣ���մ�����*/
    if(VOS_ERR == RCT_API_MgtHandlerRegister(RCT_TYPE_DP_SSL, RCT_SUBTYPE_SSL_SWM, "SWM",  Swm_CtxMsgHandler))
    {
        VOS_Printf("SWM mgt register error!!");
        SWM_Biz_ChannelMatchUnInit();
        return;
    }
}
    
/*****************************************************************************
 �� �� ��  : SWM_Ini_CtxTaskUnInit
 ��������  : SWM��ҵ��������Դ�ͷ�
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
VOID SWM_CtxTaskUnInit()
{    
    SWM_Biz_ChannelMatchUnInit();

    /*ע�᱾�̵߳���Ϣ������Ϣ*/
    RCT_API_MgtHandlerUnRegister(RCT_TYPE_DP_SSL, RCT_SUBTYPE_SSL_SWM); 
}


/*****************************************************************************
 �� �� ��  : SWM_Ini_CtxInit
 ��������  : switchҵ��ʶ���л�ģ���ʼ��ע��
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
LONG SWM_CtxInit()
{
    /*��ע�������ĳ�ʼ��*/
    if ( VOS_ERR == RCT_API_EnvTaskInitRegister(SWM_CtxTaskInit, NULL, RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_SWM,  SWM_CtxTaskUnInit) )
    {
        VOS_Printf("Swm ctx inti register error!!");
        return VOS_ERR;
    }
    
    VOS_Printf("SWM init ...........................OK!");
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : SWM_Ini_CtxUnInit
 ��������  : �����ʼ��
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
VOID SWM_CtxUnInit()
{
    RCT_API_EnvTaskInitUnRegister(RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_SWM);
}


