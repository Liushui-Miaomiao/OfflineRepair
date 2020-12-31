#include "Random.h"

#include <Windows.h>

static char GetRandomChar(void);

//取指定长度的随机字符串
char* GetRandomStr(int length) {
	if (length <= 0) {
		return NULL;
	}

	char* l_pRandomStr;
	l_pRandomStr = (char*)malloc(sizeof(char) * length);
	if (!l_pRandomStr) {
		return NULL;
	}

	int i;
	for (i = 0; i < length; i++) {
		*(l_pRandomStr + i) = GetRandomChar();
	}
	*(l_pRandomStr + length) = '\0';

	return l_pRandomStr;
}

//取随机字符
static char GetRandomChar(void) {
	char l_cCh;

	if (rand() % 2) {
		l_cCh = 'A' + rand() % 26;
	}
	else {
		l_cCh = '0' + rand() % 10;
	}

	return l_cCh;
}