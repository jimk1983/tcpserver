/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : pfw_ctrl.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2017��4��29��
  ����޸�   :
  ��������   : �˿�ת������ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/



#include "common.h"
#include "swm/swm_pub.h"
#include "upfile/upf_pub.h"


/*����������ʱ�����װ����*/
VOID UPF_Ctrl_MakeCommBizHeadLen(COM_IOBUF_S *pstIobuf, UINT32 uiLocalSockfd, UINT32 uiStreamLen)
{
    UPF_HEAD_S      *pstBizHead = NULL;
    
    if  (NULL == pstIobuf)
    {
        return;
    }

    pstBizHead = (UPF_HEAD_S *)(pstIobuf->pcData);

    VOS_Printf("upf ctrl make pstBizhead=%p", pstBizHead);
  
    return;
}

/*****************************************************************************
 �� �� ��  : UPF_CtrlHandler
 ��������  : ���ƴ���
 �������  : PFW_CONN_S *pstPFWConn    
             PFW_CTRL_INFO_S *pstCtrl  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��5��21��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 UPF_CtrlHandler(UPF_CONN_S *pstPFWConn)
{
    if ( NULL == pstPFWConn)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}



