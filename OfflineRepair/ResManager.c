#include <stdio.h>
#include <Windows.h>

#include "ResManager.h"

//ÊÍ·Å×ÊÔ´
BOOL ReleaseResource(LPCSTR lpName, LPCSTR lpType, LPCSTR lpFileName) {
	HRSRC hResource = FindResourceA(GetModuleHandle(NULL), lpName, lpType);
	if (hResource) {
		HGLOBAL hg = LoadResource(GetModuleHandle(NULL), hResource);
		LPVOID pData;
		if (!hg) {
			return FALSE;
		}
		pData = LockResource(hg);

		DWORD dwSize = SizeofResource(GetModuleHandle(NULL), hResource);

		HANDLE hFile = CreateFileA(lpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD dwBytesWritten = 0;

		BOOL l_bIsOk = WriteFile(hFile, (LPVOID)pData, dwSize, &dwBytesWritten, NULL);
		CloseHandle(hFile);
		
		printf("Resource \"%s\" release success!\n\n", lpFileName);
		return l_bIsOk;
	}

	printf("Resource \"%s\" release failed!\n\n", lpFileName);
	return FALSE;
}