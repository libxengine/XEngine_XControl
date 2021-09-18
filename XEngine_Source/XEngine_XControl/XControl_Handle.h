#pragma once

typedef struct 
{
	BOOL bIsDir;
	CHAR tszFileName[MAX_PATH];
}XCONTRAL_FILEINFO;

BOOL XControl_Handle_PostListFile(CHAR** ppszFileList, int nListCount, LPCSTR lpszPostUrl);