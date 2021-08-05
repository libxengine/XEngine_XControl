#include "XContral_Hdr.h"

BOOL XContral_Parament(int argc, char **argv, MANAGESERVICE_CONFIG *pSt_StartlParam)
{
    LPCTSTR lpszAuthCfg = _T("./XContral_Config/XContral_Config.ini");

    GetPrivateProfileString(_T("ServiceManage"), _T("TaskUrl"), NULL, pSt_StartlParam->tszTaskUrl, MAX_PATH, lpszAuthCfg);
    pSt_StartlParam->bIsAutoStart = GetPrivateProfileInt(_T("ServiceManage"), _T("AutoStart"), 0, lpszAuthCfg);
    pSt_StartlParam->bIsHideWnd = GetPrivateProfileInt(_T("ServiceManage"), _T("HideWnd"), 0, lpszAuthCfg);
    pSt_StartlParam->nTaskTime = GetPrivateProfileInt(_T("ServiceManage"), _T("TaskTime"), 0, lpszAuthCfg);
    pSt_StartlParam->nLogType = GetPrivateProfileInt(_T("LogConfig"), _T("LogType"), 0, lpszAuthCfg);

    pSt_StartlParam->st_Client.bEnable = GetPrivateProfileInt(_T("ClientConfig"), _T("bEnable"), 0, lpszAuthCfg);
    pSt_StartlParam->st_Client.nPort = GetPrivateProfileInt(_T("ClientConfig"), _T("nPort"), 0, lpszAuthCfg);
    pSt_StartlParam->st_Client.nIPType = GetPrivateProfileInt(_T("ClientConfig"), _T("nIPType"), 0, lpszAuthCfg);
    GetPrivateProfileString(_T("ClientConfig"), _T("bEnable"), NULL, pSt_StartlParam->st_Client.tszServiceAddr, sizeof(pSt_StartlParam->st_Client.tszServiceAddr), lpszAuthCfg);

    for (int i = 0;i < argc;i++)
    {
        if ((0 == _tcscmp("-h", argv[i])) || (0 == _tcscmp("-H", argv[i])))
        {
            XContral_ParamentHelp();
            return FALSE;
        }
        if ((0 == _tcscmp("-v", argv[i])) || (0 == _tcscmp("-V", argv[i])))
        {
            printf("Version：V2.0.0\n");
            return FALSE;
        }
        else if (0 == _tcscmp("-a", argv[i]))
        {
            pSt_StartlParam->bIsAutoStart = 1;
        }
        else if (0 == _tcscmp("-w", argv[i]))
        {
            pSt_StartlParam->bIsHideWnd = 1;
        }
        else if (0 == _tcscmp("-l", argv[i]))
        {
            pSt_StartlParam->nLogType = _ttoi(argv[i] + 1);
        }
        else if (0 == _tcscmp("-c", argv[i]))
        {
            pSt_StartlParam->bCreateEmail = 1;
        }
    }
    return TRUE;
}
BOOL XContral_Parament_EMail(MANAGESERVICE_CONFIG* pSt_StartlParam)
{
    TCHAR tszEnBuffer[4096];
    TCHAR tszDeBuffer[4096];
    LPCTSTR lpszSrcFile = _T("./XContral_Config/XContral_EMail.ini.dat");
    LPCTSTR lpszDstFile = _T("./XContral_Config/XContral_EMail.ini.bak");

    memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
    memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

    pSt_StartlParam->st_EMail.pStl_ListAddr = new list<string>;

    FILE* pSt_EnFile = _tfopen(lpszSrcFile, _T("rb"));
    FILE* pSt_DeFile = _tfopen(lpszDstFile, _T("wb"));
    if (NULL == pSt_EnFile || NULL == pSt_DeFile)
    {
        XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，获取电子邮件信息文件失败,无法发送邮件信息，错误:%d"), errno);
        return FALSE;
    }
    int nERet = fread(tszEnBuffer, 1, sizeof(tszEnBuffer), pSt_EnFile);
    if (nERet <= 0)
    {
        XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，获取电子邮件配置信息失败,无法发送邮件信息，错误:%d"), errno);
        return FALSE;
    }
    if (!OPenSsl_XCrypto_Decoder(tszEnBuffer, &nERet, tszDeBuffer, _T("xengineemail")))
    {
        XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，解密电子邮件信息失败,无法发送邮件信息，错误:%lX"), OPenSsl_GetLastError());
        return FALSE;
    }
    fwrite(tszDeBuffer, 1, nERet, pSt_DeFile);
    fclose(pSt_EnFile);
    fclose(pSt_DeFile);

    pSt_StartlParam->st_EMail.st_EMailSmtp.bIsCall = FALSE;
	GetPrivateProfileString(_T("Email"), _T("SmtpAddr"), NULL, pSt_StartlParam->st_EMail.st_EMailSmtp.tszServiceAddr, MAX_PATH, lpszDstFile);
	GetPrivateProfileString(_T("Email"), _T("SmtpUser"), NULL, pSt_StartlParam->st_EMail.st_EMailSmtp.tszUserName, MAX_PATH, lpszDstFile);
	GetPrivateProfileString(_T("Email"), _T("SmtpPass"), NULL, pSt_StartlParam->st_EMail.st_EMailSmtp.tszPassWord, MAX_PATH, lpszDstFile);
	GetPrivateProfileString(_T("Email"), _T("SmtpFrom"), NULL, pSt_StartlParam->st_EMail.st_EMailSmtp.tszFromAddr, MAX_PATH, lpszDstFile);

    int i = 0;
    while (1)
    {
        i++;
        TCHAR tszSendKey[MAX_PATH];
        TCHAR tszSendValue[MAX_PATH];

        memset(tszSendKey, '\0', sizeof(tszSendKey));
        memset(tszSendValue, '\0', sizeof(tszSendValue));

        _stprintf(tszSendKey, _T("MailAddr%d"), i);
        if (GetPrivateProfileString(_T("SendTo"), tszSendKey, NULL, tszSendValue, MAX_PATH, lpszDstFile) <= 0)
        {
            break;
        }
        pSt_StartlParam->st_EMail.pStl_ListAddr->push_back(tszSendValue);
    }
    XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，获得电子邮件配置信息,将发送数据"));

    pSt_StartlParam->st_EMail.bEnable = TRUE;
    _tremove(lpszDstFile);
    return TRUE;
}
void XContral_ParamentHelp()
{
    printf(_T("--------------------------启动参数帮助开始--------------------------\n"));
    printf(_T("后台管理服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的ini配置文件里面的参数\n"));
    printf(_T("-h or -H：启动参数帮助提示信息\n"));
    printf(_T("-a：设置服务跟随机器一起启动\n"));
    printf(_T("-w：设置窗口隐藏启动\n"));
    printf(_T("-l：设置日志模式,0 文件 1 控制台 2全部\n"));
    printf(_T("-c：加密邮件配置,无参数.待加密文件:./Manage_Config/Manage_EMail.ini 加密后的文件:./Manage_Config/Manage_EMail.ini.dat\n"));
    printf(_T("    如果你需要把信息发送到自己的电子邮箱,可以使用此功能,注意:你必须按照格式放置文件.加密完成后可以删除Manage_EMail.ini而保留Manage_EMail.ini.dat\n"));
    printf(_T("--------------------------启动参数帮助结束--------------------------\n"));
}
