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
#include <tchar.h>
#include <WinSock2.h>
#include <json/json.h>
#else
#ifdef _CENTOS
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif
#endif
#endif //PCH_H
#include <string.h>
#include <memory>
#include <list>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include "../XControl_Protocol.h"
using namespace std;
#include "Protocol_Define.h"
#include "Protocol_Error.h"
/********************************************************************
//    Created:     2021/09/08  10:40:08
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\pch.h
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
extern BOOL Protocol_IsErrorOccur;
extern DWORD Protocol_dwErrorCode;

#ifdef _WINDOWS
#ifdef _DEBUG
#pragma comment(lib,"x86/XEngine_Core/XEngine_OPenSsl.lib")
#else
#ifdef _WIN64
#pragma comment(lib,"x64/XEngine_Core/XEngine_OPenSsl.lib")
#else
#pragma comment(lib,"x86/XEngine_Core/XEngine_OPenSsl.lib")
#endif
#endif
#endif