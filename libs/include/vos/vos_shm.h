/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_shm.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��7��
  ����޸�   :
  ��������   : �����ڴ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

/*�����ڴ��Ĭ�����ݿ��С128K*/
#define VOS_SHMEM_SIZE  131072

#if VOS_PLAT_LINUX
typedef struct tagLinuxMemUnit
{
    pthread_mutex_t stProcMutex;
    CHAR            acMapMem[VOS_SHMEM_SIZE];   
}LINUX_MEM_UNIT, *PLINUX_MEM_UNIT;

typedef struct tagLinuxSHareMem
{
    INT32 shmid;
    key_t key;
    PLINUX_MEM_UNIT pstMem;
}LINUX_SHAREMEM_S;
typedef LINUX_SHAREMEM_S VOS_SHM_T, *PVOS_SHM_T;
#elif VOS_PLAT_MAC

#elif VOS_PLAT_WIN

#endif



VOID*   VOS_SHMemCreate(VOS_SHM_T *pstHandle, CHAR *pcKeyString, UINT32 uiSize);

VOID*   VOS_SHMemOpen(VOS_SHM_T *pstHandle, CHAR *pcKeyString);

VOID    VOS_SHMemLock(VOS_SHM_T *pstHandle);

VOID    VOS_SHMemUnLock(VOS_SHM_T *pstHandle);

VOID    VOS_SHMemClose(VOS_SHM_T *pstHandle);







