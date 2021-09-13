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
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Protocol_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        封装类函数                                    */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Packet_RPCResponse
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
extern "C" BOOL Protocol_Packet_RPCResponse(LPCSTR lpszFuncName, LPCSTR lpszValue, int nMsgLen, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enXRPC_ParamType, CHAR* ptszMsgBuffer, int* pInt_MsgLen);
/********************************************************************
函数名称：Protocol_Packet_RPCRequest
函数功能：打包远程调用函数协议
 参数.一：lpszFuncName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入函数名称
 参数.二：enRtnType
  In/Out：In
  类型：枚举型
  可空：N
  意思：输入返回值类型
 参数.三：pppSt_ListParamete
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入参数
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入参数个数
 参数.五：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据
 参数.六：pInt_MsgLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入:提供缓冲区大小 输出:打包的数据大小
返回值
  类型：逻辑型
  意思：是否成功
备注：pppSt_ListParamete内存由用户申请和释放
*********************************************************************/
extern "C" BOOL Protocol_Packet_RPCRequest(LPCSTR lpszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParament, CHAR* ptszMsgBuffer, int* pInt_MsgLen);
/************************************************************************/
/*                        解析类函数                                    */
/************************************************************************/
/********************************************************************
函数名称：Protocol_Parse_RPCRequest
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
extern "C" BOOL Protocol_Parse_RPCRequest(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE* penRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete);
/********************************************************************
函数名称：Protocol_Parse_RPCReponse
函数功能：客户端返回数据解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：ptszFuncName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出服务器返回的函数名称
 参数.四：pSt_RPCParament
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出返回的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Protocol_Parse_RPCReponse(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, PROTOCOL_XRPCPARAMETE* pSt_RPCParament);