#pragma once
/********************************************************************
//	Created:	2017/5/26   10:25
//	Filename: 	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage\BackManage_Define.h
//	File Path:	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage
//	File Base:	BackManage_Define
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	后台管理服务导出函数
//	History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                    导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD XInfo_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                    导出的系统信息获取函数                            */
/************************************************************************/
/********************************************************************
函数名称：XContral_Info_HardWare
函数功能：获取硬件信息
 参数.一：ptszHWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的数据,这个数据是JSON格式
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出数据的长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL XContral_Info_HardWare(TCHAR* ptszHWInfo, int* pInt_Len);
/********************************************************************
函数名称：XContral_Info_SoftWare
函数功能：获取软件系统信息
 参数.一：ptszSWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出系统信息JSON结构
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出系统信息长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL XContral_Info_SoftWare(TCHAR* ptszSWInfo, int* pInt_Len);
