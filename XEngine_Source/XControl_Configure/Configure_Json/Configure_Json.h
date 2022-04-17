#pragma once
/********************************************************************
//    Created:     2021/08/24  11:17:07
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Configure_Json\Configure_Json.h
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Configure_Json
//    File Base:   Configure_Json
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置文件读取类
//    History:
*********************************************************************/

class CConfigure_Json
{
public:
	CConfigure_Json();
	~CConfigure_Json();
public:
	BOOL Config_Json_File(LPCSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_FileConfig);
	BOOL Config_Json_LoadList(LPCSTR lpszConfigFile, XENGINE_CONFIGAPP* pSt_AppConfig);
};