#pragma once

BOOL __stdcall XControl_Callback_RPCLogin(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XControl_Callback_RPCRecv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void __stdcall XControl_Callback_RPCLeave(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall XControl_Callback_RPCHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);

void XControl_Client_Close(LPCSTR lpszClientAddr, int nIPProto, BOOL bHeart);
BOOL XControl_Client_Send(LPCSTR lpszClientAddr, LPCSTR lpszMsgBuffer, int nMsgLen, int nIPProto);

XHTHREAD XControl_Thread_TCPTask();
XHTHREAD XControl_Thread_UDPTask();