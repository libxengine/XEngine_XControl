// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _WINDOWS
// 添加要在此处预编译的标头
#include "framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <WinSock2.h>
#include <json/json.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#ifdef _CENTOS
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif
#endif
#endif //PCH_H
#include <list>
#include <thread>
#include <memory>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include "XInfo_Define.h"
#include "XInfo_Error.h"
/********************************************************************
//	Created:	2017/5/26   10:25
//	Filename: 	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage\BackManage_CommHdr.h
//	File Path:	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage
//	File Base:	BackManage_CommHdr
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	后台管理服务公用头文件
//	History:
*********************************************************************/
extern BOOL BackManage_IsErrorOccur;
extern DWORD BackManage_dwErrorCode;

#ifdef _WINDOWS
#ifdef _WIN64
#pragma comment(lib,"x64/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x64/XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"x64/XEngine_NetHelp/NetHelp_APIHelp.lib")
#else
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x86/XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp.lib")
#endif
#endif
