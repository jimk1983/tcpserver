/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vgm_ctx.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月2日
  最近修改   :
  功能描述   : VGateway的上下文任务初始化
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月2日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "vgm/vgm_pub.h"


/*****************************************************************************
 函 数 名  : VGM_Ini_CtxTaskInit
 功能描述  : 线程任务初始化
 输入参数  : VOID *pvArg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_TCP_CtxTaskInit(VOID *pvArg)
{
     /*直接创建网关即可*/
     if ( VOS_ERR == VGM_SSLConn_VTCreate(VGM_DEFAULT_PORT) )
     {
        VOS_Printf("vgm vt-create error!\n");
        return;
     }
     
     VOS_Printf("vgm tcp ctx task init ok");
}

/*****************************************************************************
 函 数 名  : VGM_TCP_CtxTaskUnInit
 功能描述  : 任务出错时，资源释放
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_TCP_CtxTaskUnInit()
{

}


/*****************************************************************************
 函 数 名  : VGM_TCP_CtxInit
 功能描述  : TCP线程网关监听的任务初始化注册
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VGM_TCP_CtxInit()
{
    /*将任务函数放置在索引RCT_TASKINII_ARRYINDEX_TCP中，并且会在RCT_TYPE_TCP类型的线程上进行初始化*/
    if ( VOS_ERR == RCT_API_EnvTaskInitRegister(VGM_TCP_CtxTaskInit, NULL, RCT_TYPE_DP_TCP,RCT_SUBTYPE_SINGLE, VGM_TCP_CtxTaskUnInit) )
    {
        VOS_Printf("VGM tcp ctx init register error!!");
        return VOS_ERR;
    }
    
    VOS_Printf("VGM Ctx init ........................OK!");
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VGM_TCP_CtxUnInit
 功能描述  : 任务资源释放
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月31日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_TCP_CtxUnInit()
{
    VGM_TCP_CtxTaskUnInit();
}

/*****************************************************************************
 函 数 名: VGM_CtxInit
 功能描述  :    Vgateway的上下文初始化，包含了TCP和UDPS两个线程的任务
                           资源初始化，都放在本文件中实现了，主要是实现消息处理
                           消息处理还是分开进行了，因为是两个不同的线程，上下文是不一样的
                           因此TCP的上下文和消息，以及UDPS的上下文和消息，都分开进行处理。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月2日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VGM_CtxInit()
{
    /*配置资源初始化*/
    if ( VOS_ERR == VGM_CFG_CtxInit() )
    {
        VOS_Printf("VGM cfg ctx init error!!");
        return VOS_ERR;
    }
    
    /*初始化TCP监听的线程
    注意: 如果要多个TCP线程,并且监听同一个socketfd, 那么要注意epoll惊群现象:
    如果设置为阻塞模式，那么可以用多个线程accept，不会出现惊群现象
    此时监听的线程就不能采用EPOLL的架构了，后续消息架构可以考虑下
    (主要是为了提高并发)*/
    if ( VOS_ERR ==  VGM_TCP_CtxInit() )
    {
        VGM_CFG_CtxUnInit();
        VOS_Printf("VGM udps ctx init error!!");
        return VOS_ERR;
    }
    
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VGM_CtxUninit
 功能描述  : 任务上下文资源释放
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月2日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_CtxUnInit()
{
    VGM_TCP_CtxUnInit();
    VGM_CFG_CtxUnInit();
}

