#include "XControl_Hdr.h"

XHTHREAD XControl_Thread_HttpTask()
{
	time_t nTimeStart = time(NULL);

	while (bIsRun)
	{
		int nBLen = 0;
		CHAR* ptszMsgBody = NULL;
		APIHELP_HTTPPARAMENT st_HTTPParam;

		memset(&st_HTTPParam, '\0', sizeof(APIHELP_HTTPPARAMENT));

		st_HTTPParam.nTimeConnect = 2;
		if (APIHelp_HttpRequest_Get(st_ServiceConfig.tszTaskUrl, &ptszMsgBody, &nBLen, NULL, NULL, NULL, &st_HTTPParam))
	    {
			nTimeStart = time(NULL);//更新
			XControl_Task_ProtocolParse(ptszMsgBody, nBLen);
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBody);
		std::this_thread::sleep_for(std::chrono::seconds(st_ServiceConfig.st_Time.nHTTPThreadTime));
	}
	return 0;
}
XHTHREAD XControl_Thread_TCPTask()
{
	while (bIsRun)
	{
		int nMsgLen = 0;
		CHAR* ptszMsgBuffer = NULL;
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		if (XClient_TCPSelect_RecvPkt(hTCPSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			XControl_Task_ProtocolParse(ptszMsgBuffer, nMsgLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(st_ServiceConfig.st_Time.nTCPThreadTime));
	}
	return 0;
}
XHTHREAD XControl_Thread_UDPTask()
{
	while (bIsRun)
	{
		int nMsgLen = 0;
		CHAR* ptszMsgBuffer = NULL;
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
		if (XClient_TCPSelect_RecvPkt(hUDPSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			XControl_Task_ProtocolParse(ptszMsgBuffer, nMsgLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(st_ServiceConfig.st_Time.nUDPThreadTime));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
BOOL XControl_Task_ProtocolParse(LPCSTR lpszMsgBuffer, int nMsgLen)
{
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());
	JSONCPP_STRING st_JsonError;

	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:JSON解析错误");
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;
	//获得任务序列号
	int nTaskSerial = st_JsonRoot["nTaskSerial"].asUInt();
	if (nTaskSerial <= m_nTaskSerial)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:任务序列号不正确");
		return FALSE;
	}
	m_nTaskSerial = nTaskSerial;
	//执行任务
	switch (st_JsonRoot["unOperatorCode"].asUInt())
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE:
	{
		XNETHANDLE xhTask;
		CHAR tszFileUrl[MAX_PATH];
		CHAR tszSaveUrl[MAX_PATH];

		memset(tszFileUrl, '\0', MAX_PATH);
		memset(tszSaveUrl, '\0', MAX_PATH);
		strcpy(tszFileUrl, st_JsonRoot["DownloadUrl"].asCString());
		strcpy(tszSaveUrl, st_JsonRoot["SaveUrl"].asCString());

		if (!DownLoad_Http_Create(&xhTask, tszFileUrl, tszSaveUrl))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:下载任务处理失败,错误码:%lX", Download_GetLastError());
			return FALSE;
		}
		while (TRUE)
		{
			NETDOWNLOAD_TASKINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETDOWNLOAD_TASKINFO));

			if (DownLoad_Http_Query(xhTask, &st_TaskInfo))
			{
				if (ENUM_XENGINE_DOWNLOAD_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus)
				{
					break;
				}
			}
		}
		DownLoad_Http_Delete(xhTask);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEFILE:
		CHAR tszDelFile[MAX_PATH];
		memset(tszDelFile, '\0', MAX_PATH);

		strcpy(tszDelFile, st_JsonRoot["DeleteFile"].asCString());
		if (-1 == remove(tszDelFile))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:删除文件任务处理失败,错误码:%d", errno);
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR:
		CHAR tszDelDir[MAX_PATH];
		memset(tszDelDir, '\0', MAX_PATH);

		strcpy(tszDelDir, st_JsonRoot["DeleteDir"].asCString());
		if (!SystemApi_File_DeleteMutilFolder(tszDelDir))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:删除文件夹任务处理失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_UPFILE:
	{
		XNETHANDLE xhTask;
		CHAR tszUPFile[MAX_PATH];
		CHAR tszUPUrl[MAX_PATH];

		memset(tszUPFile, '\0', MAX_PATH);
		memset(tszUPUrl, '\0', MAX_PATH);
		strcpy(tszUPFile, st_JsonRoot["UPLoadFile"].asCString());
		strcpy(tszUPUrl, st_JsonRoot["UPLoadUrl"].asCString());

		if (!DownLoad_Ftp_Create(&xhTask, tszUPFile, tszUPUrl, FALSE, FALSE))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:FTP上传任务处理失败,错误码:%lX", Download_GetLastError());
			return FALSE;
		}
		while (TRUE)
		{
			NETDOWNLOAD_TASKINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETDOWNLOAD_TASKINFO));

			if (DownLoad_Ftp_Query(xhTask, &st_TaskInfo))
			{
				if (ENUM_XENGINE_DOWNLOAD_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus)
				{
					break;
				}
			}
		}
		DownLoad_Ftp_Delete(xhTask);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_GETLIST:
	{
		int nListCount = 0;
		CHAR** ppszFileList;
		if (SystemApi_File_EnumFile(st_JsonRoot["FindPath"].asCString(), &ppszFileList, &nListCount))
		{
			XControl_Handle_PostListFile(ppszFileList, nListCount, st_JsonRoot["PostUrl"].asCString());
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppszFileList, nListCount);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求文件列表失败,错误码:%lX", SystemApi_GetLastError());
		}
	}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_EXEC:
	{
		DWORD dwProcessID = 0;
		if (SystemApi_Process_CreateProcess(&dwProcessID, st_JsonRoot["ExecFile"].asCString(),NULL, st_JsonRoot["ExecShow"].asInt()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "HTTP任务:请求创建进程成功,进程:%s", st_JsonRoot["ExecFile"].asCString());
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求创建进程:%s 失败,错误码:%lX", st_JsonRoot["ExecFile"].asCString(), SystemApi_GetLastError());
		}
	}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_POPMESSAGE:
	{
#ifdef _MSC_BUILD
		MessageBoxA(NULL, st_JsonRoot["MessageBox"].asCString(), "提示", MB_OK);
#else
#endif
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "HTTP任务:请求弹出消息:%s", st_JsonRoot["MessageBox"].asCString());
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS:
		if (!SystemApi_Process_Stop(st_JsonRoot["tszProcessName"].asCString()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求停止进程失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN:
		if (!SystemApi_System_Shutdown((DWORD)st_JsonRoot["ShutDownType"].asInt64()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求关机失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD:
		if (-1 == system(st_JsonRoot["tszExecCmd"].asCString()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求执行命令失败,错误码:%lX", SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_CONNECT:
		if (IPPROTO_TCP == st_JsonRoot["nIPType"].asUInt())
		{
			XClient_TCPSelect_Close(hTCPSocket);
			XClient_TCPSelect_Create(&hTCPSocket, st_JsonRoot["tszIPAddr"].asCString(), st_JsonRoot["nPort"].asInt());
		}
		else
		{
			XClient_UDPSelect_Close(hUDPSocket);
			XClient_UDPSelect_Create(&hUDPSocket, st_JsonRoot["tszIPAddr"].asCString(), st_JsonRoot["nPort"].asInt());
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_USER:
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_NOTHINGTODO:
		break;
	default:
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:请求的操作码不支持,操作码:%d", st_JsonRoot["unOperatorCode"].asUInt());
		return FALSE;
	}
	return TRUE;
}