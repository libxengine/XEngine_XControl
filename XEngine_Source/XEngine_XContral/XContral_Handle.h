#pragma once

typedef struct 
{
	BOOL bIsDir;
	TCHAR tszFileName[MAX_PATH];
}XCONTRAL_FILEINFO;

BOOL XContral_Handle_PostListFile(CHAR** ppszFileList, int nListCount, LPCTSTR lpszPostUrl);