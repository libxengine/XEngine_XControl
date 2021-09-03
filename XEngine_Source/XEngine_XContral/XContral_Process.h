#pragma once

BOOL APPManage_CreateService(XENGINE_APPINFO* pSt_APPInfo);
BOOL APPManage_CreateProcess(XENGINE_APPINFO* pSt_APPInfo, DWORD* pdwProcessID);
XHTHREAD APPManage_Thread_Process();
