#include "OfflineRepair.h"

#define FILE_NAME_LEN 8

const char* g_p_cSlash = "\\";
const char* g_p_cHeuName = "8F903698240FE799F61EEDA8595181137B996156.heu";
const char* g_p_cSwzName = "8F903698240FE799F61EEDA8595181137B996156.swz";
const char* g_p_cTxtName = "cacheSize.txt";

int main(void) {
	SetConsoleTitleA("Offline Repair");
	
	char* p_cAppDataPath;
	p_cAppDataPath = NULL;
	size_t sz;
	sz = 0;

	//取用户 %AppData% 目录
	if (_dupenv_s(&p_cAppDataPath, &sz, "AppData") || !p_cAppDataPath) {
		puts("Fail to get the user AppData directory!");

		free(p_cAppDataPath);
		exit(EXIT_FAILURE);
	}

	srand((unsigned int)time(NULL));

	//取 %AppData%\Adobe\Flash Player\AssetCache 目录
	char* p_cAdobeCachePath;
	p_cAdobeCachePath = (char*)malloc(sizeof(char) * 512);
	if (!p_cAdobeCachePath) {
		puts("Pointer [p_cAdobeCachePath] creation failed!");
		_getch();

		exit(EXIT_FAILURE);
	}
	const char* p_cCachePath = "\\Adobe\\Flash Player\\AssetCache";
	strcpy_s(p_cAdobeCachePath, strlen(p_cAppDataPath) + 1, p_cAppDataPath);
	strcat_s(p_cAdobeCachePath, strlen(p_cAppDataPath) + strlen(p_cCachePath) + 1, p_cCachePath);

	//返回0表示存在，-1不存在
	if (!_access(p_cAdobeCachePath, 0)) {
		//存在则清空目录
		RemoveFolder(p_cAdobeCachePath);
	}

	//清空完毕再次创建
	if (!CreateFolder(p_cAdobeCachePath)) {
		exit(EXIT_FAILURE);
	}

	//取 %AppData%\Adobe\Flash Player\AssetCache\随机八位字符 目录
	char* p_cRandomFolderName;
	p_cRandomFolderName = GetRandomStr(FILE_NAME_LEN);
	char* p_cTempFolder;
	p_cTempFolder = (char*)malloc(sizeof(char) * 512);
	if (!p_cTempFolder) {
		puts("Pointer [p_cTempFolder] creation failed!");
		_getch();

		exit(EXIT_FAILURE);
	}
	strcpy_s(p_cTempFolder, strlen(p_cAdobeCachePath) + 1, p_cAdobeCachePath);
	strcat_s(p_cTempFolder, strlen(p_cAdobeCachePath) + strlen(g_p_cSlash) + 1, g_p_cSlash);
	strcat_s(p_cTempFolder, strlen(p_cAdobeCachePath) + strlen(g_p_cSlash) + strlen(p_cRandomFolderName) + 1, p_cRandomFolderName);

	if (_access(p_cTempFolder, 0) == -1) {
		if (!CreateFolder(p_cTempFolder)) {
			exit(EXIT_FAILURE);
		}
	}

	//取 %AppData%\Adobe\Flash Player\AssetCache\随机八位字符\目标文件 目录
	char* p_cHeuPath;
	p_cHeuPath = (char*)malloc(sizeof(char) * 512);
	char* p_cSwzPath;
	p_cSwzPath = (char*)malloc(sizeof(char) * 512);
	char* p_cTxtPath;
	p_cTxtPath = (char*)malloc(sizeof(char) * 512);

	if (p_cHeuPath && p_cSwzPath && p_cTxtPath) {
		strcpy_s(p_cHeuPath, strlen(p_cTempFolder) + 1, p_cTempFolder);
		strcat_s(p_cHeuPath, strlen(p_cTempFolder) + strlen(g_p_cSlash) + 1, g_p_cSlash);
		strcat_s(p_cHeuPath, strlen(p_cTempFolder) + strlen(g_p_cSlash) + strlen(g_p_cHeuName) + 1, g_p_cHeuName);

		strcpy_s(p_cSwzPath, strlen(p_cTempFolder) + 1, p_cTempFolder);
		strcat_s(p_cSwzPath, strlen(p_cTempFolder) + strlen(g_p_cSlash) + 1, g_p_cSlash);
		strcat_s(p_cSwzPath, strlen(p_cTempFolder) + strlen(g_p_cSlash) + strlen(g_p_cSwzName) + 1, g_p_cSwzName);

		strcpy_s(p_cTxtPath, strlen(p_cTempFolder) + 1, p_cTempFolder);
		strcat_s(p_cTxtPath, strlen(p_cTempFolder) + strlen(g_p_cSlash) + 1, g_p_cSlash);
		strcat_s(p_cTxtPath, strlen(p_cTempFolder) + strlen(g_p_cSlash) + strlen(g_p_cTxtName) + 1, g_p_cTxtName);
	}

	//释放资源文件
	if (!(ReleaseResource((LPCSTR)IDR_RES_HEU1, "RES_HEU", p_cHeuPath) && 
		ReleaseResource((LPCSTR)IDR_RES_SWZ1, "RES_SWZ", p_cSwzPath) && 
		ReleaseResource((LPCSTR)IDR_RES_TXT1, "RES_TXT", p_cTxtPath))) {
		puts("Repair failed!\n\n");
		puts("Please press any key to exit...");
		_getch();

		exit(EXIT_FAILURE);
	}

	puts("Repair sucess!\n\n");
	puts("Please press any key to exit...");
	_getch();

	return EXIT_SUCCESS;
}