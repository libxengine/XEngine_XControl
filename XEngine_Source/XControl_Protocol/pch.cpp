#include "pch.h"
#include "Protocol_Packet/Protocol_Packet.h"
#include "Protocol_Parse/Protocol_Parse.h"
/********************************************************************
//    Created:     2021/09/08  10:39:47
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\pch.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
BOOL Protocol_IsErrorOccur = FALSE;
DWORD Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_Parse m_ProtocolParse;
CProtocol_Packet m_ProtocolPacket;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Protocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Protocol_dwErrorCode;
}
/************************************************************************/
/*                        封装类函数                                    */
/************************************************************************/
extern "C" BOOL Protocol_Packet_RPCResponse(LPCSTR lpszFuncName, LPCSTR lpszValue, int nMsgLen, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enXRPC_ParamType, CHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_ProtocolPacket.Protocol_Packet_RPCResponse(lpszFuncName, lpszValue, nMsgLen, enXRPC_ParamType, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL Protocol_Packet_RPCRequest(LPCSTR lpszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enRtnType, list<PROTOCOL_XRPCPARAMETE>*pStl_ListParament, CHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_ProtocolPacket.Protocol_Packet_RPCRequest(lpszFuncName, enRtnType, pStl_ListParament, ptszMsgBuffer, pInt_MsgLen);
}
/************************************************************************/
/*                        解析类函数                                    */
/************************************************************************/
extern "C" BOOL Protocol_Parse_RPCRequest(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE * penRtnType, list<PROTOCOL_XRPCPARAMETE>*pStl_ListParamete)
{
	return m_ProtocolParse.Protocol_Parse_RPCRequest(lpszMsgBuffer, nMsgLen, ptszFuncName, penRtnType, pStl_ListParamete);
}
extern "C" BOOL Protocol_Parse_RPCReponse(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR * ptszFuncName, PROTOCOL_XRPCPARAMETE * pSt_RPCParament)
{
	return m_ProtocolParse.Protocol_Parse_RPCReponse(lpszMsgBuffer, nMsgLen, ptszFuncName, pSt_RPCParament);
}