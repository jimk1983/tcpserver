/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : dbg_log.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��2��14��
  ����޸�   :
  ��������   : Debug
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��2��14��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
    
#define LOG_LEVEL_ERROR             0x01
#define LOG_LEVEL_WARNING         0x02
#define LOG_LEVEL_EVENT              0x04
#define LOG_LEVEL_DEBUG             0x08
#define LOG_LEVEL_ALL                  0x0F         /*0x0F--ȫ����0x00:--ȫ�ر�*/


VOID VOS_PrintDebug(const CHAR *pcformat, ...);

VOID VOS_DebugTraceOutput(const CHAR *pcformat, ...);

VOID VOS_DebugBufHexOutput(CHAR *pcHeadDescript, CHAR *pcBuf, LONG ulLen);

VOID VOS_WriteSystemLog(const CHAR *pcformat,...);

VOID VOS_WriteRctLog(const CHAR *pcformat,...);

LONG VOS_Log_Init();

VOID VOS_Log_UnInit();

#define DEBUG_PRINT_FORMAT "[FILE=%s,FUNC=%s,LINE=%d]--->"

#define DbgTrace(pcDbgInfo, ...)\
{\
  if( VOS_TRUE )\
  {\
    (VOID)VOS_DebugTraceOutput((DEBUG_PRINT_FORMAT pcDbgInfo),__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);\
  }\
}


