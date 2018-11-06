/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : swm_tls.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��4��
  ����޸�   :
  ��������   : TLS�ķ��ͺͽ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��4��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    

/*���صĹܵ���ʼ���ݽ��սڵ�*/
struct tagSwmTlsConn
{
    /*��ǰ�ڵ�, ������pipe��*/
    VOS_DLIST_NODE_S            stNode;
    /*��ǰ��ʼTLS�ڵ�Ĺܵ�����*/
    SWM_PIPE_CONN_S             stTlsPipe;
    
    /*���屾�ڵ�������¼�*/
    RCT_NETOPT_EVENT_S         stNetEvtOps;
    /*���屾�ڵ���ϻ��¼�*/
    RCT_EXPIROPT_EVENT_S      stExpireOps;
    /*���屾�ڵ�Ķ�ʱ���¼�*/
    //RCT_TIMEROPT_EVENT_S      stTimerOps;
    
    /*ҵ��ͨ��*/
    SWM_BIZ_CHANNEL_S         *pstBizChannel;

    /*��ǰ��socketfd*/
    LONG                        lConnfd;
    /*��ǰ��sslָ��*/
    //SSL_S                       *pstSsl;
    /*SSL���ֵ�״̬*/
    LONG                        lHandShakeStatus;

    /*��¼��ǰ��socket iobuf*/
    COM_IOBUF_S                    *pstRecvIobuf;
    /*���ͻؿͻ��˵�*/
    COM_IOBUF_S                    *pstSendIobuf;
    
};


LONG    SWM_TLS_ConnCreate(SWM_MSG_ADDCONN_S *pstMsgInfo);

LONG    SWM_TLS_ConnRelease(SWM_TLS_CONN_S *pstTlsConn);

VOID    SWM_TLS_ConnExpireConfirm(SWM_TLS_CONN_S *pstTlsConn);

