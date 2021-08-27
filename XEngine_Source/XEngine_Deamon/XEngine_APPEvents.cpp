#include "XEngine_Hdr.h"

BOOL XEngine_APPManage_RunService(LPCTSTR lpszServiceFile)
{
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

	return TRUE;
}
BOOL XEngine_APPManage_CheckService()
{
	DWORD dwRet = 0;
	if (!GetExitCodeProcess(st_ProcessInfo.hProcess, &dwRet))
	{
		dwRet = GetLastError();
		XEngine_APPManage_StopService();
		return FALSE;
	}
	if (STILL_ACTIVE != dwRet)
	{
		XEngine_APPManage_StopService();
		return FALSE;
	}
	return TRUE;
}
BOOL XEngine_APPManage_StopService()
{
	TerminateProcess(st_ProcessInfo.hProcess, 1);
	CloseHandle(st_ProcessInfo.hThread);
	CloseHandle(st_ProcessInfo.hProcess);

	return TRUE;
}