/******************************************************************************

                  ��Ȩ���� (C), 2017-2020, �������޹�˾

 ******************************************************************************
  �� �� ��   : rds_ctx.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2018��8��17��
  ����޸�   :
  ��������   : redis�������Ľӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��8��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*����RDS������������*/
#define RDS_ARRY_NUMS   8

/*Ĭ�ϵ�Redis�ķ������Ķ˿�*/
#define RDS_SERVER_PORT     6379

typedef enum
{
    RDS_LBALGM_TYPE_LEASTCONN = 0,  /*��С������*/
    
    RDS_LBALGM_TYPE_NUMS
}RDS_LBALGM_TYPE_E;

/*RDS��������Ϣ*/
typedef struct tagRdsConnInfo
{
    CHAR            acSevAddr[VOS_IPV4ADDR_STRLEN]; /*redis�ķ�������ַ*/
    int             iSevPort;                       /*redis�ķ������˿�*/
    REDIS_CONN_S*   pstRdsConn;                     /*redis�����Ӿ��*/  
}RDS_CONN_INFO_S, *PRDS_CONN_INFO_S;

/*RDS����������Ϣ*/
typedef struct tagRdsCtx
{
    RDS_CONN_INFO_S     stArryRDSConn[RDS_ARRY_NUMS];   /*RDS����*/
    ULONG               ulNums;                         /*��ǰ����*/
    RDS_LBALGM_TYPE_E   eRdsLBAlgmType;                 /*���ؾ����㷨*/
}RDS_CTX_S, *PRDS_CTX_S;

LONG RDS_CtxInit();

VOID RDS_CtxUnInit();



