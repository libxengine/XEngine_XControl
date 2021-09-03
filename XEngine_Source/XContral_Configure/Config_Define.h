#pragma once
/********************************************************************
//    Created:     2021/08/24  10:57:48
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Configure\Config_Define.h
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Configure
//    File Base:   Config_Define
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     ����ģ�鵼��
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        ���������ݽṹ
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XEngine_ServerConfig
{
	CHAR tszTaskUrl[MAX_PATH];
	CHAR tszTmpFile[MAX_PATH];
	CHAR tszAPPDeamon[MAX_PATH];
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
//�û������б�
typedef struct
{
	CHAR tszAPPName[MAX_PATH];         //Ӧ�ó�������
	CHAR tszAPPPath[MAX_PATH];         //Ӧ�ó���·��
	BOOL bShow;                        //��ʾ�������� 
	BOOL bAutoStart;                   //�Ƿ��Զ�����
	BOOL bEnable;                      //�Ƿ�����
	BOOL bService;                     //�Ƿ�Ϊ��������
	int nReTime;                       //�Ƿ��Զ�����
	//�Զ���
	int nErrorTime;
	__int64x nStartTime;
}XENGINE_APPINFO;
typedef struct
{
	list<XENGINE_APPINFO>* pStl_ListApp;
}XENGINE_CONFIGAPP;
//////////////////////////////////////////////////////////////////////////
//                        �����ĺ���
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        ���ö�ȡ����                                  */
/************************************************************************/
/********************************************************************
�������ƣ�Config_Json_File
�������ܣ�JSON�������ö�ȡ
 ����.һ��lpszConfigFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�������ļ���ַ
 ����.����pSt_FileConfig
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼�������ȡ����������Ϣ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
extern "C" BOOL Config_Json_File(LPCSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_FileConfig);
/********************************************************************
�������ƣ�Config_Json_LoadList
�������ܣ�JSON APP�б����ö�ȡ
 ����.һ��lpszConfigFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�������ļ���ַ
 ����.����pSt_FileConfig
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼�������ȡ����������Ϣ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
extern "C" BOOL Config_Json_LoadList(LPCSTR lpszConfigFile, XENGINE_CONFIGAPP* pSt_AppConfig);