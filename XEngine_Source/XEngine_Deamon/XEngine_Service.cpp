#include "XEngine_Hdr.h"
/*
功能说明：初始化
*/
void APPManageDeamon_Init()
{
	st_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	st_Status.dwCurrentState = SERVICE_STOPPED;
	st_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	st_Status.dwWin32ExitCode = 0;
	st_Status.dwServiceSpecificExitCode = 0;
	st_Status.dwCheckPoint = 0;
	st_Status.dwWaitHint = 0;
}
/*
功能说明:判断服务是否已经被安装
参数说明：无
返回值：BOOL类型
*/
BOOL APPManageDeamon_IsInstalled()
{
	BOOL bResult = FALSE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务
		SC_HANDLE hService = ::OpenService(hSCM, tszServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}
/*
功能说明:安装服务函数
参数说明：无
返回值：BOOL类型
*/
BOOL APPManageDeamon_Install()
{
	if (APPManageDeamon_IsInstalled())
	{
		return TRUE;
	}
	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		TCHAR tszMsgBuffer[MAX_PATH];
		memset(tszMsgBuffer, '\0', MAX_PATH);

		_stprintf_s(tszMsgBuffer, _T("Couldn't open service manager:%d"), GetLastError());
		MessageBox(NULL, tszMsgBuffer, tszServiceName, MB_OK);
		return FALSE;
	}
	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	//创建服务
	SC_HANDLE hService = CreateService(hSCM, tszServiceName, tszServiceName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, szFilePath, NULL, NULL, _T(""), NULL, NULL);
	if (hService == NULL)
	{
		CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), tszServiceName, MB_OK);
		return FALSE;
	}
	//设置描述
	SC_HANDLE hSCService = OpenService(hSCM, tszServiceName, SERVICE_CHANGE_CONFIG);
	if (NULL == hSCService)
	{
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hSCService);
		return FALSE;
	}
	SERVICE_DESCRIPTION st_ServiceDescription;
	SERVICE_DELAYED_AUTO_START_INFO st_ServiceDelayStart;

	st_ServiceDescription.lpDescription = (LPWSTR)_T("XEngine 管理服务,用于守护关键进程运行和保护系统");
	st_ServiceDelayStart.fDelayedAutostart = TRUE;

	ChangeServiceConfig2(hSCService, SERVICE_CONFIG_DESCRIPTION, &st_ServiceDescription);
	ChangeServiceConfig2(hSCService, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &st_ServiceDelayStart);
	//打开服务
	StartService(hSCService, 0, NULL);
	CloseServiceHandle(hSCService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	return TRUE;
}
/*
功能说明:删除服务函数
参数说明：无
返回值：BOOL类型
*/
BOOL APPManageDeamon_Uninstall()
{
	if (!APPManageDeamon_IsInstalled())
	{
		return TRUE;
	}

	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		TCHAR tszMsgBuffer[MAX_PATH];
		memset(tszMsgBuffer, '\0', MAX_PATH);

		_stprintf_s(tszMsgBuffer, _T("Couldn't open service manager:%d"), GetLastError());
		MessageBox(NULL, tszMsgBuffer, tszServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = OpenService(hSCM, tszServiceName, SERVICE_STOP | DELETE);
	if (hService == NULL)
	{
		CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), tszServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS st_Status;
	ControlService(hService, SERVICE_CONTROL_STOP, &st_Status);
	//删除服务
	BOOL bDelete = DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	if (!bDelete)
	{
		return FALSE;
	}
	return TRUE;
}