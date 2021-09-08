#pragma once

XHTHREAD CALLBACK XContral_RPCThread(LPVOID lParam);
BOOL XContral_RPC_Free(list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete);
BOOL XContral_RPC_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCSTR lpszClientAddr, LPCSTR lpszMsgBuffer, int nMsgLen, CHAR** pptszListHdr, int nHdrCount);