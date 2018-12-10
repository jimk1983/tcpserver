/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vgm_cfg.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��3��
  ����޸�   :
  ��������   : Vgateway��������Ϣ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    
    
#include "common.h"
#include "vgm/vgm_pub.h"

/*�����������ص���Ϣ����Ҫͨ��set/get�����úͻ�ȡ
������ֱ��extern����������Ϣ*/
VGM_CFG_CTX_S  *g_pstVgmCfgCtx   =   NULL;       /*��Ϊȫ�ֵı���*/


/*****************************************************************************
 �� �� ��  : VGM_CFG_GetFreeVTID
 ��������  : �ҵ����е���������ID
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
ULONG VGM_CFG_GetFreeVTID()
{
    ULONG ulIndex = 0;

    if ( NULL == g_pstVgmCfgCtx )
    {
        return  VOS_ERR;
    }
        
    for ( ulIndex =0; ulIndex < VGM_CFG_ARRAY_NUMS; ulIndex++ )
    {
        /*�ҵ����е�ָ��*/
        if ( NULL == g_pstVgmCfgCtx->pastVgCfgArray[ulIndex] )
        {
            return ulIndex;
        }
    }
    
    return VOS_ERR;    
}

/*****************************************************************************
 �� �� ��  : VGM_CFG_GetVTCtxByVTID
 ��������  : ��ȡ��Ӧ��VTID�������ص�����������
 �������  : ULONG ulVtId  
 �������  : ��
 �� �� ֵ  : VGM_CTX_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VGM_CTX_S *VGM_CFG_GetVTCtxByVTID(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId )
    {
        VOS_Printf("param error!");
        return NULL;
    }
    
    return g_pstVgmCfgCtx->pastVgCfgArray[ulVtId];
}

/*****************************************************************************
 �� �� ��  : VGM_CFG_DelVTCtxByVTID
 ��������  : ɾ������
 �������  : ULONG ulVtId  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_CFG_DelVTCtxByVTID(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId )
    {
        VOS_Printf("param error!");
        return;
    }

    if ( NULL != g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]  )
    {
        VOS_Free((CHAR *)g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]);
        g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]= NULL;
    }
    g_pstVgmCfgCtx->ulCurrentNums--;
    
    return ;
}


/*****************************************************************************
 �� �� ��:  VGM_CFG_SetVTCtxByVTID
 ��������  :    ����VTCtx
 �������  :    VGM_CTX_S *pstVgCtx  --�����õ�CTX
            ULONG ulVtId               --���е�VTID
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VGM_CFG_SetVTCtxByVTID(VGM_CTX_S *pstVgCtx, ULONG ulVtId)
{
    if ( NULL == g_pstVgmCfgCtx 
        || VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }
    
    if(NULL !=  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        VOS_Printf("vtid ctx has exist!");
        return VOS_ERR;
    }

    g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] = pstVgCtx;
    g_pstVgmCfgCtx->ulCurrentNums++;

    return VOS_OK;
}



/*****************************************************************************
 �� �� ��  : VGM_CFG_GetCurrentConnNums
 ��������  : ��ȡ��ǰ���ص�������
 �������  : VGM_CTX_S *pstVgCtx  
             ULONG ulVtId         
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��12��6��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
ULONG VGM_CFG_GatewayConnGetNums(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        return VOS_ERR;
    }
    
    if(NULL ==  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        return VOS_ERR;
    }
    
    return g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]->ulConnNums;
}

/*****************************************************************************
 �� �� ��  : VGM_CFG_GatewayConnIncrement
 ��������  : ���ӵ�ǰ���ص�����ͳ����
 �������  : ULONG ulVtId  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��12��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_CFG_GatewayConnIncrement(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        return;
    }
    
    if(NULL ==  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        return;
    }
    
    VOS_InterlockedIncrement(&g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]->ulConnNums);
}


/*****************************************************************************
 �� �� ��  : VGM_CFG_GatewayConnDecrement
 ��������  : ɾ����ǰ���ص�����ͳ����
 �������  : ULONG ulVtId  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��12��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_CFG_GatewayConnDecrement(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        return;
    }
    
    if(NULL ==  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        return;
    }
    
    VOS_InterlockedDecrement(&g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]->ulConnNums);
}


/*****************************************************************************
 �� �� ��  : VGM_CFG_CtxInit
 ��������  : ��������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VGM_CFG_CtxInit()
{
    ULONG ulIndex = 0;

    if ( NULL != g_pstVgmCfgCtx )
    {
        return VOS_OK;
    }
    
    /*��������*/
    g_pstVgmCfgCtx = (VGM_CFG_CTX_S *)VOS_Malloc(VGM_MID_SID_CFG,  sizeof(VGM_CFG_CTX_S));

    if ( NULL == g_pstVgmCfgCtx )
    {
        VOS_Printf("malloc error!");
        return VOS_ERR;
    }
    
    /*��ʼ��ָ������*/
    for (ulIndex =0; ulIndex < VGM_CFG_ARRAY_NUMS; ulIndex++)
    {
        g_pstVgmCfgCtx->pastVgCfgArray[ulIndex] = NULL;
    }

    g_pstVgmCfgCtx->ulCurrentNums = 0;
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VGM_CFG_CtxUnInit
 ��������  : ��Դ�ͷţ��ڲ�Ҫ���û��Լ���֤
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��3��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VGM_CFG_CtxUnInit()
{
    if ( NULL == g_pstVgmCfgCtx )
    {
        VOS_Free((CHAR *)g_pstVgmCfgCtx);
        return;
    }
}


