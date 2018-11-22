/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_mutex.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��17��
  ����޸�   :
  ��������   : �߳����Լ��ź���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef _VOS_MUTEX_H_
#define _VOS_MUTEX_H_


//SM_P(), SM_V(), the same as EventWait(), EventNotify()
#if VOS_PLAT_LINUX
typedef struct tagLinuxSem
{
    UINT32 V;
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
}LINUX_SEM_S;
typedef LINUX_SEM_S VOS_SM_T;
#elif VOS_PLAT_MAC
typedef struct tagMacOSSem
{
    UINT32 V;
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
}MACOS_SEM_S;
typedef MACOS_SEM_S VOS_SM_T;

#elif VOS_PLAT_WIN
#ifndef HANDLE
#define HANDLE (VOID *)
#endif
typedef HANDLE  VOS_SM_T;
#endif


/*��д������*/
#if VOS_PLAT_LINUX
typedef struct tagLinuxRWLock
{
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
    UINT32              uiStatus;
    volatile UINT32  uiLockFlag;
}LINUX_RW_LOCK_S;
typedef LINUX_RW_LOCK_S VOS_RW_LOCK_S;
#elif VOS_PLAT_WIN
typedef struct tagWinRWLock
{
    CRITICAL_SECTION stMutex;
    INT32            stCond;
    UINT32              uiStatus;
    volatile UINT32  uiLockFlag;
}WIN_RW_LOCK_S;
typedef WIN_RW_LOCK_S VOS_RW_LOCK_S;
#endif


#if VOS_PLAT_LINUX
#define __xchg_op(x, ptr) \
    ({ \
    \
    __typeof(*(ptr)) __ret = (x); \
    switch (sizeof(*(ptr))) { \
\
    case 1:      \
    asm volatile("xchgb %b0,%1\n"   \
    : "+q" (__ret), "+m" (*(ptr))   \
    ::"memory", "cc");  \
    break;  \
    case 2: \
    asm volatile("xchgw %w0,%1\n"   \
    : "+r" (__ret), "+m" (*(ptr))   \
    :: "memory", "cc");      \
    break;  \
    case 4:     \
    asm volatile("xchgl %k0,%1\n"   \
    : "+r" (__ret), "+m" (*(ptr))   \
    ::"memory", "cc");  \
    break;  \
    case 8:      \
    asm volatile("xchgq %0,%1\n"    \
     : "+r" (__ret), "+m" (*(ptr))  \
    ::"memory", "cc");  \
    break;  \
}   \
__ret;  \
})

#define xchg(ptr,v) __xchg_op((v), (ptr))
#endif



/*��ʼ���̹߳����д��*/
#define VOS_RWLOCK_INIT(_rwlock)    VOS_ThreadMutex_Init(&(_rwlock))

/*�ͷŹ�����*/
#define VOS_RWLOCK_FREE(_rwlock)    VOS_ThreadMutex_Destroy(&(_rwlock))

/*����*/
#define VOS_RWLOCK_LOCK(_rwLock)    VOS_ThreadMutex_RWLock(&(_rwLock))

/*����*/
#define VOS_RWLOCK_UNLOCK(_rwLock)    VOS_ThreadMutex_RWUnLock(&(_rwLock))

/*����*/
#define VOS_RWLOCK_PLOCK(_prwLock)    VOS_ThreadMutex_RWLock(_prwLock)

/*����*/
#define VOS_RWLOCK_UNPLOCK(_prwLock)    VOS_ThreadMutex_RWUnLock(_prwLock)


INT32 VOS_ThreadMutex_Init(VOS_RW_LOCK_S *pstMutex);

INT32 VOS_ThreadMutex_RWLock(VOS_RW_LOCK_S *pstMutex);

INT32 VOS_ThreadMutex_RWUnLock(VOS_RW_LOCK_S *pstMutex);

INT32 VOS_ThreadMutex_Destroy(VOS_RW_LOCK_S *pstMutex);


INT32 VOS_SM_Init(VOS_SM_T *pstSem);
    
INT32 VOS_SM_P(VOS_SM_T *pstSem, UINT32 ulTimeOutMiliSec);

INT32 VOS_SM_V(VOS_SM_T *pstSem);

INT32 VOS_SM_Destroy(VOS_SM_T *pstSem);


LONG VOS_InterlockedIncrement(ULONG *pulNums);

LONG VOS_InterlockedDecrement(ULONG *pulNums);

#endif

