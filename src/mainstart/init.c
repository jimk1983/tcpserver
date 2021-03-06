/******************************************************************************

                  版权所有 (C), 2018-2028, 技术有限公司

 ******************************************************************************
  文 件 名   : init.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月5日
  最近修改   :
  功能描述   : 初始化
  函数列表   :
              SEV_EnvInit
              SEV_EnvUnInit
  修改历史   :
  1.日    期   : 2018年11月5日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#include "common.h"
#include "init.h"
/*管理面*/
#include "vgm/vgm_pub.h"
#include "fsm/fsm_pub.h"
#include "rds/rds_pub.h"
/*业务面*/
#include "swm/swm_pub.h"
#include "upfile/upf_pub.h"


/*****************************************************************************
 函 数 名  : Main_EnvInit
 功能描述  : 服务环境初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月5日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 Main_EnvInit()
{
    /*框架初始化*/
    if( VOS_ERR == RCT_API_EnvInit() )
    {
        return VOS_ERR;
    }

    /********管理面**********/
    /*网关管理*/
    if ( VOS_ERR == VGM_CtxInit() )
    {
        Main_EnvUnInit();
        return VOS_ERR;
    }

    /*文件资源管理*/
    if ( VOS_ERR == FSM_CtxInit() )
    {  
        Main_EnvUnInit();
        return VOS_ERR;
    }

    /*Redis服务器*/
    if ( VOS_ERR == RDS_CtxInit() )
    {  
        Main_EnvUnInit();
        return VOS_ERR;
    }
        
    /********业务面**********/
    /*1. SWM业务初始化*/
    if ( VOS_ERR == SWM_CtxInit() )
    {
        Main_EnvUnInit();
        return VOS_ERR;
    }
    /*2. UPF业务初始化*/
    if ( VOS_ERR == UPF_CtxInit() )
    {
        Main_EnvUnInit();
        return VOS_ERR;
    }

    return VOS_OK;
}



/*****************************************************************************
 函 数 名  : SEV_EnvUnInit
 功能描述  : 服务环境去初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月5日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID Main_EnvUnInit()
{
    VGM_CtxUnInit();
    SWM_CtxUnInit();
    FSM_CtxUnInit();
    RDS_CtxUnInit();
    UPF_CtxUnInit();
    RCT_API_EnvUnInit();
}



