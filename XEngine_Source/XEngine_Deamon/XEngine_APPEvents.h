#pragma once

extern BOOL bIsService;
extern STARTUPINFO st_StartInfo;
extern PROCESS_INFORMATION st_ProcessInfo;

BOOL NetEngine_APPManage_RunService(LPCTSTR lpszServiceFile);
BOOL NetEngine_APPManage_CheckService();
BOOL NetEngine_APPManage_StopService();
BOOL NetEngine_APPManage_CheckProcessName();