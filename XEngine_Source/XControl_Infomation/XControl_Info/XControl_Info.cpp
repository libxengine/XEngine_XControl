#include "pch.h"
#include "XControl_Info.h"
/********************************************************************
//    Created:     2021/09/08  10:12:32
//    File Name:   D:\XEngine_XControl\XEngine_Source\XControl_Infomation\XControl_Info\XControl_Info.cpp
//    File Path:   D:\XEngine_XControl\XEngine_Source\XControl_Infomation\XControl_Info
//    File Base:   XControl_Info
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息收集类
//    History:
*********************************************************************/
CXControl_Info::CXControl_Info()
{

}
CXControl_Info::~CXControl_Info()
{

}
//////////////////////////////////////////////////////////////////////////
//                          公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XControl_Info_HardWare
函数功能：获取硬件信息
 参数.一：ptszHWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的数据,这个数据是JSON格式
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出数据的长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXControl_Info::XControl_Info_HardWare(CHAR *ptszHWInfo,int *pInt_Len)
{
    BackManage_IsErrorOccur = FALSE;

    if ((NULL == ptszHWInfo) || (NULL == pInt_Len))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_HARDWARE_PARAMENT;
        return FALSE;
    }
    int nDiskNumber = 0;
    CHAR** pptszRootName;
    CHAR tszOSName[MAX_PATH];
    DWORD nOSVersion;
    DWORD nOSBuild;
    DWORD nOSPro;
    CHAR tszOSInfo[1024];
    SYSTEMAPI_DISK_INFOMATION st_DiskInfo;
    SYSTEMAPI_CPU_INFOMATION st_CPUInfo;
    SYSTEMAPI_MEMORY_INFOMATION st_MemoryInfo;
    SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;

    memset(tszOSName,'\0',sizeof(tszOSName));
    memset(tszOSInfo,'\0',sizeof(tszOSInfo));
    memset(&st_MemoryInfo, '\0', sizeof(SYSTEMAPI_MEMORY_INFOMATION));
    memset(&st_CPUInfo,'\0',sizeof(SYSTEMAPI_CPU_INFOMATION));
    memset(&st_DiskInfo,'\0',sizeof(SYSTEMAPI_DISK_INFOMATION));
    memset(&st_SDKSerial,'\0',sizeof(SYSTEMAPI_SERIAL_INFOMATION));

    if (!SystemApi_HardWare_GetDiskNumber(&pptszRootName, &nDiskNumber))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    BaseLib_OperatorMemory_Free((XPPPMEM)&pptszRootName, nDiskNumber);

    CHAR tszDriveStr[MAX_PATH];
    memset(tszDriveStr, '\0', MAX_PATH);
    
#ifdef _WINDOWS
    GetLogicalDriveStringsA(MAX_PATH, tszDriveStr);
#else
    LPCSTR lpszDir = _T("/");
    strcpy(tszDriveStr, lpszDir);
