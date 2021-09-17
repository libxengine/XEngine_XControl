#include "../XControl_Hdr.h"

void XRPCCore_Task_MethodMessage(LPCSTR lpszClientAddr, LPCSTR lpszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE nRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete)
{
	int nMsgLen = 2048;
	CHAR tszMsgBuffer[2048];

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	list<PROTOCOL_XRPCPARAMETE>::const_iterator stl_ListIterator = pStl_ListParamete->begin();
	for (; stl_ListIterator != pStl_ListParamete->end(); stl_ListIterator++)
	{
		if (ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_STRING == stl_ListIterator->enXRPC_ParameteType)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "RPC客户端:%s,打印函数-值为:%s", lpszClientAddr, stl_ListIterator->lParameteValue);
		}
	}
	Protocol_Packet_RPCResponse(lpszFuncName, NULL, 0, nRtnType, tszMsgBuffer, &nMsgLen);
	XControl_Client_Send(lpszClientAddr, tszMsgBuffer, nMsgLen, XENGINE_CONTRALAPP_NETTYPE_XPRC);
}