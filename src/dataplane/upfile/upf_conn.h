/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : pfw_conn.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2017年4月29日
  最近修改   :
  功能描述   : 端口转发节点创建
  函数列表   :
  修改历史   :
  1.日    期   : 2017年4月29日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/



LONG UPF_Conn_DelNotify(UPF_CONN_S *pstNemConn);

LONG UPF_Conn_TransBufToDownPipeNode(UPF_CONN_S *pstNemConn, COM_IOBUF_S *pstIobuf);

LONG UPF_Conn_Create(SWM_BIZ_CHANNEL_S *pstBizChannel);

LONG UPF_Conn_Release(UPF_CONN_S *pstNemConn);