#endif

    if (!SystemApi_HardWare_GetDiskInfomation(tszDriveStr,&st_DiskInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_HardWare_GetCpuInfomation(&st_CPUInfo))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetMemoryUsage(&st_MemoryInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_HardWare_GetSerial(&st_SDKSerial))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetSystemVer(tszOSName,&nOSVersion,&nOSBuild,&nOSPro))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }

    Json::Value st_JsonRoot;
    Json::Value st_JsonDisk;
    Json::Value st_JsonCpu;
    Json::Value st_JsonSerial;
    Json::Value st_JsonMemory;
    Json::Value st_JsonNetCard;

    st_JsonDisk["DiskNumber"] = nDiskNumber;
    st_JsonDisk["DiskFree"] = (Json::UInt64)st_DiskInfo.dwDiskFree;
    st_JsonDisk["DiskTotal"] = (Json::UInt64)st_DiskInfo.dwDiskTotal;
    st_JsonDisk["DiskName"] = tszDriveStr;

    st_JsonCpu["CpuNumber"] = st_CPUInfo.nCpuNumber;
    st_JsonCpu["CpuSpeed"] = st_CPUInfo.nCpuSpeed;
    st_JsonCpu["CpuName"] = st_CPUInfo.tszCpuName;

    st_JsonMemory["MemoryFree"] = (Json::UInt64)st_MemoryInfo.dwMemory_Free;
    st_JsonMemory["MemoryTotal"] = (Json::UInt64)st_MemoryInfo.dwMemory_Total;

    st_JsonSerial["DiskSerial"] = st_SDKSerial.tszDiskSerial;
    st_JsonSerial["CpuSerial"] = st_SDKSerial.tszCpuSerial;
    st_JsonSerial["BoardSerial"] = st_SDKSerial.tszBoardSerial;
    st_JsonSerial["SystemSerail"] = st_SDKSerial.tszSystemSerail;

	int nListCount = 0;
    NETXAPI_CARDINFO** ppSt_ListIFInfo;
    NetXApi_Socket_GetCardInfo(&ppSt_ListIFInfo, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
        Json::Value st_JsonIPAddr;
        st_JsonIPAddr["tszIFName"] = ppSt_ListIFInfo[i]->tszIFName;
        st_JsonIPAddr["tszIPAddr"] = ppSt_ListIFInfo[i]->tszIPAddr;
        st_JsonIPAddr["tszBroadAddr"] = ppSt_ListIFInfo[i]->tszBroadAddr;
        st_JsonIPAddr["tszDnsAddr"] = ppSt_ListIFInfo[i]->tszDnsAddr;
        st_JsonIPAddr["tszMacAddr"] = ppSt_ListIFInfo[i]->tszMacAddr;
        st_JsonNetCard.append(st_JsonIPAddr);
	}
    BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListIFInfo, nListCount);

    st_JsonRoot["Disk"] = st_JsonDisk;
    st_JsonRoot["Cpu"] = st_JsonCpu;
    st_JsonRoot["Memory"] = st_JsonMemory;
    st_JsonRoot["Serial"] = st_JsonSerial;
    st_JsonRoot["NetCard"] = st_JsonNetCard;

    sprintf(tszOSInfo, "%s %lu %lu %lu", tszOSName, nOSVersion, nOSBuild, nOSPro);
    st_JsonRoot["Platfrom"] = tszOSInfo;

    if (*pInt_Len < (int)st_JsonRoot.toStyledString().length())
    {
        *pInt_Len = st_JsonRoot.toStyledString().length();
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_HARDWARE_LEN;
        return FALSE;
    }
    *pInt_Len = st_JsonRoot.toStyledString().length();
    memcpy(ptszHWInfo,st_JsonRoot.toStyledString().c_str(),*pInt_Len);

    return TRUE;
}
/********************************************************************
函数名称：XControl_Info_SoftWare
函数功能：获取软件系统信息
 参数.一：ptszSWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出系统信息JSON结构
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出系统信息长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXControl_Info::XControl_Info_SoftWare(CHAR *ptszSWInfo,int *pInt_Len)
{
    BackManage_IsErrorOccur = FALSE;

    if ((NULL == ptszSWInfo) || (NULL == pInt_Len))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_PARAMENT;
        return FALSE;
    }
    int nProcessCount;
    DWORD nOSBuild;
    DWORD nOSProcessor;
    DWORD nOSVersion;
    CHAR tszOSInfo[MAX_PATH];
    CHAR tszUPTime[MAX_PATH];
    CHAR tszOSUser[MAX_PATH];
    CHAR tszServicePacket[MAX_PATH];
    XENGINE_LIBTIMER st_LibTimer;

    memset(tszOSInfo, '\0', MAX_PATH);
    memset(tszUPTime, '\0', MAX_PATH);
    memset(tszOSUser, '\0', MAX_PATH);
    memset(tszServicePacket, '\0', MAX_PATH);
    memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

#ifdef _WINDOWS
	DWORD dwMaxSize = MAX_PATH;
	if (!GetComputerNameA(tszOSUser, &dwMaxSize))
	{
		BackManage_IsErrorOccur = TRUE;
		BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_GETNAME;
		return FALSE;
	}
#else
	struct passwd* pSt_Passwd = NULL;
	pSt_Passwd = getpwuid(getuid());
	if (NULL == pSt_Passwd)
	{
		BackManage_IsErrorOccur = TRUE;
		BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_GETNAME;
		return FALSE;
	}
    strcpy(tszOSUser, pSt_Passwd->pw_name);
#endif
    if (!SystemApi_System_GetSystemVer(tszOSInfo,&nOSVersion,&nOSBuild,&nOSProcessor))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetProcessCount(&nProcessCount))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetUpTime(&st_LibTimer))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    sprintf(tszUPTime, "%04d-%02d-%02d %02d:%02d:%02d", st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);

    Json::Value st_JsonRoot;
    Json::Value st_JsonSystem;

    st_JsonSystem["OSUser"] = tszOSUser;
    st_JsonSystem["OSUPTime"] = tszUPTime;
    st_JsonSystem["OSVersion"] = tszOSInfo;
    st_JsonSystem["OSProcessCount"] = nProcessCount;
    st_JsonRoot["OSInfo"] = st_JsonSystem;

    if (*pInt_Len < (int)st_JsonRoot.toStyledString().length())
    {
        *pInt_Len = st_JsonRoot.toStyledString().length();
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_LEN;
        return FALSE;
    }
    *pInt_Len = st_JsonRoot.toStyledString().length();
    memcpy(ptszSWInfo, st_JsonRoot.toStyledString().c_str(), *pInt_Len);

    return TRUE;
}
