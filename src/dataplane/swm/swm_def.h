/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_def.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��6��
  ����޸�   :
  ��������   : ����ͷ�ļ�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��6��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

/*ssl���ֵ�״̬*/
typedef enum tagSwmTslStatus
{
    SWM_TLS_SSL_STATUS_INIT = 0,            /*��ʼ��״̬*/
    SWM_TLS_SSL_STATUS_ACCEPTING,           /*������*/
    SWM_TLS_SSL_STATUS_ACCEPTED,            /*�������*/

    SWM_TLS_SSL_STATUS_NUMS
}SWM_TLS_SSL_STATUS_E;

/*���ƹܵ�����*/
typedef enum tagSwmCtrlCmdCode
{
    SWM_CTRLCMD_UNKNOW = 0,
    SWM_CTRLCMD_SNDOUT_COMPELETED,   /*������ɣ������Ѿ�Ϊ��*/

    SWM_CTRLCMD_NUMS
}SWM_CTRL_CMDCODE_E;


/*SWMҵ��TLS�ڵ�*/
typedef struct tagSwmTlsConn            SWM_TLS_CONN_S;

/*SWMҵ��ͨ��*/
typedef struct tagSWMBizChannel         SWM_BIZ_CHANNEL_S;


