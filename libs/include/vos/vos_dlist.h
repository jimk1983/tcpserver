/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_dlist.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��10��
  ����޸�   :
  ��������   : VOS��DLIST����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��10��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_DLIST_H_
#define _VOS_DLIST_H_

/*DLIST�ڵ�*/
typedef struct tagVosDlistNode
{
    struct tagVosDlistNode *prev;
    struct tagVosDlistNode *next;
}VOS_DLIST_NODE_S, *PVOS_DLIST_NODE_S;

#ifndef container_of
#define container_of(ptr, type, member) \
        (type *)((char *)(ptr) - (char *) &((type *)0)->member)
#endif

/*���ݳ�Ա����������ȡ�ڵ��׵�ַ*/
#define VOS_DLIST_ENTRY(pnode,type,membernode) \
         container_of(pnode,type,membernode)
     
/*�����������еĽڵ�,�м�ڵ㲻�ܽ��������Ķ���*/
#define VOS_DLIST_FOR_EACH_ENTRY(pos, pstList, type, stnode)\
     for(pos = VOS_DLIST_ENTRY((pstList)->next, type, stnode);\
         &pos->stnode != (pstList);\
         pos =VOS_DLIST_ENTRY(pos->stnode.next, type, stnode))\

     /*�����������еĽڵ�,��ɾ��ÿ���ڵ�,�м�����ժ��
     ������ͷ�ó�һ���ڵ㣬��ժ���ýڵ㣬�����ϻ��ͷŵȺ���*/
#define VOS_DLIST_FOR_EACH_DEL(pos, pstList, type, stnode)\
          for(pos = VOS_DLIST_ENTRY((pstList)->next, type, stnode);\
              &pos->stnode != (pstList);\
              pos =VOS_DLIST_ENTRY((pstList)->next, type, stnode))\

     /*��ʼ��*/
#define VOS_DLIST_INIT(pstNode) \
do{\
    if ( NULL != (pstNode) )\
    {\
         (pstNode)->prev = (pstNode);\
         (pstNode)->next = (pstNode);\
    }\
}while(0)
          
          
     /*��ӽڵ㵽����ͷ��*/
#define VOS_DLIST_ADD_HEAD(pstList, pstNode)\
 {\
     if( (NULL != (pstList)) && (NULL != (pstNode)) )\
     {\
          (pstNode)->next = (pstList)->next;\
          (pstNode)->prev = (pstList);\
          (pstList)->next->prev = (pstNode);\
          (pstList)->next = (pstNode);\
     }\
 }
  
     /*��ӽڵ㵽����ĩβ*/
#define VOS_DLIST_ADD_TAIL(pstList,pstNode)\
     {\
         if( (NULL != (pstList)) && (NULL != (pstNode)) )\
         {\
             (pstNode)->next = (pstList);\
             (pstNode)->prev = (pstList)->prev;\
             (pstList)->prev->next = (pstNode);\
             (pstList)->prev = (pstNode);\
         }\
     }
          
     /*���ڵ���뵽����ǰ��ڵ��м�*/
#define VOS_DLIST_INSERT(pstPrev,pstNext,pstNode)\
     {\
         if ( (NULL != (pstPrev)) \
             && (NULL != (pstNext))\
             && (NULL != (pstNode)))\
         {\
             (pstPrev)->next = (pstNode);\
             (pstNext)->prev = (pstNode);\
             (pstNode)->prev = (pstPrev);\
             (pstNode)->next = (pstNext);\
         }\
     }
          
     /*��������ժ�����ڵ�*/
#define VOS_DLIST_DEL(pstNode)\
     {\
         if(NULL != (pstNode))\
         {\
             (pstNode)->next->prev = (pstNode)->prev;\
             (pstNode)->prev->next = (pstNode)->next;\
             (pstNode)->next = (pstNode);\
             (pstNode)->prev = (pstNode);\
         }\
     }
          
     /*�ƶ�����ͷ����������*/
#define VOS_DLIST_HEAD_MOVE(pstListFrom,pstListTo)\
     {\
         if ( (NULL != (pstListFrom)) && (NULL != (pstListTo)) )\
         {\
             (pstListTo)->next = (pstListFrom)->next;\
             (pstListFrom)->next->prev = (pstListTo);\
             (pstListFrom)->prev->next = (pstListTo);\
             (pstListTo)->prev = (pstListFrom)->prev;\
             (pstListFrom)->next = (pstListFrom);\
             (pstListFrom)->prev = (pstListFrom);\
         }\
     }





INT32 VOS_Node_Init(VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_HeadAdd(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_TailAdd(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_IsEmpty(VOS_DLIST_NODE_S *pstList);

INT32 VOS_Node_Remove(VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_MoveList(VOS_DLIST_NODE_S *pstListFrom, VOS_DLIST_NODE_S *pstListTo);

INT32 VOS_Node_HeadGet(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S **ppstNode);

INT32 VOS_Node_TailGet(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S **ppstNode);


#endif
