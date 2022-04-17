#pragma once

XHTHREAD CALLBACK XControl_RPCThread(LPVOID lParam);
BOOL XControl_RPC_Free(list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete);
BOOL XControl_RPC_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCSTR lpszClientAddr, LPCSTR lpszMsgBuffer, int nMsgLen, CHAR** pptszListHdr, int nHdrCount);