#include "pch.h"
#include "Configure_Json.h"
/********************************************************************
//    Created:     2021/08/24  11:17:18
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Configure_Json\Configure_Json.cpp
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Configure_Json
//    File Base:   Configure_Json
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置文件读取类
//    History:
*********************************************************************/
CConfigure_Json::CConfigure_Json()
{

}
CConfigure_Json::~CConfigure_Json()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Config_Json_File
函数功能：JSON基础配置读取
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.二：pSt_FileConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CConfigure_Json::Config_Json_File(LPCSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_FileConfig)
{
	Config_IsErrorOccur = FALSE;

	if ((NULL == lpszConfigFile) || (NULL == pSt_FileConfig))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_PARAMENT;
		return FALSE;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;

	FILE* pSt_File = fopen(lpszConfigFile, "rb");
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_OPENFILE;
		return FALSE;
	}
	int nCount = 0;
	CHAR tszMsgBuffer[4096];
	while (1)
	{
		int nRet = fread(tszMsgBuffer + nCount, 1, 2048, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nCount, &st_JsonRoot, &st_JsonError))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_PARSE;
		return FALSE;
	}

	strcpy(pSt_FileConfig->tszTaskUrl, st_JsonRoot["tszTaskUrl"].asCString());
	strcpy(pSt_FileConfig->tszTmpFile, st_JsonRoot["tszTmpFile"].asCString());
	pSt_FileConfig->nPort = st_JsonRoot["nPort"].asInt();
	pSt_FileConfig->bAutoStart = st_JsonRoot["bAutoStart"].asInt();
	pSt_FileConfig->bHideWnd = st_JsonRoot["bHideWnd"].asInt();

	if (st_JsonRoot["ClientTime"].empty() || (3 != st_JsonRoot["ClientTime"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_TIME;
		return FALSE;
	}
	Json::Value st_JsonTime = st_JsonRoot["ClientTime"];
	pSt_FileConfig->st_Time.nTaskTime = st_JsonTime["nTaskTime"].asInt();
	pSt_FileConfig->st_Time.nCheckTime = st_JsonTime["nCheckTime"].asInt();
	pSt_FileConfig->st_Time.nErrorTime = st_JsonTime["nErrorTime"].asInt();

	if (st_JsonRoot["ClientConfig"].empty() || (4 != st_JsonRoot["ClientConfig"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_CLIENT;
		return FALSE;
	}
	Json::Value st_JsonClient = st_JsonRoot["ClientConfig"];
	pSt_FileConfig->st_Client.bEnable = st_JsonClient["bEnable"].asInt();
	pSt_FileConfig->st_Client.nIPType = st_JsonClient["nIPType"].asInt();
	pSt_FileConfig->st_Client.nPort = st_JsonClient["nPort"].asInt();
	strcpy(pSt_FileConfig->st_Client.tszIPAddr, st_JsonClient["tszIPAddr"].asCString());

	if (st_JsonRoot["LogConfig"].empty() || (4 != st_JsonRoot["LogConfig"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XLOG;
		return FALSE;
	}
	Json::Value st_JsonXLog = st_JsonRoot["LogConfig"];
	pSt_FileConfig->st_XLog.nMaxSize = st_JsonXLog["nMaxSize"].asInt();
	pSt_FileConfig->st_XLog.nMaxCount = st_JsonXLog["nMaxCount"].asInt();
	pSt_FileConfig->st_XLog.nLogLeave = st_JsonXLog["nLogLeave"].asInt();
	strcpy(pSt_FileConfig->st_XLog.tszLogFile, st_JsonXLog["tszLogFile"].asCString());

	if (st_JsonRoot["VersionList"].empty() || (1 != st_JsonRoot["VersionList"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_VERSION;
		return FALSE;
	}
	Json::Value st_JsonVersion = st_JsonRoot["VersionList"];

	pSt_FileConfig->st_Version.pStl_ListVer = new list<string>;
	for (unsigned int i = 0; i < st_JsonVersion.size(); i++)
	{
		pSt_FileConfig->st_Version.pStl_ListVer->push_back(st_JsonVersion[i].asCString());
	}
	return TRUE;
}
/********************************************************************
函数名称：Config_Json_LoadList
函数功能：JSON APP列表配置读取
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.二：pSt_FileConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CConfigure_Json::Config_Json_LoadList(LPCSTR lpszConfigFile, XENGINE_CONFIGAPP* pSt_AppConfig)
{
	Config_IsErrorOccur = FALSE;

	if ((NULL == lpszConfigFile) || (NULL == pSt_AppConfig))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_PARAMENT;
		return FALSE;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;

	FILE* pSt_File = fopen(lpszConfigFile, "rb");
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_OPENFILE;
		return FALSE;
	}
	int nCount = 0;
	CHAR tszMsgBuffer[4096];
	while (1)
	{
		int nRet = fread(tszMsgBuffer + nCount, 1, 2048, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nCount, &st_JsonRoot, &st_JsonError))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XCONTRAL_MODULE_CONFIG_JSON_PARSE;
		return FALSE;
	}
	int nListCount = st_JsonRoot["ListCount"].asInt();
	Json::Value st_JsonArray = st_JsonRoot["ListArray"];

	pSt_AppConfig->pStl_ListApp = new list<XENGINE_APPINFO>;
	for (int i = 0; i < nListCount; i++)
	{
		XENGINE_APPINFO st_APPInfo;
		memset(&st_APPInfo, '\0', sizeof(XENGINE_APPINFO));

		strcpy(st_APPInfo.tszAPPName, st_JsonArray[i]["APPName"].asCString());
		strcpy(st_APPInfo.tszAPPPath, st_JsonArray[i]["APPPath"].asCString());
		st_APPInfo.bAutoStart = st_JsonArray[i]["APPAutoStart"].asInt();
		st_APPInfo.bService = st_JsonArray[i]["APPService"].asInt();
		st_APPInfo.nReTime = st_JsonArray[i]["APPReTime"].asInt();
		st_APPInfo.bEnable = st_JsonArray[i]["APPEnable"].asInt();

		pSt_AppConfig->pStl_ListApp->push_back(st_APPInfo);
	}
	return TRUE;
}