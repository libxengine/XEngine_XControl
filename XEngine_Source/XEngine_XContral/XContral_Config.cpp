﻿#include "XContral_Hdr.h"

BOOL XContral_Parament(int argc, char **argv)
{

    LPCSTR lpszBaseCfg = "./XContral_Config/XEngine_Config.json";
    LPCSTR lpszListCfg = "./XContral_Config/XEngine_ConfigList.json";

    if (!Config_Json_File(lpszBaseCfg, &st_ServiceConfig))
    {
        return FALSE;
    }
    if (!Config_Json_LoadList(lpszListCfg, &st_APPConfig))
    {
        return FALSE;
    }

    for (int i = 0;i < argc;i++)
    {
        if ((0 == strcmp("-h", argv[i])) || (0 == strcmp("-H", argv[i])))
        {
            XContral_ParamentHelp();
            return FALSE;
        }
        if ((0 == strcmp("-v", argv[i])) || (0 == strcmp("-V", argv[i])))
        {
            printf("Version：%s\n", st_ServiceConfig.st_Version.pStl_ListVer->front().c_str());
            return FALSE;
        }
        else if (0 == strcmp("-a", argv[i]))
        {
            st_ServiceConfig.bAutoStart = TRUE;
        }
        else if (0 == strcmp("-w", argv[i]))
        {
            st_ServiceConfig.bHideWnd = TRUE;
        }
        else if (0 == strcmp("-c", argv[i]))
        {
            st_EMailConfig.bCreateEmail = TRUE;
        }
    }
    return TRUE;
}
BOOL XContral_Parament_EMail()
{
    CHAR tszEnBuffer[4096];
    CHAR tszDeBuffer[4096];
    LPCSTR lpszSrcFile = "./XContral_Config/XContral_EMail.ini.dat";
    LPCSTR lpszDstFile = "./XContral_Config/XContral_EMail.ini.bak";

    memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
    memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

    st_EMailConfig.pStl_ListAddr = new list<string>;

    FILE* pSt_EnFile = fopen(lpszSrcFile, "rb");
    FILE* pSt_DeFile = fopen(lpszDstFile, "wb");
    if (NULL == pSt_EnFile || NULL == pSt_DeFile)
    {
        XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，获取电子邮件信息文件失败,无法发送邮件信息，错误:%d", errno);
        return FALSE;
    }
    int nERet = fread(tszEnBuffer, 1, sizeof(tszEnBuffer), pSt_EnFile);
    if (nERet <= 0)
    {
        XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，获取电子邮件配置信息失败,无法发送邮件信息，错误:%d", errno);
        return FALSE;
    }
    if (!OPenSsl_XCrypto_Decoder(tszEnBuffer, &nERet, tszDeBuffer, "xengineemail"))
    {
        XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "启动服务中，解密电子邮件信息失败,无法发送邮件信息，错误:%lX", OPenSsl_GetLastError());
        return FALSE;
    }
    fwrite(tszDeBuffer, 1, nERet, pSt_DeFile);
    fclose(pSt_EnFile);
    fclose(pSt_DeFile);

    st_EMailConfig.st_EMailSmtp.bIsCall = FALSE;
	BaseLib_OperatorFile_ReadProfileToString(lpszDstFile, "Email", "SmtpAddr", st_EMailConfig.st_EMailSmtp.tszServiceAddr);
	BaseLib_OperatorFile_ReadProfileToString(lpszDstFile, "Email", "SmtpUser", st_EMailConfig.st_EMailSmtp.tszUserName);
	BaseLib_OperatorFile_ReadProfileToString(lpszDstFile, "Email", "SmtpPass", st_EMailConfig.st_EMailSmtp.tszPassWord);
	BaseLib_OperatorFile_ReadProfileToString(lpszDstFile, "Email", "SmtpFrom", st_EMailConfig.st_EMailSmtp.tszFromAddr);

    int i = 0;
    while (1)
    {
        i++;
        CHAR tszSendKey[MAX_PATH];
        CHAR tszSendValue[MAX_PATH];

        memset(tszSendKey, '\0', sizeof(tszSendKey));
        memset(tszSendValue, '\0', sizeof(tszSendValue));

        sprintf(tszSendKey, "MailAddr%d", i);
        if (BaseLib_OperatorFile_ReadProfileToString(lpszDstFile, "SendTo", tszSendKey, tszSendValue) <= 0)
        {
            break;
        }
        st_EMailConfig.pStl_ListAddr->push_back(tszSendValue);
    }
    XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, "启动服务中，获得电子邮件配置信息,将发送数据");

    st_EMailConfig.bEnable = TRUE;
    remove(lpszDstFile);
    return TRUE;
}
void XContral_ParamentHelp()
{
    printf("--------------------------启动参数帮助开始--------------------------\n");
    printf("后台管理服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的ini配置文件里面的参数\n");
    printf("-h or -H：启动参数帮助提示信息\n");
    printf("-a：设置服务跟随机器一起启动\n");
    printf("-w：设置窗口隐藏启动\n");
    printf("-c：加密邮件配置,无参数.待加密文件:./Manage_Config/Manage_EMail.ini 加密后的文件:./Manage_Config/Manage_EMail.ini.dat\n");
    printf("    如果你需要把信息发送到自己的电子邮箱,可以使用此功能,注意:你必须按照格式放置文件.加密完成后可以删除Manage_EMail.ini而保留Manage_EMail.ini.dat\n");
    printf("--------------------------启动参数帮助结束--------------------------\n");
}
