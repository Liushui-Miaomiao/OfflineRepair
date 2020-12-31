#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <direct.h>
#include <conio.h>
#include <io.h>

#include "File.h"

extern const char* g_p_cSlash;

static void RemoveFile(char* filePath);

//����Ŀ¼
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
		//�������·����β����Ӧ�ٽ���һ�δ����ļ���
		if (*(l_p_cTemp + i) == '\0') {
			if (!_access(l_p_cTemp, 0)) {
				//������һ��Ŀ¼����,����ѭ��
				break;
			}

			//������һ��Ŀ¼�����ڣ��򴴽�
			if (!CreateDirectoryA(l_p_cTemp, NULL)) {
				//����ʧ������ѭ��
				break;
			}

			//�����ɹ�������TRUE
			printf("Folder \"%s\" was created successfully!\n\n", l_p_cTemp);
			return TRUE;
		}
		
		//�������·����β
		if (*(l_p_cTemp + i) == '\\') {
			//����Ƿ��Ǵ��̸�Ŀ¼
			if (!l_bSlashIsFrist) {
				l_bSlashIsFrist = TRUE;

				continue;
			}

			//�м䴦��
			*(l_p_cTemp + i) = '\0';
			if (!_access(l_p_cTemp, 0)) {
				//���Ŀ¼����
				*(l_p_cTemp + i) = '\\';
				continue;
			}
			//Ŀ¼�����ڣ��򴴽�
			if (!CreateDirectoryA(l_p_cTemp, NULL)) {
				//����ʧ������ѭ��
				break;
			}

			printf("Folder \"%s\" was created successfully!\n\n", l_p_cTemp);
			*(l_p_cTemp + i) = '\\';
		}
	}

	printf("Folder \"%s\" was created failed!\n\n", l_p_cTemp);
	return FALSE;
}

//�Ƴ�Ŀ¼
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
	
	//����Ŀ¼
	WIN32_FIND_DATA p;
	HANDLE h = FindFirstFileA(l_p_cTempFolderFilter, (LPWIN32_FIND_DATAA)&p);
	do {
		//�ų�����Ŀ¼�͸���Ŀ¼
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

			//�ݹ����Ƿ�Ϊ��Ŀ¼
			if (p.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				RemoveFolder(l_p_cTempPath);
			}
			else {
				//������ɾ���ļ�
				RemoveFile(l_p_cTempPath);
			}

			l_p_cTempPath = NULL;
		}
	} while (FindNextFileA(h, (LPWIN32_FIND_DATAA)&p));

	//�رվ��
	FindClose(h);

	//�Ƴ���Ŀ¼
	if (!_rmdir(folderPath)) {
		printf("Directory \"%s\" was deleted successfully!\n\n", folderPath);
		return TRUE;
	}

	printf("Directory \"%s\" was deleted failed!\n\n", folderPath);
	return FALSE;
}

//�Ƴ��ļ�
static void RemoveFile(char* filePath) {
	if (DeleteFileA(filePath)) {
		printf("File \"%s\" deleted successfully!\n\n", filePath);

		return;
	}

	printf("File \"%s\" failed to delete!\n\n", filePath);
}