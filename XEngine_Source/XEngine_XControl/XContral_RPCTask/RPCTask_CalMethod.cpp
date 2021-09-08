#include "../XContral_Hdr.h"

void XRPCCore_Task_CalMethod(LPCSTR lpszClientAddr, LPCSTR lpszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE nRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete)
{
	int nRet = 0;
	int nMsgLen = 0;
	CHAR tszMsgBuffer[2048];

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	list<PROTOCOL_XRPCPARAMETE>::const_iterator stl_ListIterator = pStl_ListParamete->begin();
	for (; stl_ListIterator != pStl_ListParamete->end(); stl_ListIterator++)
	{
		int nValue = 0;
		memcpy(&nValue, stl_ListIterator->lParameteValue, stl_ListIterator->nParameteLen);

		nRet += nValue;
	}
	Protocol_Packet_FuncCall(lpszFuncName, (LPCSTR)&nRet, sizeof(int), nRtnType, tszMsgBuffer, &nMsgLen);
	XContral_Client_Send(lpszClientAddr, tszMsgBuffer, nMsgLen, XENGINE_CONTRALAPP_NETTYPE_XPRC);
}