#pragma once
/********************************************************************
//	Created:	2017/5/26   14:09
//	Filename: 	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage\BackManage_GetInfo\BackManage_GetInfo.h
//	File Path:	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage\BackManage_GetInfo
//	File Base:	BackManage_GetInfo
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	信息收集类
//	History:
*********************************************************************/

class CXContral_Info
{
public:
    CXContral_Info();
    ~CXContral_Info();
public:
    BOOL XContral_Info_HardWare(CHAR *ptszHWInfo, int *pInt_Len);
    BOOL XContral_Info_SoftWare(CHAR *ptszSWInfo, int *pInt_Len);
private:
};
