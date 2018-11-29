/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_hash.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��2��15��
  ����޸�   :
  ��������   : �򵥵�hashʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��2��15��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

#define VOS_HASHTABLE_SIZE_MIN          100
#define VOS_HASHTABLE_SIZE_MAX          18000

#define VOS_HASHNODE_STRLEN               64

/*�ձ��͵ıȽϺ���*/
typedef LONG (*VOS_HASH_COMP_FUNC)(const VOID *pvArgv1, const VOID *pvArgv2);

/*�ձ��͵�Hashֵ���㺯��*/
typedef ULONG (*VOS_HASH_VALUE_FUNC)(const VOID *pvData);

/*�ṩ���ձ���hash�ڵ�*/
typedef struct tagVosHashNode
{
    /*�ȿ��Ե�ͷ�ڵ�ĳ�ͻ����Ҳ���Ե������ڵ�*/
    VOS_DLIST_NODE_S            stBlkList;
    /*��ͻ������Ŀ*/
    ULONG                       ulBlkNum;
    /*HashΨһ�ļ�ֵ��Ҫ��֤�Ǳ���Ψһ�ģ������ظ�*/
    ULONG                       ulKey;
    /*Hash�ڵ�ֵ*/
    CHAR                        acKeyString[VOS_HASHNODE_STRLEN];
    /*Hash�ڵ㱣�������*/
    VOID                        *pvData;
}VOS_HASH_NODE_S;

/*�ṩ���ձ���hash��*/
typedef struct tagVOSHashTable
{
    /*��ǰHash�������*/
    VOS_HASH_NODE_S         **ppstArrayHashTbl;
    /*Hash��ıȽϺ���*/
    VOS_HASH_COMP_FUNC       pfHashKeyCmp;
    /*Hash���ֵ���㺯��*/
    VOS_HASH_VALUE_FUNC      pfHashCode;
    /*ѡȡ�ĺ��ʵ����������С*/
    LONG                                 lPrime;
}VOS_HASH_TABLE_S;


VOS_HASH_TABLE_S *VOS_Hash_TableCreate(ULONG ulSize, VOS_HASH_COMP_FUNC pfHashCmp, VOS_HASH_VALUE_FUNC pfHashVal);
VOID    VOS_Hash_TableRelease(VOS_HASH_TABLE_S *pstHashTbl);

ULONG VOS_Hash_String(const VOID *pcStr);
LONG   VOS_Hash_CmpString(const VOID *pvArgv1, const VOID *pvArgv2);

ULONG VOS_Hash_ULONGValue(const VOID * pulKey);
LONG   VOS_Hash_ULONGCmp(const VOID *pvArgv1, const VOID *pvArgv2);

LONG   VOS_Hash_ULONGKeySetData(VOS_HASH_TABLE_S *pstHashTbl, ULONG ulKey, VOID *pvData);
LONG   VOS_Hash_ULONGKeyRemove(VOS_HASH_TABLE_S *pstHashTbl, ULONG ulKey);
LONG   VOS_Hash_ULONGKeyGetData(VOS_HASH_TABLE_S *pstHashTbl, ULONG ulKey, VOID **pvData);

LONG   VOS_Hash_StringKeySetData(VOS_HASH_TABLE_S *pstHashTbl, CHAR *pcString, VOID *pvData);
LONG   VOS_Hash_StringKeyRemove(VOS_HASH_TABLE_S *pstHashTbl, CHAR *pcString);

ULONG   VOS_Hash_TablePrime(ULONG ulSize);


