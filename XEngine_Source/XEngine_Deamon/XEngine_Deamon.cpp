#include "XEngine_Hdr.h"
//定义全局函数变量
void APPManageDeamon_Init();
BOOL APPManageDeamon_IsInstalled();
BOOL APPManageDeamon_Install();
BOOL APPManageDeamon_Uninstall();
void WINAPI ServiceMain();
void WINAPI ServiceStrl(DWORD dwOpcode);

TCHAR szServiceName[] = _T("NetEngine_Deamon");
BOOL bInstall = FALSE;
SERVICE_STATUS_HANDLE hServiceStatus = NULL;
SERVICE_STATUS status = { 0 };
DWORD dwThreadID = 0;

/*
功能说明：初始化
*/
void APPManageDeamon_Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_STOPPED;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
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
		SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
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
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}
	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务
	SC_HANDLE hService = CreateService(hSCM, szServiceName, szServiceName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, szFilePath, NULL, NULL, _T(""), NULL, NULL);
	if (hService == NULL)
	{
		CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), szServiceName, MB_OK);
		return FALSE;
	}
	//设置描述
	SC_HANDLE hSCService = OpenService(hSCM, szServiceName, SERVICE_CHANGE_CONFIG);
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
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);
	if (hService == NULL)
	{
		CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	ControlService(hService, SERVICE_CONTROL_STOP, &status);
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
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	APPManageDeamon_Init();

	dwThreadID = GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{ szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (_stricmp(lpCmdLine, "/install") == 0)
	{
		APPManageDeamon_Install();
	}
	else if (_stricmp(lpCmdLine, "/uninstall") == 0)
	{
		APPManageDeamon_Uninstall();
	}
	else
	{
		//bRet = StartServerWork();
		StartServiceCtrlDispatcher(st);
	}
	return 0;
}
/*
功能说明：服务主函数，这在里进行控制对服务控制的注册
*/
void WINAPI ServiceMain()
{
	// Register the control request handler
	status.dwCurrentState = SERVICE_START_PENDING;//SERVICE_RUNNING;//
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	//注册服务控制
	hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);
	if (hServiceStatus == NULL)
	{
		return;
	}
	SetServiceStatus(hServiceStatus, &status);
	//////////////////////////////////////////////////////////////////////////开始
	TCHAR tszFilePath[MAX_PATH];
	TCHAR tszDrive[MAX_PATH];
	TCHAR tszDir[MAX_PATH];
	TCHAR tszFileName[MAX_PATH];
	TCHAR tszFileExt[MAX_PATH];
	TCHAR tszRunPath[MAX_PATH];

	memset(tszFilePath, '\0', MAX_PATH);
	memset(tszDrive, '\0', MAX_PATH);
	memset(tszDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	memset(tszFileExt, '\0', MAX_PATH);
	memset(tszRunPath, '\0', MAX_PATH);

	int nLen = GetModuleFileName(NULL, tszFilePath, MAX_PATH);
	_tsplitpath_s(tszFilePath, tszDrive, tszDir, tszFileName, tszFileExt);

	_stprintf_s(tszRunPath, _T("%s%s"), tszDrive, tszDir);
	SetCurrentDirectory(tszRunPath);
	memset(tszRunPath, '\0', MAX_PATH);
	nLen = _stprintf_s(tszRunPath, _T("%s%sNetEngine_APPManageService.exe"), tszDrive, tszDir);

	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);
	//检测服务器是否启动。应用时将主要任务放于此即可

	while (true)
	{
		if (status.dwCurrentState == SERVICE_PAUSED || status.dwCurrentState == SERVICE_PAUSE_PENDING)
		{
			Sleep(500);
			continue;
		}
		else if (status.dwCurrentState == SERVICE_STOPPED || status.dwCurrentState == SERVICE_STOP_PENDING)
		{
			NetEngine_APPManage_StopService();
			break;
		}

		if (!NetEngine_APPManage_CheckService())
		{
			NetEngine_APPManage_RunService(tszRunPath);
		}

		Sleep(1000);
	}
	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
}
/*
功能说明：	服务控制主函数，这里实现对服务的控制，当在服务管理器上停止或其它操作时，将会运行此处代码
参数说明:	dwOpcode，控制服务的状态
返回值：无
*/
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &status);
		PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		break;
	case SERVICE_CONTROL_PAUSE:
		status.dwCurrentState = SERVICE_PAUSED;
		SetServiceStatus(hServiceStatus, &status);
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		break;
	}
}