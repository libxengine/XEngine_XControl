/********************************************************************
//    Created:     2021/07/09  16:41:16
//    File Name:   D:\XEngine_XContral\XEngine_Source\XContral_Protocol.h
//    File Path:   D:\XEngine_XContral\XEngine_Source
//    File Base:   XContral_Protocol
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     ����Э��
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        ������ö����
//////////////////////////////////////////////////////////////////////////
//����
typedef enum
{
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_UNKNOW = 0,                           //δ֪����,����˿���Ϊ0,��ָ��
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_VOID = 1,                             //������,ʹ�ô�������Ϊ����ֵ,���������������κζ���
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BOOL = 2,                             //�߼���
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_INT = 3,                              //������
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_UINT = 4,                             //�޷���������
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_LONGINT = 5,                          //��������
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_ULONG = 6,                            //�޷��ų�������
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_FLOAT = 7,                            //������
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_DOUBLE = 8,                           //˫���ȸ�����
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_STRING = 9,                           //�ַ���
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE_BIN = 10                              //����������
}ENUM_PROTOCOL_XRPC_PARAMETE_TYPE, * LPENUM_PROTOCOL_XRPC_PARAMETE_TYPE;
//////////////////////////////////////////////////////////////////////////
//                        ���������ݽṹ
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	ENUM_PROTOCOL_XRPC_PARAMETE_TYPE enXRPC_ParameteType;                  //��������
	int nParameteLen;                                                     //������С,��ֵ����������ֵ����Ϊ0
	LPVOID lParameteValue;                                                //�Զ������ֵ
}PROTOCOL_XRPCPARAMETE, * LPPROTOCOL_XRPCPARAMETE;
///////////////////////////////////////////////////////////////////////////
//                          ���������ݶ���
///////////////////////////////////////////////////////////////////////////
//��̨����Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_NOTHINGTODO 0x5000       //û����Ҫִ�е�����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE 0x5001          //���ز�������һ������
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEFILE 0x5002        //ɾ��ָ���ļ�
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR 0x5003         //ɾ��ָ��Ŀ¼
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_UPFILE 0x5004            //�ϴ�һ���ļ���ָ��FTP
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_GETLIST 0x5005           //�����ȡ�ļ����е��ļ��б�
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_EXEC 0x5006              //ִ��ָ������
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_POPMESSAGE 0x5007        //����ָ����Ϣ
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS 0x5008       //����ָ������
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN 0x5009          //Զ�̹رռ����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD 0x5010              //ִ������
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_CONNECT 0x5011           //����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_USER 0x50F0              //�û��Զ��忪ʼ