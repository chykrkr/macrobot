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

enum class MouseButton {
	UNDEFINED,
	LEFT,
	RIGHT,
};

enum class MouseAction {
	UNDEFINED,
	UP,
	DOWN,
	CLICK,
};

enum class KeyAction {
	UNDEFINED,
	UP,
	DOWN,
	STROKE,
};

static map<string, DWORD> keymap = {
	{ "VK_RETURN",	VK_RETURN },
	{ "VK_UP",		VK_UP },
	{ "VK_DOWN",	VK_DOWN },
	{ "VK_LEFT",	VK_LEFT },
	{ "VK_RIGHT",	VK_RIGHT },
};

static map<char, DWORD> charToVk = {
	{ '0',	0x30 },
	{ '1',	0x31 },
	{ '2',	0x32 },
	{ '3',	0x33 },
	{ '4',	0x34 },
	{ '5',	0x35 },
	{ '6',	0x36 },
	{ '7',	0x37 },
	{ '8',	0x38 },
	{ '9',	0x39 },

	{ 'a',	0x41 },
	{ 'b',	0x42 },
	{ 'c',	0x43 },
	{ 'd',	0x44 },
	{ 'e',	0x45 },
	{ 'f',	0x46 },
	{ 'g',	0x47 },
	{ 'h',	0x48 },
	{ 'i',	0x49 },
	{ 'j',	0x4A },
	{ 'k',	0x4B },
	{ 'l',	0x4C },
	{ 'm',	0x4D },
	{ 'n',	0x4E },
	{ 'o',	0x4F },
	{ 'p',	0x50 },
	{ 'q',	0x51 },
	{ 'r',	0x52 },
	{ 's',	0x53 },
	{ 't',	0x54 },
	{ 'u',	0x55 },
	{ 'v',	0x56 },
	{ 'w',	0x57 },
	{ 'x',	0x58 },
	{ 'y',	0x59 },
	{ 'z',	0x5A },
};

static void mouseEvent(DWORD dwFlags)
{
	/* get number of arguments */
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = dwFlags;
	SendInput(1, &ip, sizeof(INPUT));
}

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



static int mouse(lua_State * L)
{
	MouseButton ebutton = MouseButton::UNDEFINED;
	MouseAction eaction = MouseAction::UNDEFINED;
	DWORD dwFlags = 0x0000;

	const char * button, * action;
	int n = lua_gettop(L);

	if (n < 2)
		return 0;

	/* Mouse BUTTON */
	/* Mouse UP/DOWN/CLICK*/

	button = lua_tostring(L, 1);
	action = lua_tostring(L, 2);

	if (_strnicmp(button, "LEFT", strlen("LEFT")) == 0) {
		ebutton = MouseButton::LEFT;
	}
	else if (_strnicmp(button, "RIGHT", strlen("RIGHT")) == 0) {
		ebutton = MouseButton::RIGHT;
	}

	if (_strnicmp(action, "UP", strlen("UP")) == 0) {
		eaction = MouseAction::UP;
	}
	else if (_strnicmp(action, "DOWN", strlen("DOWN")) == 0) {
		eaction = MouseAction::DOWN;
	}
	else if (_strnicmp(action, "CLICK", strlen("CLICK")) == 0) {
		eaction = MouseAction::CLICK;
	}

	/* DOWN */
	dwFlags = 0x0000;

	if (ebutton == MouseButton::LEFT && (eaction == MouseAction::DOWN || eaction == MouseAction::CLICK)) {
		dwFlags = MOUSEEVENTF_LEFTDOWN;
	}
	else if (ebutton == MouseButton::RIGHT && (eaction == MouseAction::DOWN || eaction == MouseAction::CLICK)) {
		dwFlags = MOUSEEVENTF_RIGHTDOWN;
	}

	if (dwFlags != 0x0000)
		mouseEvent(dwFlags | MOUSEEVENTF_ABSOLUTE);

	/* UP */
	dwFlags = 0x0000;

	if (ebutton == MouseButton::LEFT && (eaction == MouseAction::UP || eaction == MouseAction::CLICK)) {
		dwFlags = MOUSEEVENTF_LEFTUP;
	}
	else if (ebutton == MouseButton::RIGHT && (eaction == MouseAction::UP || eaction == MouseAction::CLICK)) {
		dwFlags = MOUSEEVENTF_RIGHTUP;
	}

	if (dwFlags != 0x0000)
		mouseEvent(dwFlags | MOUSEEVENTF_ABSOLUTE);

	return 0;
}

static int mouse_move(lua_State *L)
{
	int n = lua_gettop(L);
	LONG x, y;

	if (n < 2)
		return 0;

	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);

	/* get number of arguments */
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	ip.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
	ip.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
	SendInput(1, &ip, sizeof(INPUT));

	return 0;
}

static int mouse_move_rel(lua_State *L)
{
	int n = lua_gettop(L);
	LONG x, y;

	if (n < 2)
		return 0;

	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);

	/* get number of arguments */
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = MOUSEEVENTF_MOVE;
	ip.mi.dx = x;
	ip.mi.dy = y;
	SendInput(1, &ip, sizeof(INPUT));

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

static int keyType(lua_State * L)
{
	DWORD vk = 0x00;
	INPUT ip;
	const char * key_name;
	char c;
	map<char, DWORD>::iterator it;
	int ret;

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 1)
		return 0;

	key_name = lua_tostring(L, 1);

	if (key_name == NULL)
		return 0;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	for (int i = 0; key_name[i] != '\x0'; i++) {
		c = key_name[i];

		it = charToVk.find(c);

		if (it == charToVk.end())
			continue;

		vk = it->second;

		//press
		ip.ki.wVk = vk;
		ip.ki.dwFlags = 0;
		SendInput(1, &ip, sizeof(INPUT));

		//release
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}

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

static int movecursor(lua_State *L)
{
	POINT pt;

	int n = lua_gettop(L);

	if (n < 2)
		return 0;

	GetCursorPos(&pt);

	pt.x += lua_tointeger(L, 1);
	pt.y += lua_tointeger(L, 2);

	SetCursorPos(pt.x, pt.y);

	return 0;
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

int docmd(lua_State *L)
{
	const char * cmd;

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 1)
		return 0;

	cmd = lua_tostring(L, 1);

	if (cmd == NULL)
		return 0;

	system(cmd);

	return 0;
}

void MacroBot::regFuncs()
{
	lua_register(L, "left_click", mouseLeftClick);
	lua_register(L, "right_click", mouseRightClick);
	lua_register(L, "mouse", mouse);
	lua_register(L, "mouse_move", mouse_move);
	lua_register(L, "mouse_move_rel", mouse_move_rel);
	lua_register(L, "key", keyStroke);
	lua_register(L, "type", keyType);
	lua_register(L, "sleep", sleep);
	lua_register(L, "setcursor", setcursor);
	lua_register(L, "readcursor", readcursor);
	lua_register(L, "movecursor", movecursor);
	lua_register(L, "docmd", docmd);

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

void MacroBot::doString(char * string)
{
	luaL_dostring(L, string);
}