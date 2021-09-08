#include "pch.h"
#include "Protocol_Packet.h"
/********************************************************************
//    Created:     2021/09/08  10:35:32
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Packet\Protocol_Packet.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Packet
//    File Base:   Protocol_Packet
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块打包类
//    History:
*********************************************************************/
CProtocol_Packet::CProtocol_Packet()
{
}
CProtocol_Packet::~CProtocol_Packet()
{
}
///////////////////////////////////////////////////////////////////////////////
//                      公有函数
///////////////////////////////////////////////////////////////////////////////
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
BOOL CProtocol_Packet::Protocol_Packet_FuncCall(LPCSTR lpszFuncName, LPCSTR lpszValue, int nMsgLen, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enXRPC_ParamType, CHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszFuncName) || (NULL == ptszMsgBuffer))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    st_JsonRoot["XRpc_FunctionName"] = lpszFuncName;
    st_JsonRoot["XRpc_ReturnType"] = enXRPC_ParamType;

    switch (enXRPC_ParamType)
    {
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_VOID:
    {
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BOOL:
    {
        BOOL bValue = FALSE;
        memcpy(&bValue, lpszValue, nMsgLen);
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = bValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT:
    {
        int nValue = 0;
        memcpy(&nValue, lpszValue, nMsgLen);
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = nValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_UINT:
    {
        DWORD dwValue = 0;
        memcpy(&dwValue, lpszValue, nMsgLen);
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = (Json::Value::UInt)dwValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_LONGINT:
    {
        __int64x nllValue = 0;
        memcpy(&nllValue, lpszValue, nMsgLen);
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = (Json::Value::Int64)nllValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_ULONG:
    {
        ULONGLONG ullValue = 0;
        memcpy(&ullValue, lpszValue, nMsgLen);
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = (Json::Value::UInt64)ullValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_FLOAT:
    {
        float fValue = 0;
        memcpy(&fValue, lpszValue, nMsgLen);
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = fValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_DOUBLE:
    {
        double dlValue = 0;
        memcpy(&dlValue, lpszValue, nMsgLen);
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = dlValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_STRING:
    {
        st_JsonRoot["XRpc_ReturnLen"] = nMsgLen;
        st_JsonRoot["XRpc_ReturnValue"] = lpszValue;
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BIN:
    {
        //二进制数据,我们需要编码成BASE64才能处理
        LPCSTR lpszBinHdr = "XEngine";              //部分特别的数据可能会失败,我们需要添加一个编码头
        int nBaseLen = nMsgLen * 2;
        CHAR* ptszBaseBuffer = (CHAR*)malloc(nBaseLen);
        CHAR* ptszTmpBuffer = (CHAR*)malloc(nBaseLen);
        if ((NULL == ptszBaseBuffer) || (NULL == ptszTmpBuffer))
        {
            Protocol_IsErrorOccur = TRUE;
            Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
            return FALSE;
        }
        memset(ptszBaseBuffer, '\0', nBaseLen);
        memset(ptszTmpBuffer, '\0', nBaseLen);
        //打包编码数据
        memcpy(ptszTmpBuffer, lpszBinHdr, strlen(lpszBinHdr));
        memcpy(ptszTmpBuffer + strlen(lpszBinHdr), lpszValue, nMsgLen);
        //进行编码
        nBaseLen = strlen(lpszBinHdr) + nMsgLen;  //需要真实编码数据大小
        if (!OPenSsl_Codec_Base64(ptszTmpBuffer, ptszBaseBuffer, &nBaseLen))
        {
            Protocol_IsErrorOccur = TRUE;
            Protocol_dwErrorCode = OPenSsl_GetLastError();
            return FALSE;
        }
        st_JsonRoot["XRpc_ReturnLen"] = nBaseLen;
        st_JsonRoot["XRpc_ReturnValue"] = ptszBaseBuffer;
        //释放内存
        free(ptszBaseBuffer);
        free(ptszTmpBuffer);
        ptszTmpBuffer = NULL;
        ptszBaseBuffer = NULL;
    }
    break;
    default:
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_NOTSUPPORT;
        return FALSE;
    }
    //判断大小是否合适
    if ((int)st_JsonRoot.toStyledString().length() > *pInt_MsgLen)
    {
        *pInt_MsgLen = st_JsonRoot.toStyledString().length();
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_SIZE;
        return FALSE;
    }
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
    return TRUE;
}