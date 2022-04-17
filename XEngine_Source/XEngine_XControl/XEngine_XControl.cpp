#include "XControl_Hdr.h"

BOOL bIsRun = FALSE;
BOOL bExist = FALSE;
XLOG xhLog = NULL;
SOCKET hTCPSocket = 0;
SOCKET hUDPSocket = 0;
int m_nTaskSerial = 0;
XHANDLE xhRPCPacket = NULL;
XNETHANDLE xhRPCPool = 0;
XNETHANDLE xhRPCSocket = 0;
XNETHANDLE xhRPCHeart = 0;

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
		RfcComponents_HttpServer_DestroyEx(xhRPCPacket);
		ManagePool_Thread_NQDestroy(xhRPCPool);
		NetCore_TCPXCore_DestroyEx(xhRPCSocket);
		SocketOpt_HeartBeat_DestoryEx(xhRPCHeart);

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
	LPCSTR lpszHTTPCode = "./XControl_Config/HttpCode.types";
	LPCSTR lpszHTTPMime = "./XControl_Config/HttpMime.types";
	THREADPOOL_PARAMENT** ppSt_ListRPCParam;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVERCONFIG));
	memset(&st_APPConfig, '\0', sizeof(XENGINE_CONFIGAPP));
	memset(&st_EMailConfig, '\0', sizeof(XENGIEN_EMAILCONFIG));

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
	if (st_EMailConfig.bCreateEmail)
	{
		UCHAR tszEnBuffer[4096];
		CHAR tszDeBuffer[4096];
		LPCSTR lpszSrcFile = "./XControl_Config/XControl_EMail.ini";
		LPCSTR lpszDstFile = "./XControl_Config/XControl_EMail.ini.dat";

		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
		memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

		FILE* pSt_DeFile = fopen(lpszSrcFile, "rb");
		FILE* pSt_EnFile = fopen(lpszDstFile, "wb");

		if (NULL == pSt_EnFile || NULL == pSt_DeFile)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，检测到加密脚本失败，读取文件失败，错误:%d", errno);
			goto NETSERVICE_APPEXIT;
		}
		int nDRet = fread(tszDeBuffer, 1, sizeof(tszDeBuffer), pSt_DeFile);
		if (nDRet <= 0)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，检测到加密脚本失败，读取数据失败，错误:%d", errno);
			goto NETSERVICE_APPEXIT;
		}
		if (!OPenSsl_XCrypto_Encoder(tszDeBuffer, &nDRet, tszEnBuffer, "xengineemail"))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，解密电子邮件信息失败,无法发送邮件信息，错误:%lX", OPenSsl_GetLastError());
			goto NETSERVICE_APPEXIT;
		}
		fwrite(tszEnBuffer, 1, nDRet, pSt_EnFile);
		fclose(pSt_EnFile);
		fclose(pSt_DeFile);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建加密电子邮箱信息成功,程序退出.");
		goto NETSERVICE_APPEXIT;
	}

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

	if (0 != _access(st_ServiceConfig.tszTmpFile, 0))
	{
		if (XControl_Parament_EMail())
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

				XNETHANDLE xhSmtp;
				if (!RfcComponents_EMailClient_SmtpInit(&xhSmtp, &st_EMailConfig.st_EMailSmtp))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，初始化SMTP服务失败,错误:%lX", EMailClient_GetLastError());
					goto NETSERVICE_APPEXIT;
				}
				if (!XControl_Info_HardWare(tszHWInfo, &nHWLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，获取系统硬件信息失败,错误:%lX", XInfo_GetLastError());
					goto NETSERVICE_APPEXIT;
				}
				if (!XControl_Info_SoftWare(tszSWInfo, &nSWLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，获取系统软件信息失败,错误:%lX", XInfo_GetLastError());
					goto NETSERVICE_APPEXIT;
				}
				sprintf(tszRPInfo, "%s\r\n%s", tszSWInfo, tszHWInfo);
				RfcComponents_EMailClient_SmtpSend(xhSmtp, st_EMailConfig.tszAddrList, "XEngine控制后台信息报告", tszRPInfo);
				RfcComponents_EMailClient_SmtpClose(xhSmtp);

				FILE* pSt_File = fopen(st_ServiceConfig.tszTmpFile, "wb");
				if (NULL == pSt_File)
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，写入临时文件失败,错误:%d", errno);
					goto NETSERVICE_APPEXIT;
				}
				fwrite(_T("XEngine"), 1, 7, pSt_File);
				fclose(pSt_File);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，投递系统信息消息成功");
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, "启动服务中，邮件配置不正确,无法投递信息!");
			}
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，检测到信息已经投递,不需要再次投递!");
	}
	//是否启用了客户端
	if (st_ServiceConfig.st_Client.bEnable)
	{
		if (IPPROTO_TCP == st_ServiceConfig.st_Client.nIPType)
		{
			if (!XClient_TCPSelect_Create(&hTCPSocket, st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建TCP连接失败,地址:%s,端口:%d,错误:%lX", st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort, XClient_GetLastError());
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建TCP连接成功,地址:%s,端口:%d", st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort);
			pSTDThread_TCP = make_shared<std::thread>(XControl_Thread_TCPTask);
			if (!pSTDThread_TCP->joinable())
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建TCP任务线程失败");
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建TCP任务线程成功");
		}
		else
		{
			if (!XClient_UDPSelect_Create(&hUDPSocket, st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建UDP连接失败,地址:%s,端口:%d,错误:%lX", st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort, XClient_GetLastError());
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建UDP连接成功,地址:%s,端口:%d", st_ServiceConfig.st_Client.tszIPAddr, st_ServiceConfig.st_Client.nPort);
			pSTDThread_UDP = make_shared<std::thread>(XControl_Thread_UDPTask);
			if (!pSTDThread_UDP->joinable())
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建UDP任务线程失败");
				goto NETSERVICE_APPEXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建UDP任务线程成功");
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, "启动服务中，客户端被设置为不自动连接");
	}
	//RPC服务
	xhRPCPacket = RfcComponents_HttpServer_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceConfig.st_XRpc.nThread);
	if (NULL == xhRPCPacket)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务器中，初始化HTTP组包失败，错误：%lX", HttpServer_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化HTTP组包成功，IO线程个数:%d", st_ServiceConfig.st_XRpc.nThread);

	if (st_ServiceConfig.st_XRpc.nTimeCheck > 0)
	{
		if (!SocketOpt_HeartBeat_InitEx(&xhRPCHeart, st_ServiceConfig.st_XRpc.nTimeOut, st_ServiceConfig.st_XRpc.nTimeCheck, XControl_Callback_RPCHeart))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "初始化RPC心跳服务失败，错误：%lX", NetCore_GetLastError());
			goto NETSERVICE_APPEXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化RPC心跳服务成功,句柄:%llu,时间:%d,次数:%d", xhRPCHeart, st_ServiceConfig.st_XRpc.nTimeOut, st_ServiceConfig.st_XRpc.nTimeCheck);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, "启动服务中，心跳服务被设置为不启用");
	}

	if (!NetCore_TCPXCore_StartEx(&xhRPCSocket, st_ServiceConfig.st_XRpc.nPort, st_ServiceConfig.st_XRpc.nClient, st_ServiceConfig.st_XRpc.nThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动RPC网络服务器失败，错误：%lX", NetCore_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，启动RPC网络服务器成功,RPC端口:%d,IO:%d", st_ServiceConfig.st_XRpc.nPort, st_ServiceConfig.st_XRpc.nThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhRPCSocket, XControl_Callback_RPCLogin, XControl_Callback_RPCRecv, XControl_Callback_RPCLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，注册RPC网络事件成功");

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListRPCParam, st_ServiceConfig.st_XRpc.nThread, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < st_ServiceConfig.st_XRpc.nThread; i++)
	{
		int* pInt_Pos = new int;

		*pInt_Pos = i;
		ppSt_ListRPCParam[i]->lParam = pInt_Pos;
		ppSt_ListRPCParam[i]->fpCall_ThreadsTask = XControl_RPCThread;
	}
	if (!ManagePool_Thread_NQCreate(&xhRPCPool, &ppSt_ListRPCParam, st_ServiceConfig.st_XRpc.nThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动RPC线程池服务失败，错误：%lX", ManagePool_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，启动RPC线程池服务成功,启动个数:%d", st_ServiceConfig.st_XRpc.nThread);
	//创建任务线程
	pSTDThread_Http = make_shared<std::thread>(XControl_Thread_HttpTask);
	if (!pSTDThread_Http->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，创建HTTP任务线程失败");
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，创建HTTP任务线程成功");

	pSTDThread_App = make_shared<std::thread>(APPManage_Thread_Process);
	if (!pSTDThread_App->joinable())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务器，启动进程守护线程失败，无法继续...");
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，初始化进程的守护线程成功");

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，所有服务已经启动完毕,程序运行中...");
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
		RfcComponents_HttpServer_DestroyEx(xhRPCPacket);
		ManagePool_Thread_NQDestroy(xhRPCPool);
		NetCore_TCPXCore_DestroyEx(xhRPCSocket);
		SocketOpt_HeartBeat_DestoryEx(xhRPCHeart);

		XClient_TCPSelect_Close(hTCPSocket);
		XClient_UDPSelect_Close(hUDPSocket);
		HelpComponents_XLog_Destroy(xhLog);
	}

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
