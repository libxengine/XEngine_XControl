#pragma once
/********************************************************************
//    Created:     2021/09/07  14:46:23
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Protocol\Protocol_Define.h
//    File Path:   D:\XEngine_XContral\XEngine_Source\XContral_Protocol
//    File Base:   Protocol_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的枚举型
//////////////////////////////////////////////////////////////////////////
//类型
typedef enum 
{
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_UNKNOW = 0,                           //未知类型,请求端可以为0,不指定
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_VOID = 1,                             //无类型,使用此类型作为返回值,服务器将不返回任何东西
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BOOL = 2,                             //逻辑型
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT = 3,                              //整数型
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_UINT = 4,                             //无符号整数型
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_LONGINT = 5,                          //长整数型
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_ULONG = 6,                            //无符号长整数型
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_FLOAT = 7,                            //浮点型
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_DOUBLE = 8,                           //双精度浮点型
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_STRING = 9,                           //字符串
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BIN = 10                              //二进制数据
}ENUM_PROTOCOL_XRPC_PARAMETE_TYPE, * LPENUM_PROTOCOL_XRPC_PARAMETE_TYPE;
//////////////////////////////////////////////////////////////////////////
//                        导出的数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct 
{
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enXRPC_ParameteType;                  //参数类型
	int nParameteLen;                                                     //参数大小,数值相关类型这个值可以为0
	LPVOID lParameteValue;                                                //自定义参数值
}PROTOCOL_XRPCPARAMETE, * LPPROTOCOL_XRPCPARAMETE;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Protocol_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        封装类函数                                    */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Packet_FuncCall
函数功能：打包返回客户端数据库的协议函数
 参数.一：lpszFuncName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要返回的函数名
 参数.二：lpszValue
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入返回内容
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入返回内容大小
 参数.四：enXRPC_ParamType
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要返回的类型
 参数.五：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：打好包的数据
 参数.六：pInt_MsgLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入提供的缓冲区大小,输出获取到的缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Packet_FuncCall(LPCSTR lpszFuncName, LPCSTR lpszValue, int nMsgLen, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enXRPC_ParamType, CHAR* ptszMsgBuffer, int* pInt_MsgLen);
/************************************************************************/
/*                        解析类函数                                    */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Parse_FuncCall
函数功能：服务器解析函数调用
 参数.一：lpszFuncName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区,这个缓冲区不包含协议头
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析缓冲区大小
 参数.三：ptszFuncName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出这个请求的函数名称
 参数.四：penRtnType
  In/Out：Out
  类型：枚举型指针
  可空：N
  意思：输出返回值类型
 参数.五：pppSt_ListParamete
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析出来的参数
 参数.六：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出参数个数
返回值
  类型：逻辑型
  意思：是否成功
备注：必须使用帮助函数的内存释放函数进行释放内存
*********************************************************************/
extern "C" BOOL Protocol_Parse_FuncCall(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE* penRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete);