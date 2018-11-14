/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_malloc.h
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
#ifndef _VOS_MALLOC_H_
#define _VOS_MALLOC_H_


/*�õ�ָ����ַ���ֽں���*/
/***С��ϵͳ**������:��ֵa=0x11223344, ��ʵ11�����λ���ڴ��ַ�͵���������0x44,0x33,0x22,0x11
 ***���ϵͳ**��һ����0x11223344, 0x11���λ���ڴ�͵��ߵĲ鿴�ǣ�0x11, 0x22, 0x33,0x44,
 Ҳ����˵�� ��˴��ʱ������λ���ڵ�һ������������CPU�ó�����λ����С�˽�Ϊ����*/

/*�õ�����ֽ�λ��ֵ*/
#define MEM_GET_BYTE(x)     (*((BYTE *)(x)))
#define MEM_GET_INT32(x)   (*((INT32 *)(x)))

/*�����ڴ�*/
CHAR *VOS_Memory(ULONG ulSize);

/*�ڴ�����*/
VOID VOS_Mem_Zero(VOID *pcBuf, ULONG ulSize);

/*�ڴ氲ȫ�ͷ�*/
VOID VOS_MemFree(CHAR *ucPtr);

/*�ڴ濽��*/
VOID VOS_Mem_Copy(CHAR *pcDst, CHAR *pcSrc,ULONG ulSize);

/*�ڴ氲ȫ����*/
VOID VOS_Mem_Copy_S(VOID *pcDst, ULONG ulMaxLen, VOID *pcSrc,ULONG ulSize);

/*�ڴ�Ƚ�*/
LONG VOS_MemCmp(CHAR *pcMem1, CHAR *pcMem2, UINT32 uiSize);


#endif
