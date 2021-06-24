#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <json/json.h>
#else
#include <unistd.h>
#ifdef _CENTOS
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <list>
#include <thread>
#include <memory>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Error.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Define.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../XContral_Infomation/XInfo_Define.h"
#include "../XContral_Infomation/XInfo_Error.h"

typedef struct tag_ManageService_Config
{
    TCHAR tszTaskUrl[MAX_PATH];
    BOOL bIsAutoStart;
    BOOL bIsHideWnd;
    BOOL bCreateEmail;
    int nTaskTime;
    int nLogType;
    struct  
    {
        TCHAR tszServiceAddr[64];
        int nIPType;
        int nPort;
        BOOL bEnable;
    }st_Client;
    struct
    {
        RFCCOMPONENTS_EMAILSMTP st_EMailSmtp;
        BOOL bEnable;

        list<tstring>* pStl_ListAddr;
    }st_EMail;
}MANAGESERVICE_CONFIG;

extern BOOL bIsRun;
extern XLOG xhLog;
extern SOCKET hTCPSocket;
extern SOCKET hUDPSocket;
extern int m_nTaskSerial;
extern shared_ptr<std::thread> pSTDThread_Http;
extern shared_ptr<std::thread> pSTDThread_TCP;
extern shared_ptr<std::thread> pSTDThread_UDP;
extern MANAGESERVICE_CONFIG st_ServiceConfig;

#include "XContral_Config.h"
#include "XContral_Handle.h"
#include "XContral_Task.h"
#include "XContral_Network.h"

#ifdef _WINDOWS
#pragma comment(lib,"Ws2_32.lib")
#ifdef _WIN64
#pragma comment(lib,"../x64/Release/XContral_Infomation.lib")
#pragma comment(lib,"x64/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x64/XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"x64/XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"x64/XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"x64/XEngine_RfcComponents/RfcComponents_EmailClient.lib")
#pragma comment(lib,"x64/XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"x64/XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"x64/XEngine_DownLoad/XEngine_DownLoad.lib")
#else
#pragma comment(lib,"../Debug/XContral_Infomation.lib")
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"x86/XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"x86/XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"x86/XEngine_RfcComponents/RfcComponents_EmailClient.lib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"x86/XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"x86/XEngine_DownLoad/XEngine_DownLoad.lib")
#endif
#endif