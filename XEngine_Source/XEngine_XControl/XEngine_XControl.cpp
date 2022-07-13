#include "XControl_Hdr.h"

BOOL bIsRun = FALSE;
BOOL bExist = FALSE;
XLOG xhLog = NULL;
SOCKET hTCPSocket = 0;
SOCKET hUDPSocket = 0;
int m_nTaskSerial = 0;

shared_ptr<std::thread> pSTDThread_Http = NULL;
shared_ptr<std::thread> pSTDThread_TCP = NULL;
shared_ptr<std::thread> pSTDThread_UDP = NULL;
shared_ptr<std::thread> pSTDThread_App = NULL;
XENGINE_SERVERCONFIG st_ServiceConfig;
XENGINE_CONFIGAPP st_APPConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, "服务器退出...");
		bIsRun = FALSE;

		if (NULL != pSTDThread_Http)
		{
			pSTDThread_Http->join();
		}
		if (NULL != pSTDThread_TCP)
		{
			pSTDThread_TCP->join();
		}
		if (NULL != pSTDThread_UDP)
		{
			pSTDThread_UDP->join();
		}
		if (NULL != pSTDThread_App)
		{
			pSTDThread_App->join();
		}
		XClient_TCPSelect_Close(hTCPSocket);
		XClient_UDPSelect_Close(hUDPSocket);
		HelpComponents_XLog_Destroy(xhLog);
		bExist = TRUE;
		exit(0);
	}
}
int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	LPCSTR lpszWndName = "XEngine_XControlApp";
#endif
	bIsRun = TRUE;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVERCONFIG));
	memset(&st_APPConfig, '\0', sizeof(XENGINE_CONFIGAPP));

	if (!XControl_Parament(argc, argv))
	{
		printf("初始化参数失败,错误:%lX!\n", Config_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	st_XLogConfig.XLog_MaxBackupFile = st_ServiceConfig.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_ServiceConfig.st_XLog.nMaxSize;
	strcpy(st_XLogConfig.tszFileName, st_ServiceConfig.st_XLog.tszLogFile);
	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("初始化日志服务失败,无法继续!\n");
		goto NETSERVICE_APPEXIT;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_ServiceConfig.st_XLog.nLogLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化日志系统成功");

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化信号处理成功");
	if (st_ServiceConfig.bAutoStart)
	{
		if (!SystemApi_Process_AutoStart("XEngine", "XEngine_XControl"))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，注册软件开机启动失败!错误:%lX", SystemApi_GetLastError());
			goto NETSERVICE_APPEXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，注册软件开机启动成功");
	}
	if (st_ServiceConfig.bHideWnd)
	{
#ifdef _MSC_BUILD
		BOOL bIsFound = FALSE;
		SetConsoleTitleA(lpszWndName);
		HWND hWnd = GetDesktopWindow();
		hWnd = GetWindow(hWnd, GW_CHILD);
		CHAR tszTitle[MAX_PATH];
		while (NULL != hWnd)
		{
			memset(tszTitle, '\0', MAX_PATH);
			GetWindowTextA(hWnd, tszTitle, MAX_PATH);
			if (0 == strncmp(lpszWndName, tszTitle, strlen(lpszWndName)))
			{
				bIsFound = TRUE;
				break;
			}
			hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
		}
		if (bIsFound)
		{
			ShowWindow(hWnd, SW_HIDE);
		}
#endif
	}
	//启用TCP客户端线程
	pSTDThread_TCP = make_shared<std::thread>(XControl_Thread_TCPTask);
	if (!pSTDThread_TCP->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建TCP任务线程失败");
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建TCP任务线程成功");
	//启用UDP客户端线程
	pSTDThread_UDP = make_shared<std::thread>(XControl_Thread_UDPTask);
	if (!pSTDThread_UDP->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建UDP任务线程失败");
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建UDP任务线程成功");
	//启用HTTP客户端
	pSTDThread_Http = make_shared<std::thread>(XControl_Thread_HttpTask);
	if (!pSTDThread_Http->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建HTTP任务线程失败");
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建HTTP任务线程成功");
	//启用进程守护线程
	pSTDThread_App = make_shared<std::thread>(APPManage_Thread_Process);
	if (!pSTDThread_App->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务器，启动进程守护线程失败，无法继续...");
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化进程的守护线程成功");

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，所有服务已经启动完毕,程序运行中,XEngine版本:%s,服务器发行次数:%d,版本;%s...", BaseLib_OperatorVer_XGetStr(), st_ServiceConfig.st_Version.pStl_ListVer->size(), st_ServiceConfig.st_Version.pStl_ListVer->front().c_str());
	while (TRUE)
	{
		if (bExist)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
NETSERVICE_APPEXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, "后台控制服务关闭，服务器退出...");
		bIsRun = FALSE;

		if (NULL != pSTDThread_Http)
		{
			pSTDThread_Http->join();
		}
		if (NULL != pSTDThread_TCP)
		{
			pSTDThread_TCP->join();
		}
		if (NULL != pSTDThread_UDP)
		{
			pSTDThread_UDP->join();
		}
		if (NULL != pSTDThread_App)
		{
			pSTDThread_App->join();
		}
		XClient_TCPSelect_Close(hTCPSocket);
		XClient_UDPSelect_Close(hUDPSocket);
		HelpComponents_XLog_Destroy(xhLog);
	}

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
