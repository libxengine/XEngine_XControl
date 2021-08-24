#pragma once

XHTHREAD XContral_Thread_HttpTask();
XHTHREAD XContral_Thread_TcpTask();
BOOL XContral_Task_ProtocolParse(LPCSTR lpszMsgBuffer, int nMsgLen);