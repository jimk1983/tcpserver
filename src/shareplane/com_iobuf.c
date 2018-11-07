/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : com_iobuf.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��10��28��
  ����޸�   :
  ��������   : ͨ�õ�IOBUF
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include "libcomm.h"
#include "shareplane/share_comm.h"


/*****************************************************************************
 �� �� ��  : COM_Iobuf_Malloc
 ��������  : IOBUF����
 �������  : ULONG ulMid  
 �������  : ��
 �� �� ֵ  : COM_IOBUF_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
COM_IOBUF_S *COM_Iobuf_Malloc(ULONG ulMid)
{
    COM_IOBUF_S *pstIobuf  =    NULL;
    ULONG ulBufSize             =    COM_IOBUF_MALLOC_SIZE;

    pstIobuf = (COM_IOBUF_S *)VOS_Malloc(ulMid, ulBufSize);
    if ( NULL == pstIobuf )
    {
        VOS_Printf("malloc error!");
        return NULL;
    }

    VOS_Mem_Zero((CHAR *)pstIobuf, ulBufSize);

    VOS_Node_Init(&pstIobuf->stNode);

    pstIobuf->ulMagic     = COM_IOBUF_MAGIC;
    pstIobuf->lLeftLen    = COM_IOBUF_LEN;
    pstIobuf->ulPreDataLen  = COM_IOBUF_LEN;
    pstIobuf->ulMaxLen  = COM_IOBUF_LEN;

    return pstIobuf;
}

/*****************************************************************************
 �� �� ��  : COM_Iobuf_Free
 ��������  : IOBUF�ͷ�
 �������  : COM_IOBUF_S *pstIobuf  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID COM_Iobuf_Free(COM_IOBUF_S *pstIobuf)
{
    if ( NULL == pstIobuf  )
    {
        return;
    }
    
    VOS_Free((CHAR *)pstIobuf);
}

/*****************************************************************************
 �� �� ��  : COM_Iobuf_QueCreate
 ��������  : IOBUF�Ķ��д���, ������ܲ�������
                          ע��: ��Ҫ��ͬһ���߳��ڽ����շ������ܶ��߳�ʹ��
                           
 �������  : ��
 �������  : ��
 �� �� ֵ  : COM_IOBUF_QUE_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
COM_IOBUF_QUE_S *COM_Iobuf_QueCreate()
{
    COM_IOBUF_QUE_S *pstIobufQue = NULL;

    pstIobufQue = (COM_IOBUF_QUE_S *)VOS_Malloc(COM_QUE_MID, sizeof(COM_IOBUF_QUE_S));
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("malloc error");
        return NULL;
    }

    VOS_Mem_Zero((CHAR *)pstIobufQue, sizeof(COM_IOBUF_QUE_S));

    /*�����ʼ��,ͷ�ڵ�*/
    VOS_DLIST_INIT(&pstIobufQue->stList);

    pstIobufQue->uiNum = 0;
    
    return pstIobufQue;
}


/*****************************************************************************
 �� �� ��  : COM_Iobuf_QueFree
 ��������  : IOBUF�Ķ����ͷ�
 �������  : COM_IOBUF_QUE_S *pstIobufQue  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID COM_Iobuf_QueFree(COM_IOBUF_QUE_S *pstIobufQue)
{
    COM_IOBUF_S *pstIobuf         =  NULL;
    COM_IOBUF_S *pstIobufTmp  =  NULL;
    
    if ( NULL == pstIobufQue )
    {
        return;
    }

    /*����ж���Ľڵ㣬�����ɾ���������ڴ�й©*/
    if ( 0 < pstIobufQue->uiNum )
    {
        VOS_DLIST_FOR_EACH_ENTRY(pstIobuf, &pstIobufQue->stList, COM_IOBUF_S, stNode)
        {
            /*�Ƚ��¸��ڵ㱣��*/
            pstIobufTmp = VOS_DLIST_ENTRY(pstIobuf->stNode.next, COM_IOBUF_S, stNode);
            
            /*�����Ѿ������ˣ���˿���ֱ������*/
            VOS_Node_Remove((&pstIobuf->stNode));
            /*��Ϣ�ڵ�ʹ��VOS_Malloc()����ģ������ڴ����*/
            VOS_Free((CHAR *)pstIobuf);
            /*������һ���ڵ���ж�*/
            pstIobuf = pstIobufTmp;
        }
    }   
    VOS_Free((CHAR *)pstIobufQue);  

}


/*****************************************************************************
 �� �� ��  : COM_Iobuf_QuePush
 ��������  : �����
 �������  : COM_IOBUF_QUE_S *pstIobufQue  
             COM_IOBUF_S *pstIobuf         
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 COM_Iobuf_QuePush(COM_IOBUF_QUE_S *pstIobufQue, COM_IOBUF_S *pstIobuf)
{
    if ( NULL == pstIobufQue
            || NULL == pstIobuf )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    /*�������*/
    VOS_DLIST_ADD_TAIL(&pstIobufQue->stList, &pstIobuf->stNode);

    pstIobufQue->uiNum++;
    
    return VOS_OK;
}



/*****************************************************************************
 �� �� ��  : COM_Iobuf_QuePop
 ��������  : �Ӷ����л�ȡ��Ϣ�ڵ㣬�����ȳ�������ͷȡ��
 �������  : COM_IOBUF_QUE_S *pstIobufQue  
 �������  : ��
 �� �� ֵ  : COM_IOBUF_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
COM_IOBUF_S * COM_Iobuf_QuePop(COM_IOBUF_QUE_S *pstIobufQue)
{
    VOS_DLIST_NODE_S        *pstNode       = NULL;
    COM_IOBUF_S                 *pstIobuf       = NULL;
    
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("param error");
        return NULL;
    }

    /*�ն���*/
    if ( 0 == pstIobufQue->uiNum )
    {
        /*�����ܷ����ģ����*/
        if(VOS_OK != VOS_Node_IsEmpty(&pstIobufQue->stList))
        {
            VOS_ASSERT(0);
            VOS_Printf("system error: iobuf queue error!");
        }
        return NULL;
    }
    
    /*ȡ������ͷ�ڵ�*/
    if(VOS_ERR == VOS_Node_HeadGet(&pstIobufQue->stList, &pstNode))
    {
        VOS_Printf("system error: iobuf queue get node error!");
    }
    
    pstIobufQue->uiNum--;

    pstIobuf = (COM_IOBUF_S  *)pstNode;

    return pstIobuf;
}

/*****************************************************************************
 �� �� ��  : COM_Iobuf_QueGetNums
 ��������  : ��ȡ�����е�iobuf����
 �������  : COM_IOBUF_QUE_S *pstIobufQue  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
UINT32 COM_Iobuf_QueGetNums(COM_IOBUF_QUE_S *pstIobufQue)
{
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("param error");
        return 0;
    }

    return pstIobufQue->uiNum;
}

/*****************************************************************************
 �� �� ��  : COM_Iobuf_QueIsEmpty
 ��������  : �ж϶����Ƿ�Ϊ��
 �������  : COM_IOBUF_QUE_S *pstIobufQue  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG COM_Iobuf_QueIsEmpty(COM_IOBUF_QUE_S *pstIobufQue)
{
    if ( NULL == pstIobufQue )
    {
        VOS_Printf("param error");
        return VOS_FALSE;
    }
    
    if ( 0 == pstIobufQue->uiNum )
    {
        if(VOS_OK != VOS_Node_IsEmpty(&pstIobufQue->stList))
        {
            VOS_ASSERT(0);
            VOS_Printf("system error: iobuf queue error!");
            return VOS_FALSE;
        }
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


