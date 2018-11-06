/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : main.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月5日
  最近修改   :
  功能描述   : 升级服务器
  函数列表   :
              main
  修改历史   :
  1.日    期   : 2018年11月5日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#include "common.h"
#include "init.h"

int main()
{
    VOS_SM_T stVsmStop={0};
    
    /*停止信号*/
    if( VOS_ERR == VOS_SM_Init(&stVsmStop) )
    {
        VOS_PrintDebug("VOS Semaphore stop error");
        return -1;
    }
    
    /*1. 服务端的环境初始化*/
    if( VOS_ERR == Main_EnvInit())
    {
        VOS_Printf("Main enviroment initialize error!\n");
        return VOS_ERR;
    }

    /*2. 启动运行*/
    if( VOS_ERR == RCT_API_EnvRun())
    {
        VOS_Printf("Reactor enviroment running error!\n");
        return VOS_ERR;
    }
    
    /*顺便测试使用VOS信号量*/
    VOS_SM_P(&stVsmStop,0);
    Main_EnvUnInit();
    VOS_SM_Destroy(&stVsmStop);
    VOS_Printf("Main pthread End!\n");
    
    return VOS_OK;
}
