#pragma once

BOOL __stdcall XContral_Callback_RPCLogin(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XContral_Callback_RPCRecv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void __stdcall XContral_Callback_RPCLeave(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XContral_Callback_RPCHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);

void XContral_Client_Close(LPCSTR lpszClientAddr, int nIPProto, BOOL bHeart);
BOOL XContral_Client_Send(LPCSTR lpszClientAddr, LPCSTR lpszMsgBuffer, int nMsgLen, int nIPProto);

XHTHREAD XContral_Thread_TCPTask();
XHTHREAD XContral_Thread_UDPTask();