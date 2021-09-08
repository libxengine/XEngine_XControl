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
BOOL CProtocol_Parse::Protocol_Parse_FuncCall(LPCSTR lpszMsgBuffer, int nMsgLen, CHAR* ptszFuncName, ENUM_PROTOCOL_XRPC_PARAMETE_TYPE* penRtnType, list<PROTOCOL_XRPCPARAMETE>* pStl_ListParamete)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszFuncName) || (NULL == penRtnType) || (NULL == pStl_ListParamete))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_PARAMENT;
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
        Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_PARSE;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
                Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_MALLOC;
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
            Protocol_dwErrorCode = ERROR_CONTRAL_MODULE_PROTOCOL_NOTSUPPORT;
            return FALSE;
        }
        pStl_ListParamete->push_back(st_XRpcParamete);
    }
    return TRUE;
}