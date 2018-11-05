/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_string.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��2��
  ����޸�   :
  ��������   : Stringͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��2��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_STRING_H_
#define _VOS_STRING_H_

/*��������ַ�����ȫ��������*/
#define VOS_MAXSTRLEN       16384

VOID VOS_Printf(CHAR *pcformat,...);

INT32 VOS_Snprintf(CHAR *pcbuf, UINT32 ulMaxLen, CHAR *pcformat,...);

INT32 VOS_StrCmp(CHAR *pcString1,CHAR *pcString2);

INT32 VOS_StrLen(CHAR *pcString);

CHAR *VOS_StrStr(const CHAR *pcStr1, const CHAR *pcStr2);

CHAR *VOS_StrCpy(CHAR *pcString1,CHAR *pcString2);

CHAR *VOS_StrCpy_S(CHAR *pcDst, ULONG ulMaxLen, CHAR *pcSrc);

CHAR *VOS_StrNCpy_S(CHAR *pcDst, ULONG ulMaxLen,CHAR *pcSrc, ULONG ulLen);

CHAR *VOS_Strrchr(const CHAR *pcString, const CHAR chEndChr);

CHAR *VOS_Strchr(const CHAR *pcString, const CHAR chStartChr);


#endif

