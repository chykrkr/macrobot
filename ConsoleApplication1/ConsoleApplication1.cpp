﻿// ConsoleApplication1.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "windows.h"

#include "io.h"
#include "fcntl.h"
#include <vector>

using namespace std;

#define MAX_WINTEXT_BUFFER 99

#define WAIT_TIME 13

//#define WORK_MODE_WND_SELECT
#define WORK_MODE_DELAYED_RUN

struct MatchedHWND {
	HWND hwnd;
	LPWSTR wname;
};

struct FindWindowParam {
	vector<MatchedHWND> * hwndList;
	LPCWSTR wname;
};

int MultiByteToWideCharHelper(LPCCH src, LPWSTR  * dest)
{
	int size;

	if (dest == NULL)
		return -1;

	size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, -1, NULL, 0);

	/* Error or null string */
	if (size < 1)
		return size;

	*dest = (LPWSTR) malloc(sizeof(WCHAR) * size);

	size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, -1, *dest, size);

	return size;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	WCHAR buffer[MAX_WINTEXT_BUFFER + 1];

	GetWindowText(hwnd, buffer, MAX_WINTEXT_BUFFER);

	wprintf(L"%s, 0x%X\n", buffer, (unsigned int)hwnd);

	return TRUE;
}

BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam)
{
	WCHAR buffer[MAX_WINTEXT_BUFFER + 1];
	LPWSTR copy;
	int ret;
	struct FindWindowParam * param = (struct FindWindowParam *) lParam;

	ret = GetWindowText(hwnd, buffer, MAX_WINTEXT_BUFFER);

	if (ret < 1)
		return TRUE;

	if (wcsncmp(buffer, param->wname, wcslen(param->wname)) == 0) {
		copy = (LPWSTR) malloc(sizeof(WCHAR) * (wcslen(buffer) + 1));
		lstrcpyW(copy, buffer);
		param->hwndList->push_back({hwnd, copy });
	}

	return TRUE;
}

vector<MatchedHWND> * findWindowByTitle(LPCWSTR wname)
{
	//struct FindWindowParam ret = {.hwnd = NULL, .wname = wname};
	struct FindWindowParam ret = {NULL, wname};

	ret.hwndList = new vector<MatchedHWND>();

	EnumWindows(FindWindowProc, (LPARAM) &ret);

	return ret.hwndList;
}

int mouseClick(DWORD up, DWORD down)
{
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &ip, sizeof(INPUT));

	ip.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &ip, sizeof(INPUT));
	
	return 0;
}

int sendKey(WORD vk)
{
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	//press
	ip.ki.wVk = vk;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	//release
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	return 0;
}

HWND selectWindowHandle(LPWSTR wname)
{
	HWND handle = NULL;
	vector<MatchedHWND> * handles = NULL;
	int ret;
	int idx;
	WCHAR buffer[MAX_WINTEXT_BUFFER + 1];

	//EnumWindows(EnumWindowsProc, NULL);

	handles = findWindowByTitle(wname);

	wprintf(L"--------------------------------\n");

	for (unsigned int i = 0; handles != NULL && i < handles->size(); i++) {
		MatchedHWND matched = (*handles)[i];
		wprintf(L"[%d]: %s, 0x%X\n", i, matched.wname, (unsigned int)matched.hwnd);
	}

	idx = -1;

	if (handles != NULL && handles->size() > 0) {
		idx = 0;
	}

	if (handles->size() > 1) {
		wprintf(L"There are multiple matched handles. Choose one : ");
		ret = wscanf_s(L"%10s", buffer, (unsigned)_countof(buffer));

		if (ret > 0) {
			idx = _wtoi(buffer);
		}
	}

	if (idx >= 0 && idx < handles->size()) {
		handle = (*handles)[idx].hwnd;
	}
	else {
		handle = NULL;
	}

	//*Handle selected.
	if (handles != NULL) {
		for (int i = 0; handles != NULL && i < handles->size(); i++) {
			MatchedHWND matched = (*handles)[i];

			if (matched.wname == NULL)
				continue;

			free(matched.wname);
		}

		delete handles;
	}

	return handle;
}

int confirmWindowSelection(HWND handle)
{
	WCHAR buffer[MAX_WINTEXT_BUFFER + 1];
	int ret = FALSE;

	if (handle == NULL)
		return FALSE;

	SetForegroundWindow(handle);

	wprintf(L"Proceed (Y/N)? : ");
	ret = wscanf_s(L"%10s", buffer, (unsigned)_countof(buffer));

	ret = (wcsncmp(buffer, L"Y", 1) == 0) ? TRUE : FALSE;

	return ret;
}

void doWork()
{
	while (TRUE) {
		mouseClick(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
		Sleep(WAIT_TIME * 1000);

		sendKey(VK_RETURN);
		Sleep(WAIT_TIME * 1000);
	}
}

int main(int argc, char * argv[])
{
	LPWSTR param = NULL;
	HWND handle = NULL;
	int ret = FALSE;
	DWORD sleepTime = 0;

	if (argc < 2) {
		printf("Insufficient arguments");
		return -1;
	}

	_setmode(_fileno(stdout), _O_U16TEXT);

	ret = MultiByteToWideCharHelper(argv[1], &param);

#ifdef WORK_MODE_WND_SELECT
	EnumWindows(EnumWindowsProc, NULL);
	handle = selectWindowHandle(param);
	ret = confirmWindowSelection(handle);
	sleepTime = 3 * 1000;
#else
	sleepTime = ((DWORD) _wtoi(param)) * 1000;
	wprintf(L"Start after %d ms\n", sleepTime);

	ret = TRUE;
#endif

	if (ret) {
		Sleep(sleepTime);

		doWork();
	}

	/* resource deallocation */

	if (param != NULL) {
		free(param);
	}

	return 0;
}

