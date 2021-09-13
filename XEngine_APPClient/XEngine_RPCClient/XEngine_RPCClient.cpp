#pragma once
#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include "../../XEngine_Source/XControl_Protocol.h"
#include "../../XEngine_Source/XControl_Protocol/Protocol_Define.h"
#include "../../XEngine_Source/XControl_Protocol/Protocol_Error.h"

//g++ -std=c++17 -Wall -g NetHelp_APPRPCClient.cpp -o NetHelp_APPRPCClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lHelpComponents_Packets -lNetHelp_XRpc -ljsoncpp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags

#ifdef _WINDOWS
#pragma comment(lib,"Ws2_32.lib")
#ifdef _DEBUG
#pragma comment(lib,"../../XEngine_Source/Debug/XControl_Protocol.lib")
#else
#pragma comment(lib,"../../XEngine_Source/Release/XControl_Protocol.lib")
#endif
#pragma comment(lib,"x86/XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp.lib")
#endif

LPCSTR lpszServerAddr = "http://192.168.1.7:5002";
int XEngine_AddMethod(int a, int b)
{
	int nRet = 0;
    int nMsgLen = 2048;
    CHAR tszMsgBuffer[2048];
    list<PROTOCOL_XRPCPARAMETE> stl_ListParament;
	
    memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	PROTOCOL_XRPCPARAMETE st_XRPCParameteA;
	PROTOCOL_XRPCPARAMETE st_XRPCParameteB;

	memset(&st_XRPCParameteA, '\0', sizeof(PROTOCOL_XRPCPARAMETE));
	memset(&st_XRPCParameteB, '\0', sizeof(PROTOCOL_XRPCPARAMETE));

	st_XRPCParameteA.lParameteValue = malloc(sizeof(int));
	st_XRPCParameteA.enXRPC_ParameteType = ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT;
	st_XRPCParameteA.nParameteLen = sizeof(int);
	memcpy(st_XRPCParameteA.lParameteValue, &a, sizeof(int));

	stl_ListParament.push_back(st_XRPCParameteA);

	st_XRPCParameteB.lParameteValue = malloc(sizeof(int));
	st_XRPCParameteB.enXRPC_ParameteType = ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT;
	st_XRPCParameteB.nParameteLen = sizeof(int);
	memcpy(st_XRPCParameteB.lParameteValue, &b, sizeof(int));

	stl_ListParament.push_back(st_XRPCParameteB);

    Protocol_Packet_RPCRequest("XEngine_AddMethod", ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT, &stl_ListParament, tszMsgBuffer, &nMsgLen);

	int nBLen = 0;
	CHAR* ptszMsgBody;
	APIHelp_HttpRequest_Post(lpszServerAddr, tszMsgBuffer, NULL, &ptszMsgBody, &nBLen);
	
	for (auto stl_ListIterator = stl_ListParament.begin(); stl_ListIterator != stl_ListParament.end(); ++stl_ListIterator)
	{
		free(stl_ListIterator->lParameteValue);
		stl_ListIterator->lParameteValue = NULL;
	}
	stl_ListParament.clear();

	printf("%s\n", ptszMsgBody);
	return nRet;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XEngine_AddMethod(1, 2);

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}