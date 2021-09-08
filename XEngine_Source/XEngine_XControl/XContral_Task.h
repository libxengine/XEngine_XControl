#pragma once

XHTHREAD XContral_Thread_HttpTask();
XHTHREAD XContral_Thread_TCPTask();
XHTHREAD XContral_Thread_UDPTask();

BOOL XContral_Task_ProtocolParse(LPCSTR lpszMsgBuffer, int nMsgLen);