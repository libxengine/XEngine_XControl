#pragma once
#include <tchar.h>
#include <list>
#include <string>
#include <atlconv.h>
#include <Windows.h>
#include <strsafe.h>
#include <aclapi.h>
#include <TlHelp32.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
using namespace std;
#include "../XControl_Configure/Config_Define.h"
#include "../XControl_Configure/Config_Error.h"
#include "XEngine_APPEvents.h"
#include "XEngine_Service.h"
/********************************************************************
//    Created:     2021/08/27  10:51:37
//    File Name:   D:\XEngine_XContral\XEngine_Source\XEngine_Deamon\XEngine_Hdr.h
//    File Path:   D:\XEngine_XContral\XEngine_Source\XEngine_Deamon
//    File Base:   XEngine_Hdr
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公有头文件
//    History:
*********************************************************************/

extern XLOG xhLog;
extern DWORD dwThreadID;
extern TCHAR tszServiceName[];
extern SERVICE_STATUS_HANDLE hServiceStatus;
extern STARTUPINFO st_StartInfo;
extern PROCESS_INFORMATION st_ProcessInfo;
extern SERVICE_STATUS st_Status;

#ifdef _MSC_BUILD
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../x64/Debug/XControl_Configure.lib")
#else
#pragma comment(lib,"../x64/Release/XControl_Configure.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../Debug/XControl_Configure.lib")
#else
#pragma comment(lib,"../Release/XControl_Configure.lib")
#endif
#endif
#endif