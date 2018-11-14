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
#include "fsm/fsm_pub.h"
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
 �� �� ��  : UFP_Ctrl_MakePacketFileDataResponse
 ��������  : �ļ���Ӧ���Ĺ���
 �������  : COM_IOBUF_S *pstOutIobuf   
             COM_IOBUF_S *pstFileIobuf  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���
    
*****************************************************************************/
LONG UFP_Ctrl_MakeHeadInfoDataResponse(COM_IOBUF_S *pstOutIobuf, PFSM_FILE_INFO_S          pstFileInfo)
{
    PUPF_FILEDATA_RESP_S pstHead = NULL;
    SWM_BIZ_HEAD_S*   pstBizHead = NULL;
    UPF_HEAD_S*       pstUpfHead = NULL;
    CHAR*             pcPacket   = NULL;
    UINT32            uiLoadLen  = 0;
    
    pcPacket   = COM_IOBUF_GETSAVED_DATA(pstOutIobuf);
    
    pstBizHead = (SWM_BIZ_HEAD_S  *)pcPacket;

    pstBizHead->uiVersion   = VOS_htonl(EMSDK_VERSION);
    pstBizHead->uiMark1     = VOS_htonl(UPF_PROTO_MARK1);
    pstBizHead->uiMark2     = VOS_htonl(UPF_PROTO_MARK2);    
    COM_IOBUF_SETINPUTED_LEN(pstOutIobuf, SWM_BIZ_HEAD_LEN);
    
    pstUpfHead = (PUPF_HEAD_S)(pcPacket + SWM_BIZ_HEAD_LEN);
    pstUpfHead->uiCtrlCode  = VOS_htonl(UPSER_CTLCODE_XMLFILE);
    pstUpfHead->uiCtrlLength= VOS_htonl(sizeof(UPF_FILEDATA_RESP_S));
    COM_IOBUF_SETINPUTED_LEN(pstOutIobuf, sizeof(UPF_HEAD_S));
    
    pstHead = (PUPF_FILEDATA_RESP_S)(pcPacket + SWM_BIZ_HEAD_LEN + sizeof(UPF_HEAD_S));
    
    pstHead->uiAPPVersion = VOS_htonl(pstFileInfo->stFileInfo.stFileResInfo.uiFileVersion);
    pstHead->uiFileSize = VOS_htonl(pstFileInfo->stFileInfo.stFileResInfo.uiFileSize);
    if ( VOS_ERR == FSM_Conf_GetFileName(pstHead->acFileName, UPF_FILELEN, pstFileInfo->uiFileType) )
    {
        return VOS_ERR;
    }
        
    uiLoadLen = sizeof(UPF_HEAD_S) + sizeof(UPF_FILEDATA_RESP_S);
    pstBizHead->uiDataLen = VOS_htonl(uiLoadLen);
    COM_IOBUF_SETINPUTED_LEN(pstOutIobuf, sizeof(UPF_FILEDATA_RESP_S));
    
    return VOS_ERR;
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
    SWM_BIZ_HEAD_S* pstBizHead      = NULL;
    PUPF_HEAD_S     pstUpfHead      = NULL; 
    PCOM_IOBUF_S    pstChunkIobuf   = NULL;
    CHAR*           pcPacket        = NULL;
    UINT32          uiPackLen       =0 ;
    LONG            lRet        = 0;
    ULONG           uiCtrlCode  = 0;
    
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
        {
            PUPF_KVPINFO_S      pstKvpInfo = NULL;
            PFSM_FILE_INFO_S    pstFileInfo = NULL;

            pstKvpInfo = (PUPF_KVPINFO_S)(pcPacket + SWM_BIZ_HEAD_LEN + sizeof(UPF_HEAD_S));

            pstFileInfo = FSM_Conf_GetFileInfo(FSM_CONF_FILE_XML);
            if ( NULL == pstFileInfo )
            {
                VOS_Printf("fsm config get file entry error");
                return VOS_ERR;
            }
            
            pstKvpInfo->uiLastVersion   = VOS_htonl(pstFileInfo->stFileInfo.stFileResInfo.uiFileVersion);
            pstKvpInfo->uiIntervalRate  = VOS_htonl(UPF_KEEPALIVE_RATEVAL);
            pstKvpInfo->uiFileSize      = VOS_htonl(pstFileInfo->stFileInfo.stFileResInfo.uiFileSize);

            if( VOS_ERR == FSM_Conf_GetServerAddrInfo(pstKvpInfo->acAddrInfo, UPF_FILELEN))
            {
                VOS_Printf("fsm config get server error");
                return VOS_ERR;
            }
            
            if( VOS_ERR == FSM_Conf_GetFileName(pstKvpInfo->acFileName, UPF_FILELEN, FSM_CONF_FILE_XML) )
            {
                VOS_Printf("fsm config get server error");
                return VOS_ERR;
            }

            VOS_Mem_Zero(pstKvpInfo->acFileValue, UPF_FILEVALEN);
            VOS_StrCpy_S(pstKvpInfo->acFileValue, UPF_FILEVALEN, pstFileInfo->stFileInfo.stFileResInfo.acFileCRCVal);
            
            pstUpfHead->uiCtrlCode      = VOS_htonl(UPSER_CTLCODE_KPVINFO_RESP);

            COM_IOBUF_SETINPUTED_LEN(pstIobuf, sizeof(UPF_KVPINFO_S));
            
            lRet = UPF_Conn_TransBufToDownPipeNode(pstUpfConn, pstIobuf);
            if( VOS_ERR == lRet )
            {
                VOS_Printf("pipe down iobuf error!");
                return VOS_ERR;
            }
        }
        break;
        
        case UPSER_CTLCODE_XMLFILE_GET:
        {
            PUPF_FILEDATA_REQ_S     pstFileReq      = NULL;
            PFSM_FILE_INFO_S        pstFileInfo     = NULL;
            PCOM_IOBUF_S            pstOutIobuf     = NULL;
            UINT32                  uiChunkIndex    = 0;
            UINT32                  uiCount         = 0;
            
            pstFileReq = (PUPF_FILEDATA_REQ_S)(pcPacket + SWM_BIZ_HEAD_LEN + sizeof(UPF_HEAD_S));

            pstFileInfo = FSM_Conf_GetFileInfo(FSM_CONF_FILE_XML);
            if ( NULL == pstFileInfo )
            {
                return VOS_ERR;
            }

            pstUpfConn->uiMgrChunkStartIndex        =  VOS_htonl(pstFileReq->uiChunkStart);
            pstUpfConn->uiMgrChunkNums              =  pstFileInfo->uiIoBufNums;
            
            VOS_Mem_Copy_S(&pstUpfConn->stFileEntryInfo, sizeof(FSM_RENTRY_S), &pstFileInfo->stFileInfo, sizeof(FSM_RENTRY_S));

            for ( uiCount =0; uiCount < pstFileInfo->uiIoBufNums; uiCount++)
            {
                /*��Ƭ����*/
                uiChunkIndex = pstUpfConn->uiMgrChunkStartIndex + pstUpfConn->uiMgrChunkCount;
                
                /*��ȡ�ڴ�*/
                pstChunkIobuf = (COM_IOBUF_S *)FSM_Conf_GetChunkIobufByIndex(FSM_CONF_FILE_XML, uiChunkIndex);
                if ( NULL == pstChunkIobuf )
                {
                    return VOS_ERR;
                }

                pstOutIobuf = COM_Iobuf_Malloc(0);
                if ( NULL == pstOutIobuf )
                {
                    return VOS_ERR;
                }
                
                lRet = UFP_Ctrl_MakeHeadInfoDataResponse(pstOutIobuf, pstFileInfo);
                if ( VOS_ERR == lRet )
                {
                    COM_Iobuf_Free(pstOutIobuf);
                    pstOutIobuf = NULL;
                    return VOS_ERR;
                }                
                
                lRet = UPF_Conn_TransBufToDownPipeNode(pstUpfConn, pstOutIobuf);
                if( VOS_ERR == lRet )
                {
                    VOS_Printf("pipe down iobuf error!");
                    COM_Iobuf_Free(pstOutIobuf);
                    return VOS_ERR;
                }
                
                /*���͵ļ�������*/
                pstUpfConn->uiMgrChunkCount++;
                /*�򿪷���֪ͨ��ͨ�����ƽ����첽����*/
                pstUpfConn->uiMgrChunkStatus = UPF_TRNSTATUS_SNDING;
                
                if ( VOS_SYS_EWOULDBLOCK == lRet)
                {   
                    VOS_Printf("updata file transfer down EwouldBlocked!");
                    break;
                }
            }
        }    
        break;
        
        case UPSER_CTLCODE_APPFILE_GET:
            break;
        default:
            break;
    }
        
    

    return VOS_OK;
}



