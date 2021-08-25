#include "XContral_Hdr.h"

BOOL APPManage_CreateService(XENGINE_APPINFO *pSt_APPInfo)
{
	TCHAR tszCmdExe[1024];
	memset(tszCmdExe, '\0', sizeof(tszCmdExe));

#ifdef _WINDOWS
	_stprintf_s(tszCmdExe, _T("sc stop %s"), pSt_APPInfo->tszAPPName);
#else
	_stprintf_s(tszCmdExe, _T("systemctl restart %s"), pSt_APPInfo->tszAPPName);
#endif
	if (-1 == system(tszCmdExe))
	{
		return FALSE;
	}
#ifdef _WINDOWS
	memset(tszCmdExe, '\0', sizeof(tszCmdExe));
	_stprintf_s(tszCmdExe, _T("sc start %s"), pSt_APPInfo->tszAPPName);
	if (-1 == system(tszCmdExe))
	{
		return FALSE;
	}
#endif
	return TRUE;
}
BOOL APPManage_CreateProcess(XENGINE_APPINFO* pSt_APPInfo, DWORD* pdwProcessID)
{
	TCHAR tszCmdExe[1024];
	memset(tszCmdExe, '\0', sizeof(tszCmdExe));

	SystemApi_Process_Stop(pSt_APPInfo->tszAPPName);
	_stprintf_s(tszCmdExe, _T("%s%s"), pSt_APPInfo->tszAPPPath, pSt_APPInfo->tszAPPName);
	if (!SystemApi_Process_CreateProcess(pdwProcessID, tszCmdExe))
	{
		return FALSE;
	}
	return TRUE;
}

XHTHREAD APPManage_Thread_Process()
{
	while (bIsRun)
	{
		//轮询检测进程列表
		list<XENGINE_APPINFO>::iterator stl_ListIterator = st_APPConfig.pStl_ListApp->begin();
		for (; stl_ListIterator != st_APPConfig.pStl_ListApp->end(); stl_ListIterator++)
		{
			//是否启用
			if (!stl_ListIterator->bEnable)
			{
				continue;
			}
			//重试次数判断
			if (stl_ListIterator->nErrorTime > st_ServiceConfig.st_Time.nErrorTime)
			{
				stl_ListIterator->bEnable = FALSE;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("服务名：%s，由于超过指定启动失败次数：%d，这个服务检测功能被关闭..."), stl_ListIterator->tszAPPName, st_ServiceConfig.st_Time.nErrorTime);
				continue;
			}
			XENGINE_APPINFO st_APPInfo = *stl_ListIterator;

			if (stl_ListIterator->nReTime > 0)
			{
				//进程自动重启
				__int64x nNowTime = time(NULL);
				if ((nNowTime - stl_ListIterator->nStartTime) > stl_ListIterator->nReTime)
				{
					if (stl_ListIterator->bService)
					{
						if (APPManage_CreateService(&st_APPInfo))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("自动重启,重启服务：%s 成功..."), stl_ListIterator->tszAPPName);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("自动重启,重启服务：%s 失败，错误码：%d..."), stl_ListIterator->tszAPPName, errno);
						}
					}
					else
					{
						DWORD dwProcessId = 0;
						if (APPManage_CreateProcess(&st_APPInfo, &dwProcessId))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("自动重启,重启进程：%s 成功，进程ID：%d..."), stl_ListIterator->tszAPPName, dwProcessId);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("自动重启,重启进程：%s 失败，错误码：%lX..."), stl_ListIterator->tszAPPName, SystemApi_GetLastError());
						}
					}
					stl_ListIterator->nStartTime = time(NULL);
				}
			}
			else if (stl_ListIterator->bAutoStart)
			{
				//进程不存在才启动
				if (!SystemApi_Process_IsExist(st_APPInfo.tszAPPName))
				{
					if (stl_ListIterator->bService)
					{
						if (APPManage_CreateService(&st_APPInfo))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("崩溃重启,检查到服务不存在,启动服务：%s 成功..."), stl_ListIterator->tszAPPName);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("崩溃重启,检查到服务不存在,启动服务：%s 失败，错误码：%d..."), stl_ListIterator->tszAPPName, errno);
						}
					}
					else
					{
						DWORD dwProcessId = 0;
						if (APPManage_CreateProcess(&st_APPInfo, &dwProcessId))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("崩溃重启,检查到进程不存在,启动进程：%s 成功，进程ID：%d..."), stl_ListIterator->tszAPPName, dwProcessId);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("崩溃重启,检查到进程不存在,启动进程：%s 失败，错误码：%lX..."), stl_ListIterator->tszAPPName, SystemApi_GetLastError());
						}
					}
				}
			}
			else
			{
				//如果没设置，那么表示启动一次就不用管了
				if (!SystemApi_Process_IsExist(st_APPInfo.tszAPPName))
				{
					stl_ListIterator->bEnable = FALSE; //设置为已经执行，不在执行此命令
					if (stl_ListIterator->bService)
					{
						if (APPManage_CreateService(&st_APPInfo))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("其他检查,检查到服务不存在,启动服务：%s 成功..."), stl_ListIterator->tszAPPName);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("其他检查,检查到服务不存在,启动服务：%s 失败，错误码：%d..."), stl_ListIterator->tszAPPName, errno);
						}
					}
					else
					{
						DWORD dwProcessId = 0;
						if (APPManage_CreateProcess(&st_APPInfo, &dwProcessId))
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("其他检查,检查到进程不存在,启动进程：%s 成功，进程ID：%d..."), stl_ListIterator->tszAPPName, dwProcessId);
						}
						else
						{
							stl_ListIterator->nErrorTime++;
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("其他检查,检查到进程不存在,启动进程：%s 失败，错误码：%lX..."), stl_ListIterator->tszAPPName, SystemApi_GetLastError());
						}
					}
				}
			}
		}
		//休眠用户配置的时间
		std::this_thread::sleep_for(std::chrono::seconds(st_ServiceConfig.st_Time.nCheckTime));
	}
	return 0;
}
