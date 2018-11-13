/******************************************************************************

                  版权所有 (C), 2018-2028, 有限公司

 ******************************************************************************
  文 件 名   : fsm_config.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月9日
  最近修改   :
  功能描述   : 文件相关的配置管理
  函数列表   :
  修改历史   :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
    
#include "common.h"
#include "fsm/fsm_pub.h"

const FSM_CONF_INFO_S g_starryFsmConfig[]=
{
    {
        "/testSmall.xml"
    },
    {
        "/testBig.xml"
    },
    {
        "/TerminalCollector.exe"
    },
};

/*****************************************************************************
 函 数 名  : FSM_Conf_GetFileNums
 功能描述  : 获取当前的数量
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32   FSM_Conf_GetFileNums()
{
    INT32 uiNums = VOS_ERR;

    uiNums = sizeof(g_starryFsmConfig)/sizeof(FSM_CONF_INFO_S);

    return uiNums;
}


/*****************************************************************************
 函 数 名  : FSM_Conf_GetFileNameByIndex
 功能描述  : 获取当前的配置文件
 输入参数  : UINT32 iIndex  
 输出参数  : 无
 返 回 值  : CHAR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
const CHAR   *FSM_Conf_GetFileNameByIndex(UINT32 iIndex)
{
    const CHAR *pcFileName = NULL;
    
    if ( iIndex >= FSM_Conf_GetFileNums()  )
    {
        return NULL;
    }

    pcFileName = g_starryFsmConfig[iIndex].acFileName;
       
    return pcFileName;
}


