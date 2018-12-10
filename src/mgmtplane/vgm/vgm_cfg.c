/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vgm_cfg.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月3日
  最近修改   :
  功能描述   : Vgateway的配置信息
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
    
    
#include "common.h"
#include "vgm/vgm_pub.h"

/*所有虚拟网关的信息，都要通过set/get来设置和获取
不允许直接extern虚拟网关信息*/
VGM_CFG_CTX_S  *g_pstVgmCfgCtx   =   NULL;       /*该为全局的变量*/


/*****************************************************************************
 函 数 名  : VGM_CFG_GetFreeVTID
 功能描述  : 找到空闲的虚拟网关ID
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
ULONG VGM_CFG_GetFreeVTID()
{
    ULONG ulIndex = 0;

    if ( NULL == g_pstVgmCfgCtx )
    {
        return  VOS_ERR;
    }
        
    for ( ulIndex =0; ulIndex < VGM_CFG_ARRAY_NUMS; ulIndex++ )
    {
        /*找到空闲的指针*/
        if ( NULL == g_pstVgmCfgCtx->pastVgCfgArray[ulIndex] )
        {
            return ulIndex;
        }
    }
    
    return VOS_ERR;    
}

/*****************************************************************************
 函 数 名  : VGM_CFG_GetVTCtxByVTID
 功能描述  : 获取对应的VTID虚拟网关的上下文配置
 输入参数  : ULONG ulVtId  
 输出参数  : 无
 返 回 值  : VGM_CTX_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VGM_CTX_S *VGM_CFG_GetVTCtxByVTID(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId )
    {
        VOS_Printf("param error!");
        return NULL;
    }
    
    return g_pstVgmCfgCtx->pastVgCfgArray[ulVtId];
}

/*****************************************************************************
 函 数 名  : VGM_CFG_DelVTCtxByVTID
 功能描述  : 删除网关
 输入参数  : ULONG ulVtId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_CFG_DelVTCtxByVTID(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId )
    {
        VOS_Printf("param error!");
        return;
    }

    if ( NULL != g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]  )
    {
        VOS_Free((CHAR *)g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]);
        g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]= NULL;
    }
    g_pstVgmCfgCtx->ulCurrentNums--;
    
    return ;
}


/*****************************************************************************
 函 数 名:  VGM_CFG_SetVTCtxByVTID
 功能描述  :    设置VTCtx
 输入参数  :    VGM_CTX_S *pstVgCtx  --待设置的CTX
            ULONG ulVtId               --空闲的VTID
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VGM_CFG_SetVTCtxByVTID(VGM_CTX_S *pstVgCtx, ULONG ulVtId)
{
    if ( NULL == g_pstVgmCfgCtx 
        || VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        VOS_Printf("param error!");
        return VOS_ERR;
    }
    
    if(NULL !=  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        VOS_Printf("vtid ctx has exist!");
        return VOS_ERR;
    }

    g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] = pstVgCtx;
    g_pstVgmCfgCtx->ulCurrentNums++;

    return VOS_OK;
}



/*****************************************************************************
 函 数 名  : VGM_CFG_GetCurrentConnNums
 功能描述  : 获取当前网关的连接数
 输入参数  : VGM_CTX_S *pstVgCtx  
             ULONG ulVtId         
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月6日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
ULONG VGM_CFG_GatewayConnGetNums(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        return VOS_ERR;
    }
    
    if(NULL ==  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        return VOS_ERR;
    }
    
    return g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]->ulConnNums;
}

/*****************************************************************************
 函 数 名  : VGM_CFG_GatewayConnIncrement
 功能描述  : 增加当前网关的连接统计数
 输入参数  : ULONG ulVtId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_CFG_GatewayConnIncrement(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        return;
    }
    
    if(NULL ==  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        return;
    }
    
    VOS_InterlockedIncrement(&g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]->ulConnNums);
}


/*****************************************************************************
 函 数 名  : VGM_CFG_GatewayConnDecrement
 功能描述  : 删除当前网关的连接统计数
 输入参数  : ULONG ulVtId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_CFG_GatewayConnDecrement(ULONG ulVtId)
{
    if ( VGM_CFG_ARRAY_NUMS <= ulVtId)
    {
        return;
    }
    
    if(NULL ==  g_pstVgmCfgCtx->pastVgCfgArray[ulVtId] )
    {
        return;
    }
    
    VOS_InterlockedDecrement(&g_pstVgmCfgCtx->pastVgCfgArray[ulVtId]->ulConnNums);
}


/*****************************************************************************
 函 数 名  : VGM_CFG_CtxInit
 功能描述  : 配置申请
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VGM_CFG_CtxInit()
{
    ULONG ulIndex = 0;

    if ( NULL != g_pstVgmCfgCtx )
    {
        return VOS_OK;
    }
    
    /*配置申请*/
    g_pstVgmCfgCtx = (VGM_CFG_CTX_S *)VOS_Malloc(VGM_MID_SID_CFG,  sizeof(VGM_CFG_CTX_S));

    if ( NULL == g_pstVgmCfgCtx )
    {
        VOS_Printf("malloc error!");
        return VOS_ERR;
    }
    
    /*初始化指针数组*/
    for (ulIndex =0; ulIndex < VGM_CFG_ARRAY_NUMS; ulIndex++)
    {
        g_pstVgmCfgCtx->pastVgCfgArray[ulIndex] = NULL;
    }

    g_pstVgmCfgCtx->ulCurrentNums = 0;
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VGM_CFG_CtxUnInit
 功能描述  : 资源释放，内部要由用户自己保证
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月3日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VGM_CFG_CtxUnInit()
{
    if ( NULL == g_pstVgmCfgCtx )
    {
        VOS_Free((CHAR *)g_pstVgmCfgCtx);
        return;
    }
}


