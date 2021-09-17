#include "XEngine_Hdr.h"

XLOG xhLog = NULL;
DWORD dwThreadID = 0;
TCHAR tszServiceName[] = _T("XEngine_Deamon");
SERVICE_STATUS_HANDLE hServiceStatus = NULL;
STARTUPINFO st_StartInfo = { 0 };
SERVICE_STATUS st_Status = { 0 };
PROCESS_INFORMATION st_ProcessInfo = { 0 };

void WINAPI ServiceStrl(DWORD dwOpcode);
void WINAPI ServiceMain();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	APPManageDeamon_Init();
	dwThreadID = GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{ tszServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
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
		StartServiceCtrlDispatcher(st);
	}
	return 0;
}
void WINAPI ServiceMain()
{
	// Register the control request handler
	st_Status.dwCurrentState = SERVICE_START_PENDING;//SERVICE_RUNNING;//
	st_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	//注册服务控制
	hServiceStatus = RegisterServiceCtrlHandler(tszServiceName, ServiceStrl);
	if (hServiceStatus == NULL)
	{
		return;
	}
	SetServiceStatus(hServiceStatus, &st_Status);
	//////////////////////////////////////////////////////////////////////////
	TCHAR tszRunPath[MAX_PATH];
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(tszRunPath, '\0', MAX_PATH);
	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	//设置工作目录
	GetModuleFileName(NULL, tszRunPath, MAX_PATH);
	std::wstring m_StrPath(tszRunPath);
	m_StrPath = m_StrPath.substr(0, m_StrPath.rfind('\\'));
	SetCurrentDirectory(m_StrPath.c_str());
	//读取配置
	XENGINE_SERVERCONFIG st_ServiceConfig;
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVERCONFIG));
	LPCSTR lpszBaseCfg = "./XControl_Config/XEngine_Config.json";

	if (!Config_Json_File(lpszBaseCfg, &st_ServiceConfig))
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	st_XLogConfig.XLog_MaxBackupFile = st_ServiceConfig.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_ServiceConfig.st_XLog.nMaxSize;
	strcpy(st_XLogConfig.tszFileName, "./XControl_Log/XEngine_Deamon.log");
	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		return;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_ServiceConfig.st_XLog.nLogLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务完成");

	st_Status.dwWin32ExitCode = S_OK;
	st_Status.dwCheckPoint = 0;
	st_Status.dwWaitHint = 0;
	st_Status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &st_Status);
	//检测服务器是否启动。应用时将主要任务放于此即可
	while (TRUE)
	{
		if (st_Status.dwCurrentState == SERVICE_PAUSED || st_Status.dwCurrentState == SERVICE_PAUSE_PENDING)
		{
			Sleep(1000);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "服务暂停中...");
			continue;
		}
		else if (st_Status.dwCurrentState == SERVICE_STOPPED || st_Status.dwCurrentState == SERVICE_STOP_PENDING)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "关闭服务");
			XEngine_APPManage_StopService();
			break;
		}

		if (!XEngine_APPManage_CheckService())
		{
			USES_CONVERSION;
			XEngine_APPManage_RunService(A2W(st_ServiceConfig.tszAPPDeamon));
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "检查到程序不存在,启动程序");
		}
		Sleep(1000);
	}
	st_Status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &st_Status);
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
		st_Status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &st_Status);
		PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		break;
	case SERVICE_CONTROL_PAUSE:
		st_Status.dwCurrentState = SERVICE_PAUSED;
		SetServiceStatus(hServiceStatus, &st_Status);
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