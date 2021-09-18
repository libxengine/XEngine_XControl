﻿#include "XControl_Hdr.h"

BOOL __stdcall XControl_Callback_RPCLogin(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	SocketOpt_HeartBeat_InsertAddrEx(xhRPCHeart, lpszClientAddr);
	RfcComponents_HttpServer_CreateClientEx(xhRPCPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "RPC客户端连接，RPC客户端地址：%s", lpszClientAddr);
	return TRUE;
}
void __stdcall XControl_Callback_RPCRecv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhRPCPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "投递RPC数据包到消息队列失败，错误：%lX", HttpServer_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhRPCHeart, lpszClientAddr);
}
void __stdcall XControl_Callback_RPCLeave(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XControl_Client_Close(lpszClientAddr, XENGINE_CONTRALAPP_NETTYPE_XPRC, FALSE);
}
void __stdcall XControl_Callback_RPCHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XControl_Client_Close(lpszClientAddr, XENGINE_CONTRALAPP_NETTYPE_XPRC, TRUE);
}
//////////////////////////////////////////////////////////////////////////
void XControl_Client_Close(LPCSTR lpszClientAddr, int nIPProto, BOOL bHeart)
{
	if (XENGINE_CONTRALAPP_NETTYPE_XPRC == nIPProto)
	{
		RfcComponents_HttpServer_CloseClinetEx(xhRPCPacket, lpszClientAddr);

		if (bHeart)
		{
			NetCore_TCPXCore_CloseForClientEx(xhRPCSocket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhRPCHeart, lpszClientAddr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "RPC客户端离开，RPC客户端地址：%s", lpszClientAddr);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "未知客户端客户端离开，客户端地址：%s", lpszClientAddr);
	}
}
//////////////////////////////////////////////////////////////////////////
BOOL XControl_Client_Send(LPCSTR lpszClientAddr, LPCSTR lpszMsgBuffer, int nMsgLen, int nIPProto)
{
	if (XENGINE_CONTRALAPP_NETTYPE_XPRC == nIPProto)
	{
		int nPKTLen = 8196;
		CHAR tszPKTBuffer[8196];
		RFCCOMPONENTS_HTTP_HDRPARAM st_HTTPHdr;

		memset(tszPKTBuffer, '\0', sizeof(tszPKTBuffer));
		memset(&st_HTTPHdr, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

		st_HTTPHdr.nHttpCode = 200;
		st_HTTPHdr.bIsClose = TRUE;
		strcpy(st_HTTPHdr.tszMimeType, "json");

		RfcComponents_HttpServer_SendMsgEx(xhRPCPacket, tszPKTBuffer, &nPKTLen, &st_HTTPHdr, lpszMsgBuffer, nMsgLen);
		if (!NetCore_TCPXCore_SendEx(xhRPCSocket, lpszClientAddr, tszPKTBuffer, nPKTLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "发送数据给RPC客户端：%s，失败，错误：%lX", lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "未知客户端客户端:%s,请求发送数据", lpszClientAddr);
	}
	return TRUE;
}