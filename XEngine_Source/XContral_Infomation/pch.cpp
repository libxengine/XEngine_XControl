#include "pch.h"
#include "XContral_Info/XContral_Info.h"
/********************************************************************
//	Created:	2017/5/26   10:26
//	Filename: 	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage\BackManage_DllMain.cpp
//	File Path:	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage
//	File Base:	BackManage_DllMain
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	后台管理服务导出函数实现
//	History:
*********************************************************************/
BOOL BackManage_IsErrorOccur = FALSE;
DWORD BackManage_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CXContral_Info m_XInfo;
//////////////////////////////////////////////////////////////////////////
//                    导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD XInfo_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return BackManage_dwErrorCode;
}
/************************************************************************/
/*                    导出的系统信息获取函数                            */
/************************************************************************/
extern "C" BOOL XContral_Info_HardWare(CHAR * ptszHWInfo, int* pInt_Len)
{
	return m_XInfo.XContral_Info_HardWare(ptszHWInfo, pInt_Len);
}
extern "C" BOOL XContral_Info_SoftWare(CHAR * ptszSWInfo, int* pInt_Len)
{
	return m_XInfo.XContral_Info_SoftWare(ptszSWInfo, pInt_Len);
}