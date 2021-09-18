#pragma once

XHTHREAD XControl_Thread_HttpTask();
XHTHREAD XControl_Thread_TCPTask();
XHTHREAD XControl_Thread_UDPTask();

BOOL XControl_Task_ProtocolParse(LPCSTR lpszMsgBuffer, int nMsgLen);