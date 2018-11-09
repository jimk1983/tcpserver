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
INT32 UPF_CtrlHandler(UPF_CONN_S *pstUpfConn, COM_IOBUF_S *pstIobuf)
{ 
    SWM_BIZ_HEAD_S* pstBizHead = NULL;
    PUPF_HEAD_S     pstUpfHead = NULL; 
    CHAR*           pcPacket        = NULL;
    UINT32          uiPackLen        =0 ;
    LONG            lRet = 0;
    ULONG           uiCtrlCode = 0;
    
    if ( NULL == pstUpfConn
            || NULL == pstIobuf )
    {
        return VOS_ERR;
    }

    pcPacket   = COM_IOBUF_GETSAVED_DATA(pstIobuf);
    uiPackLen  = COM_IOBUF_GETSAVED_LEN(pstIobuf);
    
    pstBizHead = (SWM_BIZ_HEAD_S  *)pcPacket;

    pstUpfHead = (PUPF_HEAD_S)(pcPacket + SWM_BIZ_HEAD_LEN);

    uiCtrlCode = VOS_ntohl(pstUpfHead->uiCtrlCode);
    
    VOS_Printf("UPF conn upper :recv the iobuf=%p, upf conn=%p, uiPackLen=%d, pstBizHead=%p, acDeviceLab=%s!",  
        pstIobuf, pstUpfConn, uiPackLen, pstBizHead, pstUpfHead->acDeviceLab);
    
    switch(uiCtrlCode)
    {
        case UPSER_CTLCODE_KPVINFO_REQ:
            pstUpfHead->uiCtrlCode = VOS_htonl(UPSER_CTLCODE_KPVINFO_RESP);
            
            VOS_StrCpy_S(pstUpfHead->acDeviceLab, UPF_DEVLAB_LEN, "hello world");
        
            lRet = UPF_Conn_TransBufToDownPipeNode(pstUpfConn, pstIobuf);
            if( VOS_ERR == lRet )
            {
                VOS_Printf("pipe down iobuf error!");
                COM_Iobuf_Free(pstIobuf);
                
                return VOS_ERR;
            }
            break;
        case UPSER_CTLCODE_XMLFILE_GET:
            break;
        case UPSER_CTLCODE_APPFILE_GET:
            break;
        default:
            break;
    }
        
    

    return VOS_OK;
}



