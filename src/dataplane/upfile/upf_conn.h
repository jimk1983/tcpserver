/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : pfw_conn.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2017��4��29��
  ����޸�   :
  ��������   : �˿�ת���ڵ㴴��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��4��29��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/



LONG UPF_Conn_DelNotify(UPF_CONN_S *pstNemConn);

LONG UPF_Conn_TransBufToDownPipeNode(UPF_CONN_S *pstNemConn, COM_IOBUF_S *pstIobuf);

LONG UPF_Conn_Create(SWM_BIZ_CHANNEL_S *pstBizChannel);

LONG UPF_Conn_Release(UPF_CONN_S *pstNemConn);

