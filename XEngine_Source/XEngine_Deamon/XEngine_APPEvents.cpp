#include "XEngine_Hdr.h"

BOOL bIsService = FALSE;
STARTUPINFO st_StartInfo;
PROCESS_INFORMATION st_ProcessInfo;

BOOL NetEngine_APPManage_RunService(LPCTSTR lpszServiceFile)
{
	if (NULL == lpszServiceFile)
	{
		return FALSE;
	}
	if (bIsService)
	{
		return TRUE;
	}
	if (!NetEngine_APPManage_CheckProcessName())
	{
		return FALSE;
	}
	HANDLE hTokenThis = NULL;
	HANDLE hTokenDup = NULL;
	HANDLE hThisProcess = GetCurrentProcess();
	OpenProcessToken(hThisProcess, TOKEN_ALL_ACCESS, &hTokenThis);
	DuplicateTokenEx(hTokenThis, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hTokenDup);
	DWORD dwSessionId = WTSGetActiveConsoleSessionId();

	SetTokenInformation(hTokenDup, TokenSessionId, &dwSessionId, sizeof(DWORD));
	memset(&st_ProcessInfo, '\0', sizeof(PROCESS_INFORMATION));
	memset(&st_StartInfo, '\0', sizeof(STARTUPINFO));

	st_StartInfo.cb = sizeof(STARTUPINFO);
	st_StartInfo.wShowWindow = SW_HIDE;
	st_StartInfo.dwFlags = STARTF_USESHOWWINDOW;

	if (!CreateProcessAsUser(hTokenDup, lpszServiceFile, NULL, NULL,NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &st_StartInfo, &st_ProcessInfo))
	{
		return FALSE;
	}
	bIsService = TRUE;

	return TRUE;
}
BOOL NetEngine_APPManage_CheckService()
{
	if (!bIsService)
	{
		return FALSE;
	}
	DWORD dwRet = 0;
	if (!GetExitCodeProcess(st_ProcessInfo.hProcess, &dwRet))
	{
		dwRet = GetLastError();
		NetEngine_APPManage_StopService();
		return FALSE;
	}
	if (STILL_ACTIVE != dwRet)
	{
		NetEngine_APPManage_StopService();
		return FALSE;
	}
	return TRUE;
}
BOOL NetEngine_APPManage_StopService()
{
	if (!bIsService)
	{
		return TRUE;
	}
	bIsService = FALSE;

	TerminateProcess(st_ProcessInfo.hProcess, 1);
	CloseHandle(st_ProcessInfo.hThread);
	CloseHandle(st_ProcessInfo.hProcess);

	return TRUE;
}
BOOL NetEngine_APPManage_CheckProcessName()
{
	PROCESSENTRY32 st_ProcessEntry;
	memset(&st_ProcessEntry, '\0', sizeof(PROCESSENTRY32));

	st_ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, TH32CS_SNAPPROCESS);
	if (!Process32First(hSnapshot, &st_ProcessEntry))
	{
		return FALSE;
	}
	while (TRUE)
	{
		st_ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32Next(hSnapshot, &st_ProcessEntry))
		{
			break;
		}
		if (0 == _tcscmp(st_ProcessEntry.szExeFile, _T("NetEngine_APPManageService.exe")))
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, st_ProcessEntry.th32ProcessID);
			if (NULL == hProcess)
			{
				return FALSE;
			}
			if (!TerminateProcess(hProcess, 1))
			{
				return FALSE;
			}
			CloseHandle(hProcess);
		}
	}
	CloseHandle(hSnapshot);
	return TRUE;
}