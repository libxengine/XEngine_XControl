#include "XContral_Hdr.h"

XHTHREAD XContral_Thread_TCPTask()
{
	TCHAR tszMsgBuffer[4096];

	while (bIsRun)
	{
		int nMsgLen = 4096;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (XClient_TCPSelect_RecvMsg(hTCPSocket, tszMsgBuffer, &nMsgLen))
		{
			XContral_Task_ProtocolParse(tszMsgBuffer, nMsgLen);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}
XHTHREAD XContral_Thread_UDPTask()
{
	TCHAR tszMsgBuffer[4096];

	while (bIsRun)
	{
		int nMsgLen = 4096;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (XClient_TCPSelect_RecvMsg(hUDPSocket, tszMsgBuffer, &nMsgLen))
		{
			XContral_Task_ProtocolParse(tszMsgBuffer, nMsgLen);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}