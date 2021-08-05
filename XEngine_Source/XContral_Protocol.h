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
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_USER 0x50F0              //用户自定义开始