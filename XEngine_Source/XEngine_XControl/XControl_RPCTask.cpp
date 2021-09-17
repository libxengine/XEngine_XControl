#include "XControl_Hdr.h"

XHTHREAD CALLBACK XControl_RPCThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;
	while (bIsRun)
	{
		if (!RfcComponents_HttpServer_EventWaitEx(xhRPCPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSst_ListAddr;

		RfcComponents_HttpServer_GetPoolEx(xhRPCPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;
				int nHdrCount = 0;
				CHAR** ppszHdrList;
				CHAR* ptszMsgBuffer = NULL;
				RFCCOMPONENTS_HTTP_REQPARAM st_HTTPReqparam;

				memset(&st_HTTPReqparam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

				if (!RfcComponents_HttpServer_GetMemoryEx(xhRPCPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPReqparam, &ppszHdrList, &nHdrCount))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP服务器获取消息失败，获取数据包失败，错误：%lX", HttpServer_GetLastError());
					continue;
				}
				XControl_RPC_Handle(&st_HTTPReqparam, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, ppszHdrList, nHdrCount);
				BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
BOOL XControl_RPC_Free(list<PROTOCOL_XRPCPARAMETE> *pStl_ListParamete)
{
	auto stl_ListIterator = pStl_ListParamete->begin();
	for (; stl_ListIterator != pStl_ListParamete->end(); ++stl_ListIterator)
	{
		free(stl_ListIterator->lParameteValue);
		stl_ListIterator->lParameteValue = NULL;
	}
	pStl_ListParamete->clear();
	return TRUE;
}

BOOL XControl_RPC_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCSTR lpszClientAddr, LPCSTR lpszMsgBuffer, int nMsgLen, CHAR** pptszListHdr, int nHdrCount)
{
	CHAR tszFuncName[128];
	list<PROTOCOL_XRPCPARAMETE> stl_ListParamete;
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enXRPC_ReturnType;

	memset(tszFuncName, '\0', sizeof(tszFuncName));
	if (!Protocol_Parse_RPCRequest(lpszMsgBuffer, nMsgLen, tszFuncName, &enXRPC_ReturnType, &stl_ListParamete))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "RPC客户端:%s,处理客户端请求失败,错误：%lX", lpszClientAddr, Protocol_GetLastError());
		return FALSE;
	}
	if (0 == strncmp(tszFuncName, "XEngine_MethodCal", strlen(tszFuncName)))
	{
		XRPCCore_Task_MethodCal(lpszClientAddr, tszFuncName, enXRPC_ReturnType, &stl_ListParamete);
	}
	if (0 == strncmp(tszFuncName, "XEngine_MethodMessage", strlen(tszFuncName)))
	{
		XRPCCore_Task_MethodMessage(lpszClientAddr, tszFuncName, enXRPC_ReturnType, &stl_ListParamete);
	}
	XControl_RPC_Free(&stl_ListParamete);
	return TRUE;
}