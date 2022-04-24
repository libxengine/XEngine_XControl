/********************************************************************
//    Created:     2021/07/09  16:41:16
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Protocol.h
//    File Path:   D:\XEngine_XContral\XEngine_Source
//    File Base:   XContral_Protocol
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     控制协议
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
///////////////////////////////////////////////////////////////////////////
//                          导出的数据定义
///////////////////////////////////////////////////////////////////////////
//后台服务协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_NOTHINGTODO 0x5000       //没有需要执行的任务
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE 0x5001          //下载并且运行一个程序
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEFILE 0x5002        //删除指定文件
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR 0x5003         //删除指定目录
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_UPFILE 0x5004            //上传一个文件到指定FTP
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_GETLIST 0x5005           //请求获取文件夹中的文件列表
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_EXEC 0x5006              //执行指定程序
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_POPMESSAGE 0x5007        //弹出指定消息
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS 0x5008       //结束指定进程
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN 0x5009          //远程关闭计算机
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD 0x5010              //执行命令
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_CONNECT 0x5011           //连接
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_REPORT 0x5012            //报告信息
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_USER 0x50F0              //用户自定义开始