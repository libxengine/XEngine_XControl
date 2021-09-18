#include "pch.h"
#include "Protocol_Parse.h"
/********************************************************************
//    Created:     2021/09/08  10:35:21
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse\Protocol_Parse.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Protocol\Protocol_Parse
//    File Base:   Protocol_Parse
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块解析类
//    History:
*********************************************************************/
CProtocol_Parse::CProtocol_Parse()
{
}
CProtocol_Parse::~CProtocol_Parse()
{
}
///////////////////////////////////////////////////////////////////////////////
//                      公有函数
///////////////////////////////////////////////////////////////////////////////
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
BOOL CProtocol_Parse::Protocol_Parse_RPCRequest(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE* penRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszFuncName) || (NULL == penRtnType) || (NULL == pStl_ListParamete))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析数据
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;
    //解析函数名称
    strcpy(ptszFuncName, st_JsonRoot["XRpc_FunctionName"].asCString());
    //解析返回值
    *penRtnType = (ENUM_PROTOCOL_XRPC_PARAMETE_TYPE)st_JsonRoot["XRpc_ReturnType"].asUInt();
    //解析参数
    int nCount = st_JsonRoot["XRpc_ParamCount"].asInt();   //先获得参数个数
    Json::Value st_JsonArray = st_JsonRoot["XRpc_ParameteArray"];
    //轮训获取所有参数
    for (int i = 0; i < nCount; i++)
    {
        PROTOCOL_XRPCPARAMETE st_XRpcParamete;
        memset(&st_XRpcParamete, '\0', sizeof(PROTOCOL_XRPCPARAMETE));

        st_XRpcParamete.enXRPC_ParameteType = (ENUM_PROTOCOL_XRPC_PARAMETE_TYPE)st_JsonArray[i]["ParameteType"].asUInt();
        st_XRpcParamete.nParameteLen = st_JsonArray[i]["ParameteLen"].asInt();
        //判断参数类型
        switch (st_XRpcParamete.enXRPC_ParameteType)
        {
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BOOL:
        {
            BOOL bValue = st_JsonArray[i]["ParameteValue"].asInt();
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(st_XRpcParamete.nParameteLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', st_XRpcParamete.nParameteLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, &bValue, st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT:
        {
            int nValue = st_JsonArray[i]["ParameteValue"].asInt();
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(st_XRpcParamete.nParameteLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', st_XRpcParamete.nParameteLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, &nValue, st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_UINT:
        {
            DWORD dwValue = st_JsonArray[i]["ParameteValue"].asUInt();
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(st_XRpcParamete.nParameteLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', st_XRpcParamete.nParameteLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, &dwValue, st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_LONGINT:
        {
            __int64x nllValue = st_JsonArray[i]["ParameteValue"].asInt64();
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(st_XRpcParamete.nParameteLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', st_XRpcParamete.nParameteLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, &nllValue, st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_ULONG:
        {
            ULONGLONG ullValue = st_JsonArray[i]["ParameteValue"].asUInt64();
            memcpy(st_XRpcParamete.lParameteValue, &ullValue, st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_FLOAT:
        {
            float fValue = st_JsonArray[i]["ParameteValue"].asFloat();
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(st_XRpcParamete.nParameteLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', st_XRpcParamete.nParameteLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, &fValue, st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_DOUBLE:
        {
            double dlValue = st_JsonArray[i]["ParameteValue"].asDouble();
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(st_XRpcParamete.nParameteLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', st_XRpcParamete.nParameteLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, &dlValue, st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_STRING:
        {
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(st_XRpcParamete.nParameteLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', st_XRpcParamete.nParameteLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, st_JsonArray[i]["ParameteValue"].asCString(), st_XRpcParamete.nParameteLen);
        }
        break;
        case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BIN:
        {
            //二进制数据,我们需要解码后使用
            LPCSTR lpszBinHdr = "XEngine";
            int nHdrLen = strlen(lpszBinHdr);
            int nBaseLen = st_XRpcParamete.nParameteLen * 2;
            CHAR* ptszBaseBuffer = (CHAR*)malloc(nBaseLen);
            if (NULL == ptszBaseBuffer)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(ptszBaseBuffer, '\0', nBaseLen);
            //进行解码
            nBaseLen = st_XRpcParamete.nParameteLen;
            if (!OPenSsl_Codec_Base64(st_JsonArray[i]["ParameteValue"].asCString(), ptszBaseBuffer, &nBaseLen, FALSE))
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = OPenSsl_GetLastError();
                return FALSE;
            }
            //为参数申请足够的内存大小
            st_XRpcParamete.lParameteValue = malloc(nBaseLen + 1);
            if (NULL == st_XRpcParamete.lParameteValue)
            {
                Protocol_IsErrorOccur = TRUE;
                Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
                return FALSE;
            }
            memset(st_XRpcParamete.lParameteValue, '\0', nBaseLen + 1);
            //拷贝数据
            memcpy(st_XRpcParamete.lParameteValue, ptszBaseBuffer + nHdrLen, nBaseLen - nHdrLen);
            //释放内存
            free(ptszBaseBuffer);
            ptszBaseBuffer = NULL;
        }
        break;
        default:
            Protocol_IsErrorOccur = TRUE;
            Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_NOTSUPPORT;
            return FALSE;
        }
        pStl_ListParamete->push_back(st_XRpcParamete);
    }
    return TRUE;
}
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
BOOL CProtocol_Parse::Protocol_Parse_RPCReponse(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, PROTOCOL_XRPCPARAMETE* pSt_RPCParament)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszFuncName))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析数据
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;
    //解析函数名称
    strcpy(ptszFuncName, st_JsonRoot["XRpc_FunctionName"].asCString());
    //判断是否是无返回值类型
    if (ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_VOID == (ENUM_PROTOCOL_XRPC_PARAMETE_TYPE)st_JsonRoot["XRpc_ReturnType"].asUInt())
    {
        //如果是,直接返回真,没有任何数据返回
        if (NULL != pSt_RPCParament)
        {
            pSt_RPCParament->enXRPC_ParameteType = ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_VOID;
        }
        return TRUE;
    }
    //获得返回的内容
    pSt_RPCParament->enXRPC_ParameteType = (ENUM_PROTOCOL_XRPC_PARAMETE_TYPE)st_JsonRoot["XRpc_ReturnType"].asUInt();
    pSt_RPCParament->nParameteLen = st_JsonRoot["XRpc_ReturnLen"].asInt();
    //判断类型
    switch (pSt_RPCParament->enXRPC_ParameteType)
    {
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BOOL:
    {
        BOOL bValue = st_JsonRoot["XRpc_ReturnValue"].asInt();
        memcpy(pSt_RPCParament->lParameteValue, &bValue, pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT:
    {
        int nValue = st_JsonRoot["XRpc_ReturnValue"].asInt();
        memcpy(pSt_RPCParament->lParameteValue, &nValue, pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_UINT:
    {
        DWORD dwValue = st_JsonRoot["XRpc_ReturnValue"].asUInt();
        memcpy(pSt_RPCParament->lParameteValue, &dwValue, pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_LONGINT:
    {
        __int64x nllValue = st_JsonRoot["XRpc_ReturnValue"].asInt64();
        memcpy(pSt_RPCParament->lParameteValue, &nllValue, pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_ULONG:
    {
        ULONGLONG ullValue = st_JsonRoot["XRpc_ReturnValue"].asUInt64();
        memcpy(pSt_RPCParament->lParameteValue, &ullValue, pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_FLOAT:
    {
        float fValue = st_JsonRoot["XRpc_ReturnValue"].asFloat();
        memcpy(pSt_RPCParament->lParameteValue, &fValue, pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_DOUBLE:
    {
        double dlValue = st_JsonRoot["XRpc_ReturnValue"].asDouble();
        memcpy(pSt_RPCParament->lParameteValue, &dlValue, pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_STRING:
    {
        memcpy(pSt_RPCParament->lParameteValue, st_JsonRoot["XRpc_ReturnValue"].asCString(), pSt_RPCParament->nParameteLen);
    }
    break;
    case ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BIN:
    {
        //二进制数据,我们需要解码后使用
        LPCSTR lpszBinHdr = "XEngine";
        int nHdrLen = strlen(lpszBinHdr);
        int nBaseLen = pSt_RPCParament->nParameteLen * 2;
        CHAR* ptszBaseBuffer = (CHAR*)malloc(nBaseLen);
        if (NULL == ptszBaseBuffer)
        {
            Protocol_IsErrorOccur = TRUE;
            Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_MALLOC;
            return FALSE;
        }
        memset(ptszBaseBuffer, '\0', nBaseLen);
        //进行解码
        nBaseLen = pSt_RPCParament->nParameteLen;
        if (!OPenSsl_Codec_Base64(st_JsonRoot["XRpc_ReturnValue"].asCString(), ptszBaseBuffer, &nBaseLen, FALSE))
        {
            Protocol_IsErrorOccur = TRUE;
            Protocol_dwErrorCode = OPenSsl_GetLastError();
            return FALSE;
        }
        //拷贝数据
        memcpy(pSt_RPCParament->lParameteValue, ptszBaseBuffer + nHdrLen, nBaseLen - nHdrLen);
        //释放内存
        free(ptszBaseBuffer);
        ptszBaseBuffer = NULL;
    }
    break;
    default:
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTROL_MODULE_PROTOCOL_NOTSUPPORT;
        return FALSE;
    }
    return TRUE;
}