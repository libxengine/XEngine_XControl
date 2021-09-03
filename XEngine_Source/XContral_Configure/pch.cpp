#include "pch.h"
#include "Configure_Json/Configure_Json.h"
/********************************************************************
//    Created:     2021/08/24  13:14:12
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\pch.cpp
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Configure
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
BOOL Config_IsErrorOccur = FALSE;
DWORD Config_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CConfigure_Json m_ConfigJson;
//////////////////////////////////////////////////////////////////////////
//                    导出函数实现
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Config_dwErrorCode;
}
/************************************************************************/
/*                        配置读取导出                                  */
/************************************************************************/
extern "C" BOOL Config_Json_File(LPCSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_FileConfig)
{
	return m_ConfigJson.Config_Json_File(lpszConfigFile, pSt_FileConfig);
}
extern "C" BOOL Config_Json_LoadList(LPCSTR lpszConfigFile, XENGINE_CONFIGAPP * pSt_AppConfig)
{
	return m_ConfigJson.Config_Json_LoadList(lpszConfigFile, pSt_AppConfig);
}