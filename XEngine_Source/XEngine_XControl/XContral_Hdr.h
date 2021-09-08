#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <json/json.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
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
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Error.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Define.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../XControl_Protocol.h"
#include "../XControl_Configure/Config_Define.h"
#include "../XControl_Configure/Config_Error.h"
#include "../XControl_Infomation/XInfo_Define.h"
#include "../XControl_Infomation/XInfo_Error.h"
#include "../XControl_Protocol/Protocol_Define.h"
#include "../XControl_Protocol/Protocol_Error.h"

#define XENGINE_CONTRALAPP_NETTYPE_XPRC 1
//” º˛≈‰÷√
typedef struct
{
	RFCCOMPONENTS_EMAILSMTP st_EMailSmtp;
	BOOL bEnable;
	BOOL bCreateEmail;
	list<string>* pStl_ListAddr;
}XENGIEN_EMAILCONFIG;

extern BOOL bIsRun;
extern BOOL bExist;
extern XLOG xhLog;
extern SOCKET hTCPSocket;
extern SOCKET hUDPSocket;
extern int m_nTaskSerial;
extern XHANDLE xhRPCPacket;
extern XNETHANDLE xhRPCPool;
extern XNETHANDLE xhRPCSocket;
extern XNETHANDLE xhRPCHeart;

extern shared_ptr<std::thread> pSTDThread_Http;
extern shared_ptr<std::thread> pSTDThread_TCP;
extern shared_ptr<std::thread> pSTDThread_UDP;
extern shared_ptr<std::thread> pSTDThread_App;
extern XENGINE_SERVERCONFIG st_ServiceConfig;
extern XENGINE_CONFIGAPP st_APPConfig;
extern XENGIEN_EMAILCONFIG st_EMailConfig;

#include "XContral_Config.h"
#include "XContral_Handle.h"
#include "XContral_Task.h"
#include "XContral_Network.h"
#include "XContral_Process.h"
#include "XContral_RPCTask.h"

#include "XContral_RPCTask/RPCTask_CalMethod.h"

#ifdef _WINDOWS
#pragma comment(lib,"Ws2_32.lib")
#ifdef _WIN64
#pragma comment(lib,"../x64/Release/XControl_Configure.lib")
#pragma comment(lib,"../x64/Release/XControl_Infomation.lib")
#pragma comment(lib,"../x64/Release/XControl_Protocol.lib")
#pragma comment(lib,"x64/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x64/XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"x64/XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"x64/XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"x64/XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"x64/XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"x64/XEngine_RfcComponents/RfcComponents_EmailClient.lib")
#pragma comment(lib,"x64/XEngine_RfcComponents/RfcComponents_HttpServer.lib")
#pragma comment(lib,"x64/XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"x64/XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"x64/XEngine_DownLoad/XEngine_DownLoad.lib")
#else
#pragma comment(lib,"../Debug/XControl_Configure.lib")
#pragma comment(lib,"../Debug/XControl_Infomation.lib")
#pragma comment(lib,"../Debug/XControl_Protocol.lib")
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"x86/XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"x86/XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"x86/XEngine_RfcComponents/RfcComponents_EmailClient.lib")
#pragma comment(lib,"x86/XEngine_RfcComponents/RfcComponents_HttpServer.lib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"x86/XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"x86/XEngine_DownLoad/XEngine_DownLoad.lib")
#endif
#endif