#pragma once
/********************************************************************
//    Created:     2021/08/24  10:57:48
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Config_Define.h
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Configure
//    File Base:   Config_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置模块导出
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XEngine_ServerConfig
{
	CHAR tszTaskUrl[MAX_PATH];
	CHAR tszTmpFile[MAX_PATH];
	BOOL bAutoStart;
	BOOL bHideWnd;
	struct
	{
		int nCheckTime;
		int nErrorTime;
	}st_Time;
	struct
	{
		CHAR tszIPAddr[128];
		int nIPType;
		int nPort;
		BOOL bEnable;
	}st_Client;
	struct
	{
		CHAR tszLogFile[MAX_PATH];
		int nMaxSize;
		int nMaxCount;
		int nLogLeave;
	}st_XLog;
	struct
	{
		list<string>* pStl_ListVer;
	}st_Version;
}XENGINE_SERVERCONFIG;
//用户进程列表
typedef struct
{
	CHAR tszAPPName[MAX_PATH];         //应用程序名称
	CHAR tszAPPPath[MAX_PATH];         //应用程序路径
	BOOL bAutoStart;                    //是否自动启动
	BOOL bEnable;                       //是否启用
	BOOL bService;                      //是否为服务启动
	int nReTime;                        //是否自动重启
	//自定义
	int nErrorTime;
	__int64x nStartTime;
}XENGINE_APPINFO;
typedef struct
{
	list<XENGINE_APPINFO>* pStl_ListApp;
}XENGINE_CONFIGAPP;
//////////////////////////////////////////////////////////////////////////
//                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        配置读取导出                                  */
/************************************************************************/
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
extern "C" BOOL Config_Json_File(LPCSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_FileConfig);
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
extern "C" BOOL Config_Json_LoadList(LPCSTR lpszConfigFile, XENGINE_CONFIGAPP* pSt_AppConfig);