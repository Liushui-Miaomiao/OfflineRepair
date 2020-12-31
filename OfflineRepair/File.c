#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <direct.h>
#include <conio.h>
#include <io.h>

#include "File.h"

extern const char* g_p_cSlash;

static void RemoveFile(char* filePath);

//创建目录
BOOL CreateFolder(char* folderPath) {
	char* l_p_cTemp;
	l_p_cTemp = (char*)malloc(sizeof(char) * 512);
	if (!l_p_cTemp) {
		return FALSE;
	}
	strcpy_s(l_p_cTemp, strlen(folderPath) + 1, folderPath);

	unsigned int i;
	BOOL l_bSlashIsFrist;
	l_bSlashIsFrist = FALSE;
	for (i = 0; i <= strlen(l_p_cTemp); i++) {
		//如果处于路径结尾，则应再进行一次创建文件夹
		if (*(l_p_cTemp + i) == '\0') {
			if (!_access(l_p_cTemp, 0)) {
				//如果最后一级目录存在,跳出循环
				break;
			}

			//如果最后一级目录不存在，则创建
			if (!CreateDirectoryA(l_p_cTemp, NULL)) {
				//创建失败跳出循环
				break;
			}

			//创建成功，返回TRUE
			printf("Folder \"%s\" was created successfully!\n\n", l_p_cTemp);
			return TRUE;
		}
		
		//如果不是路径结尾
		if (*(l_p_cTemp + i) == '\\') {
			//检测是否是磁盘根目录
			if (!l_bSlashIsFrist) {
				l_bSlashIsFrist = TRUE;

				continue;
			}

			//中间处理
			*(l_p_cTemp + i) = '\0';
			if (!_access(l_p_cTemp, 0)) {
				//如果目录存在
				*(l_p_cTemp + i) = '\\';
				continue;
			}
			//目录不存在，则创建
			if (!CreateDirectoryA(l_p_cTemp, NULL)) {
				//创建失败跳出循环
				break;
			}

			printf("Folder \"%s\" was created successfully!\n\n", l_p_cTemp);
			*(l_p_cTemp + i) = '\\';
		}
	}

	printf("Folder \"%s\" was created failed!\n\n", l_p_cTemp);
	return FALSE;
}

//移除目录
BOOL RemoveFolder(char* folderPath) {
	const char* folderFilter = "\\*.*";
	char* l_p_cTempFolderFilter;
	l_p_cTempFolderFilter = (char*)malloc(sizeof(char) * 512);

	if (!l_p_cTempFolderFilter) {
		puts("Pointer [l_p_cTempFolderFilter] creation failed!");
		_getch();

		exit(EXIT_FAILURE);
	}
	strcpy_s(l_p_cTempFolderFilter, strlen(folderPath) + 1, folderPath);
	strcat_s(l_p_cTempFolderFilter, strlen(folderPath) + strlen(folderFilter) + 1, folderFilter);
	
	//遍历目录
	WIN32_FIND_DATA p;
	HANDLE h = FindFirstFileA(l_p_cTempFolderFilter, (LPWIN32_FIND_DATAA)&p);
	do {
		//排除自身目录和父级目录
		if (strcmp((char* )p.cFileName, ".") && strcmp((char*)p.cFileName, "..")) {
			char* l_p_cTempPath;
			l_p_cTempPath = (char*)malloc(sizeof(char) * 512);
			if (!l_p_cTempPath) {
				puts("Pointer [l_p_cTempPath] creation failed!");
				_getch();

				exit(EXIT_FAILURE);
			}
			strcpy_s(l_p_cTempPath, strlen(folderPath) + 1, folderPath);
			strcat_s(l_p_cTempPath, strlen(folderPath) + strlen(g_p_cSlash) + 1, g_p_cSlash);
			strcat_s(l_p_cTempPath, strlen(folderPath) + strlen(g_p_cSlash) + strlen((char*)p.cFileName) + 1, (char*)p.cFileName);

			//递归检查是否为子目录
			if (p.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				RemoveFolder(l_p_cTempPath);
			}
			else {
				//不是则删除文件
				RemoveFile(l_p_cTempPath);
			}

			l_p_cTempPath = NULL;
		}
	} while (FindNextFileA(h, (LPWIN32_FIND_DATAA)&p));

	//关闭句柄
	FindClose(h);

	//移除空目录
	if (!_rmdir(folderPath)) {
		printf("Directory \"%s\" was deleted successfully!\n\n", folderPath);
		return TRUE;
	}

	printf("Directory \"%s\" was deleted failed!\n\n", folderPath);
	return FALSE;
}

//移除文件
static void RemoveFile(char* filePath) {
	if (DeleteFileA(filePath)) {
		printf("File \"%s\" deleted successfully!\n\n", filePath);

		return;
	}

	printf("File \"%s\" failed to delete!\n\n", filePath);
}