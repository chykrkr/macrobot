// macrobot.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MacroBot.h"

#ifdef __cplusplus
#include "lua.hpp"
#else
#include "lua.h"
#include "luaconf.h"
#include "lualib.h"
#include "lauxlib.h"
#endif

#include <map>
#include <string>

using std::map;
using std::string;

static map<string, DWORD> keymap = {
	{ "VK_RETURN",	VK_RETURN },
	{ "VK_UP",		VK_UP },
	{ "VK_DOWN",	VK_DOWN },
	{ "VK_LEFT",	VK_LEFT },
	{ "VK_RIGHT",	VK_RIGHT },

};

static int mouseLeftClick(lua_State * L)
{
	/* get number of arguments */
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &ip, sizeof(INPUT));

	ip.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &ip, sizeof(INPUT));

	/* return the number of results */
	return 0;
}

static int mouseRightClick(lua_State * L)
{
	/* get number of arguments */
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &ip, sizeof(INPUT));

	ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &ip, sizeof(INPUT));

	/* return the number of results */
	return 0;
}

static int keyStroke(lua_State * L)
{
	DWORD vk = 0x00;
	INPUT ip;
	const char * key_name;
	map<string, DWORD>::iterator it;
	int ret;

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 1)
		return 0;

	key_name = lua_tostring(L, 1);

	if (key_name == NULL)
		return 0;

	it = keymap.find(key_name);

	if (it == keymap.end()) {
		ret = sscanf_s(key_name, "%2x", &vk);
	}
	else {
		vk = it->second;
		ret = 1;
	}

	if (ret < 1) {
		return 0;
	}
	
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

static int sleep(lua_State * L)
{
	DWORD t;

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 1)
		return 0;

	t = (DWORD)lua_tointeger(L, 1);

	Sleep(t);

	return 0;
}

static int setcursor(lua_State *L)
{
	int n = lua_gettop(L);
	int x, y;

	if (n < 2)
		return 0;

	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);

	SetCursorPos(x, y);

	return 0;
}

static int readcursor(lua_State *L)
{
	POINT pt;

	GetCursorPos(&pt);

	lua_pushnumber(L, pt.x);
	lua_pushnumber(L, pt.y);

	return 2;
}

int average(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* loop through each argument */
	for (i = 1; i <= n; i++)
	{
		/* total the arguments */
		sum += lua_tonumber(L, i);
	}

	/* push the average */
	lua_pushnumber(L, sum / n);

	/* push the sum */
	lua_pushnumber(L, sum);

	/* return the number of results */
	return 2;
}

void MacroBot::regFuncs()
{
	lua_register(L, "left_click", mouseLeftClick);
	lua_register(L, "right_click", mouseRightClick);
	lua_register(L, "key", keyStroke);
	lua_register(L, "sleep", sleep);
	lua_register(L, "setcursor", setcursor);
	lua_register(L, "readcursor", readcursor);
	
	//test
	lua_register(L, "average", average);
}

MacroBot::MacroBot()
{
	L = luaL_newstate();

	luaL_openlibs(L);

	regFuncs();
}


MacroBot::~MacroBot()
{
	lua_close(L);
}

void MacroBot::doFile(char * script_file)
{
	luaL_dofile(L, script_file);
}
