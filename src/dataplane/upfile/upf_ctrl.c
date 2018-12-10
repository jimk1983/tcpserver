/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : pfw_ctrl.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2017年4月29日
  最近修改   :
  功能描述   : 端口转发控制模块
  函数列表   :
  修改历史   :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/


#include "common.h"
#include "swm/swm_pub.h"
#include "fsm/fsm_pub.h"
#include "upfile/upf_pub.h"



/*数据流返回时候的组装报文*/
VOID UPF_Ctrl_MakeCommBizHeadLen(COM_IOBUF_S *pstIobuf, UINT32 uiLocalSockfd, UINT32 uiStreamLen)
{
    //UPF_HEAD_S      *pstBizHead = NULL;
    
    if  (NULL == pstIobuf)
    {
        return;
    }

    //pstBizHead = (UPF_HEAD_S *)(pstIobuf->pcData);

    //VOS_Printf("upf ctrl make pstBizhead=%p", pstBizHead);
  
    return;
}

/*****************************************************************************
 函 数 名  : UFP_Ctrl_MakePacketFileDataResponse
 功能描述  : 文件响应包的构建
 输入参数  : COM_IOBUF_S *pstOutIobuf   
             COM_IOBUF_S *pstFileIobuf  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 蒋康
    修改内容   : 新生成函数
    
*****************************************************************************/
LONG UFP_Ctrl_MakePacketDataResponse(UPF_CONN_S *pstUpfConn, COM_IOBUF_S *pstOutIobuf, COM_IOBUF_S *pstChunkBuf, UCHAR *pcChunkCrcValue, UINT32 uiChunkIndex)
{
    PUPF_FILEDATA_RESP_S pstData = NULL;
    SWM_BIZ_HEAD_S*   pstBizHead = NULL;
    UPF_HEAD_S*       pstUpfHead = NULL;
    CHAR*             pcPacket   = NULL;
    UINT32            uiLoadLen  = 0;

    if ( NULL == pstOutIobuf
        || NULL == pstUpfConn 
        || NULL == pstChunkBuf )
    {
        return VOS_ERR;
    }
    
    pcPacket   = COM_IOBUF_GETSAVED_DATA(pstOutIobuf);

    /*业务头封装*/
    pstBizHead = (SWM_BIZ_HEAD_S  *)pcPacket;
    pstBizHead->uiVersion   = VOS_htonl(EMSDK_VERSION);
    pstBizHead->uiMark1     = VOS_htonl(UPF_PROTO_MARK1);
    pstBizHead->uiMark2     = VOS_htonl(UPF_PROTO_MARK2);    
    COM_IOBUF_SETINPUTED_LEN(pstOutIobuf, SWM_BIZ_HEAD_LEN);

    /*业务头封装*/
    pstUpfHead = (PUPF_HEAD_S)(pcPacket + SWM_BIZ_HEAD_LEN);
    pstUpfHead->uiCtrlCode  = VOS_htonl(pstUpfConn->uiUpfCtrlCode);
    pstUpfHead->uiCtrlLength= VOS_htonl(sizeof(UPF_FILEDATA_RESP_S));
    COM_IOBUF_SETINPUTED_LEN(pstOutIobuf, sizeof(UPF_HEAD_S));

    /*数据包封装*/    
    pstData = (PUPF_FILEDATA_RESP_S)(pcPacket + SWM_BIZ_HEAD_LEN + sizeof(UPF_HEAD_S));
    pstData->uiAPPVersion = VOS_htonl(pstUpfConn->stFileEntryInfo.stFileResInfo.uiFileVersion);
    pstData->uiFileSize   = VOS_htonl(pstUpfConn->stFileEntryInfo.stFileResInfo.uiFileSize);
    pstData->uiChunkNums  = VOS_htonl(pstUpfConn->uiMgrChunkNums);
    pstData->uiChunkIndex = VOS_htonl(uiChunkIndex);
    pstData->uiChunkSize  = VOS_htonl(pstChunkBuf->ulDataLen);
    VOS_Mem_Copy_S(pstData->acChunkValue, UPF_FILEVALEN, pcChunkCrcValue, UPF_FILEVALEN);
    
    if ( VOS_ERR == FSM_Conf_GetFileName(pstData->acFileName, UPF_FILELEN, (FSM_CONF_FILE_E)pstUpfConn->stFileEntryInfo.stFileResInfo.uiFileType) )
    {
        return VOS_ERR;
    }
    
    VOS_Mem_Copy_S(pstData->acChunkContent, UPF_CONTENTLEN, pstChunkBuf->pcData, pstChunkBuf->ulDataLen);
    uiLoadLen = sizeof(UPF_HEAD_S) + sizeof(UPF_FILEDATA_RESP_S);
    pstBizHead->uiDataLen = VOS_htonl(uiLoadLen);
    COM_IOBUF_SETINPUTED_LEN(pstOutIobuf, sizeof(UPF_FILEDATA_RESP_S));

    //VOS_Printf("UFP_Ctrl_MakePacketDataResponse: FileSize=%d, fileName=%s", 
    //                pstUpfConn->stFileEntryInfo.stFileResInfo.uiFileSize, 
    //                pstData->acFileName);
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : UPF_Ctrl_PipeDownData
 功能描述  : 下发数据包
 输入参数  : UPF_CONN_S *pstUpfConn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月15日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 UPF_Ctrl_PipeDownData(UPF_CONN_S *pstUpfConn)
{
    PFSM_CHUNK_IOBUF_S  pstChunkInfo    = NULL;
    PCOM_IOBUF_S        pstOutIobuf     = NULL;
    UINT32              uiChunkIndex    = 0;
    UINT32              uiCount         = 0;
    LONG                lRet            = 0;

    if ( NULL == pstUpfConn )
    {
        return VOS_ERR;
    }

    for ( uiCount =0; uiCount < pstUpfConn->uiMgrChunkNums; uiCount++)
    {
        /*分片索引*/
        uiChunkIndex = pstUpfConn->uiMgrChunkStartIndex + pstUpfConn->uiMgrChunkCount;
        
        /*获取内存*/
        pstChunkInfo = (PFSM_CHUNK_IOBUF_S)FSM_Conf_GetChunkIobufByIndex((FSM_CONF_FILE_E)pstUpfConn->stFileEntryInfo.stFileResInfo.uiFileType, uiChunkIndex);
        if ( NULL == pstChunkInfo )
        {
            return VOS_ERR;
        }

        pstOutIobuf = COM_Iobuf_Malloc(0);
        if ( NULL == pstOutIobuf )
        {
            return VOS_ERR;
        }
        
        lRet = UFP_Ctrl_MakePacketDataResponse(pstUpfConn,pstOutIobuf, pstChunkInfo->pstIoBuf,pstChunkInfo->acChunkCRCVal, uiChunkIndex);
        if ( VOS_ERR == lRet )
        {
            COM_Iobuf_Free(pstOutIobuf);
            pstOutIobuf = NULL;
            return VOS_ERR;
        }                
        
        lRet = UPF_Conn_TransBufToDownPipeNode(pstUpfConn, pstOutIobuf);
        if( VOS_ERR == lRet )
        {
            VOS_Printf("pipe down iobuf error!err=%d", lRet);
            COM_Iobuf_Free(pstOutIobuf);
            return VOS_ERR;
        }
        
        /*发送的计数增加*/
        pstUpfConn->uiMgrChunkCount++;
        
        /*水位控制*/
        if ( VOS_SYS_EWOULDBLOCK == lRet)
        {   
            //VOS_Printf("updata file transfer down EwouldBlocked!");
            break;
        }

        /*是不是已经最后一片*/
        if ( uiChunkIndex == pstUpfConn->uiMgrChunkNums )
        {
            //VOS_Printf("UPF_Ctrl down data, uiChunkIndex=%d,pstUpfConn->uiMgrChunkCount=%d,!",uiChunkIndex, pstUpfConn->uiMgrChunkCount);
            /*发送结束了*/
            pstUpfConn->uiMgrChunkStatus = UPF_TRNSTATUS_SNDEND;
            break;
        }
    }

    return VOS_OK;
}
/*****************************************************************************
 函 数 名  : UPF_CtrlHandler
 功能描述  : 控制处理
 输入参数  : PFW_CONN_S *pstPFWConn    
             PFW_CTRL_INFO_S *pstCtrl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年5月21日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
INT32 UPF_Ctrl_Handler(UPF_CONN_S *pstUpfConn, COM_IOBUF_S *pstIobuf)
{ 
    SWM_BIZ_HEAD_S* pstBizHead      = NULL;
    PUPF_HEAD_S     pstUpfHead      = NULL; 
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
    
    //VOS_Printf("UPF conn upper :recv the iobuf=%p, upf conn=%p, uiPackLen=%d, pstBizHead=%p!",  
    //    pstIobuf, pstUpfConn, uiPackLen, pstBizHead);
    
    switch(uiCtrlCode)
    {
        case UPSER_CTLCODE_KPVINFO_REQ:
        {
            PUPF_KVPINFO_S      pstKvpInfo = NULL;
            PFSM_FILE_INFO_S    pstFileInfo = NULL;

            pstKvpInfo = (PUPF_KVPINFO_S)(pcPacket + SWM_BIZ_HEAD_LEN + sizeof(UPF_HEAD_S));
            
            VOS_PRINT("TerminalID: [%s]-->Keepalive: AppVersion=%08x, Desptor=%s",
                pstKvpInfo->acDevLabel, 
                VOS_ntohl(pstKvpInfo->uiLastVersion),
                pstKvpInfo->acDevDecptor);
            if ( '\0' == pstUpfConn->acTerminalID[0]  )
            {
                VOS_StrCpy_S(pstUpfConn->acTerminalID, UPF_DEVLAB_LEN-1, pstKvpInfo->acDevLabel);
            }

            pstFileInfo = FSM_Conf_GetFileInfo(FSM_CONF_FILE_XML);
            if ( NULL == pstFileInfo )
            {
                VOS_Printf("fsm config get file entry error");
                return VOS_ERR;
            }
            
            pstKvpInfo->uiLastVersion   = VOS_htonl(pstFileInfo->stFileInfo.stFileResInfo.uiFileVersion);
            pstKvpInfo->uiIntervalRate  = VOS_htonl(UPF_KEEPALIVE_RATEVAL);
            pstKvpInfo->uiFileSize      = VOS_htonl(pstFileInfo->stFileInfo.stFileResInfo.uiFileSize);
            pstKvpInfo->usCmdCode       = VOS_htons(UPDEV_CMDCODE_UPDATA_FORCE);

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
            
            pstFileReq = (PUPF_FILEDATA_REQ_S)(pcPacket + SWM_BIZ_HEAD_LEN + sizeof(UPF_HEAD_S));

            pstFileInfo = FSM_Conf_GetFileInfo(FSM_CONF_FILE_XML);
            if ( NULL == pstFileInfo )
            {
                return VOS_ERR;
            }

            pstUpfConn->uiMgrChunkStartIndex    = VOS_htonl(pstFileReq->uiChunkStart);
            pstUpfConn->uiMgrChunkNums          = pstFileInfo->uiIoBufNums;
            pstUpfConn->uiMgrChunkCount         = 0;
            pstUpfConn->uiMgrChunkStatus        = UPF_TRNSTATUS_SNDING;
            pstUpfConn->uiUpfCtrlCode           = UPSER_CTLCODE_XMLFILE;
            
            VOS_Mem_Copy_S(&pstUpfConn->stFileEntryInfo, sizeof(FSM_RENTRY_S), &pstFileInfo->stFileInfo, sizeof(FSM_RENTRY_S));
            
            if ( VOS_ERR == UPF_Ctrl_PipeDownData(pstUpfConn) )
            {
                 return VOS_ERR;
            }
            else
            {
                COM_Iobuf_Free(pstIobuf);
            }
        }    
        break;
        
        case UPSER_CTLCODE_APPFILE_GET:
        {
            PUPF_FILEDATA_REQ_S     pstFileReq      = NULL;
            PFSM_FILE_INFO_S        pstFileInfo     = NULL;
            
            pstFileReq = (PUPF_FILEDATA_REQ_S)(pcPacket + SWM_BIZ_HEAD_LEN + sizeof(UPF_HEAD_S));

            pstFileInfo = FSM_Conf_GetFileInfo(FSM_CONF_FILE_APP);
            if ( NULL == pstFileInfo )
            {
                return VOS_ERR;
            }
            
            pstUpfConn->uiMgrChunkStartIndex    = VOS_htonl(pstFileReq->uiChunkStart);
            pstUpfConn->uiMgrChunkNums          = pstFileInfo->uiIoBufNums;
            pstUpfConn->uiMgrChunkCount         = 0;
            pstUpfConn->uiMgrChunkStatus        = UPF_TRNSTATUS_SNDING;
            pstUpfConn->uiUpfCtrlCode           = UPSER_CTLCODE_APPFILE;
            
            VOS_Mem_Copy_S(&pstUpfConn->stFileEntryInfo, sizeof(FSM_RENTRY_S), &pstFileInfo->stFileInfo, sizeof(FSM_RENTRY_S));

            if ( VOS_ERR == UPF_Ctrl_PipeDownData(pstUpfConn) )
            {
                return VOS_ERR;
            }
            else
            {
                COM_Iobuf_Free(pstIobuf);
            }
        }
        break;
        default:
            VOS_Printf("UPF conn upper :recv the iobuf=%p, upf conn=%p, uiPackLen=%d, pstBizHead=%p!",  
                pstIobuf, pstUpfConn, uiPackLen, pstBizHead);
            break;
    }
        
    

    return VOS_OK;
}



