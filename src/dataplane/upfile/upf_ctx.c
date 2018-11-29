/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : pfw_ctx.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2017��4��29��
  ����޸�   :
  ��������   : portforward
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include "common.h"
#include "swm/swm_pub.h"
#include "fsm/fsm_pub.h"
#include "upfile/upf_pub.h"

UPF_CTX_S *g_pstUPFCtx = NULL;


/*****************************************************************************
 �� �� ��  : UPF_CtxBizMatch
 ��������  :    UpFile �ļ�����ҵ��ʶ��,ʶ�����½�һ��UPF��ҵ��ܵ��ڵ�
 �������  :    SWM_BIZ_CHANNEL_S *pstBizChannel  --ҵ��ͨ�����ܵ�������
            CHAR *pcData                      --����
            ULONG ulDataLen                   --���ݳ���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
ULONG UPF_CtxBizMatch(SWM_BIZ_CHANNEL_S *pstBizChannel, CHAR *pcData, ULONG ulDataLen)
{
    SWM_BIZ_HEAD_S *pstUpfHead = NULL;

    if ( NULL == pstBizChannel
        || NULL == pcData )
    {
        VOS_Printf("param error");
        return EMPTO_BIZTYPEID_UNKNOW;
    }

    pstUpfHead = (SWM_BIZ_HEAD_S *)pcData;

    /*ҵ��������ƥ��*/
    if ( UPF_PROTO_MARK1 == VOS_ntohl(pstUpfHead->uiMark1)
        && UPF_PROTO_MARK2 == VOS_ntohl(pstUpfHead->uiMark2) ) 
    {
        /*ƥ��󣬴�����ͨ����UPF�ڵ�*/
        if ( VOS_ERR == UPF_Conn_Create(pstBizChannel) )
        {
            VOS_Printf("pfw connect node create error!");
            return EMPTO_BIZTYPEID_UNKNOW;
        }
        pstBizChannel->ulPipeNum++;
        VOS_Printf("pfw biz type has been matched and create nem node OK!");
        return EMPTO_BIZTYPEID_UPF;
    }
    
    return EMPTO_BIZTYPEID_UNKNOW;
}



/*****************************************************************************
 �� �� ��  : UPF_Ini_CtxCreate
 ��������  : UPF��������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG UPF_CtxCreate()
{
    SWM_BIZ_MATCH_CTX_S  stBizCtx = {0};

    /*�ж��SSL�̣߳�����Ҫ����жϣ����ܴ������*/
    if (NULL == g_pstUPFCtx)
    {
        g_pstUPFCtx = (UPF_CTX_S *)VOS_Malloc(PFW_MID_SID_INI, sizeof(UPF_CTX_S));
        if ( NULL == g_pstUPFCtx  )
        {
            VOS_Printf("param error");
            return VOS_ERR;
        }

        VOS_Mem_Zero((CHAR *)g_pstUPFCtx, sizeof(UPF_CTX_S));

        /*ע��ǰ��ֵ*/
        stBizCtx.ulBizType = EMPTO_BIZTYPEID_UPF;
        stBizCtx.pvFunc =  (VOID*)UPF_CtxBizMatch;
        stBizCtx.pvCtx   = g_pstUPFCtx;

        /*ҵ��ʶ��ע�ᣬ��SWMģ����ʶ��*/    
        if ( VOS_ERR == SWM_Biz_ChannelMatchRegister(&stBizCtx))
        {
            VOS_Printf("swm biz channel match register error");
            VOS_Free((CHAR *)g_pstUPFCtx);
            return VOS_ERR;
        }
    }
    
   
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : UPF_CtxRelease
 ��������  : �ͷ�������
 �������  : UPF_CTX_S *pstCtx  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID UPF_CtxRelease()
{
    if(NULL != g_pstUPFCtx)
    {
        (VOID)SWM_Biz_ChannelMatchUnRegister(EMPTO_BIZTYPEID_UPF);
        VOS_Free((CHAR *)g_pstUPFCtx);
    }
}


/*****************************************************************************
 �� �� ��  : UPF_CtxTaskInit
 ��������  : PFW��������Դ��ʼ��
 �������  : VOID *pvArg  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID UPF_CtxTaskInit(VOID *pvArg)
{
    /*ҵ�������ĳ�ʼ��*/
    if ( VOS_ERR == UPF_CtxCreate() )
    {
        VOS_Printf("UPF ctx create!!");
        return;
    }

    VOS_Printf("UPF ctx task initialize OK!");
}


/*****************************************************************************
 �� �� ��  : UPF_CtxTaskUnInit
 ��������  : ��Դ�ͷ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID UPF_CtxTaskUnInit(VOID *argv)
{
    
    UPF_CtxRelease();
}

/*****************************************************************************
 �� �� ��  : PFW_CtxInit
 ��������  : �˿�ת��ģ��ĳ�ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG UPF_CtxInit()
{
    /*��ע�������ĳ�ʼ��*/
    if ( VOS_ERR == RCT_API_EnvTaskInitRegister(UPF_CtxTaskInit, NULL, RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_UPF, UPF_CtxTaskUnInit) )
    {
        VOS_Printf("UPF ctx inti register error!!");
        return VOS_ERR;
    }
    
    VOS_Printf("UPF Ctx init .......................OK!");
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : PFW_CtxUnInit
 ��������  : ȥ��ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID UPF_CtxUnInit()
{
    RCT_API_EnvTaskInitUnRegister(RCT_TYPE_DP_SSL,RCT_SUBTYPE_SSL_UPF);
}

