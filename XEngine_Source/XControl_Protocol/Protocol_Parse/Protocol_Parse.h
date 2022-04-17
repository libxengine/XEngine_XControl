#pragma once
/********************************************************************
//    Created:     2021/09/08  10:35:07
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse\Protocol_Parse.h
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse
//    File Base:   Protocol_Parse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块解析类
//    History:
*********************************************************************/

class CProtocol_Parse
{
public:
    CProtocol_Parse();
    ~CProtocol_Parse();
public:
    BOOL Protocol_Parse_RPCRequest(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE* penRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete);
    BOOL Protocol_Parse_RPCReponse(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, PROTOCOL_XRPCPARAMETE* pSt_RPCParament);
protected:
private:
};
