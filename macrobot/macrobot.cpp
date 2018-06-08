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

const map<string, WORD> MacroBot::keymap = {
	{ "VK_RETURN",	VK_RETURN },
	{ "VK_UP",	VK_UP },
	{ "VK_DOWN",	VK_DOWN },
	{ "VK_LEFT",	VK_LEFT },
	{ "VK_RIGHT",	VK_RIGHT },
	{ "VK_SHIFT",	VK_SHIFT },
	{ "VK_LSHIFT",	VK_LSHIFT },
	{ "VK_CAPITAL",	VK_CAPITAL },
	{ "VK_NUMLOCK", VK_NUMLOCK },
	{ "VK_SCROLL",	VK_SCROLL },
	{ "VK_BACK",	VK_BACK },
	{ "VK_TAB",	VK_TAB },
	{ "VK_CLEAR",	VK_CLEAR },
	{ "VK_ESCAPE",	VK_ESCAPE },
	{ "VK_DELETE",	VK_DELETE },
};

const map<string, WORD> MacroBot::toggleKey = {
	{ "VK_CAPITAL",	VK_CAPITAL },
	{ "VK_NUMLOCK", VK_NUMLOCK },
	{ "VK_SCROLL",	VK_SCROLL },
};

const map<char, CharProperty> MacroBot::charToVK = {
	/* alpha set */
	{ 'a',	{ 0x41, 0 } },
	{ 'b',	{ 0x42, 0 } },
	{ 'c',	{ 0x43, 0 } },
	{ 'd',	{ 0x44, 0 } },
	{ 'e',	{ 0x45, 0 } },
	{ 'f',	{ 0x46, 0 } },
	{ 'g',	{ 0x47, 0 } },
	{ 'h',	{ 0x48, 0 } },
	{ 'i',	{ 0x49, 0 } },
	{ 'j',	{ 0x4A, 0 } },
	{ 'k',	{ 0x4B, 0 } },
	{ 'l',	{ 0x4C, 0 } },
	{ 'm',	{ 0x4D, 0 } },
	{ 'n',	{ 0x4E, 0 } },
	{ 'o',	{ 0x4F, 0 } },
	{ 'p',	{ 0x50, 0 } },
	{ 'q',	{ 0x51, 0 } },
	{ 'r',	{ 0x52, 0 } },
	{ 's',	{ 0x53, 0 } },
	{ 't',	{ 0x54, 0 } },
	{ 'u',	{ 0x55, 0 } },
	{ 'v',	{ 0x56, 0 } },
	{ 'w',	{ 0x57, 0 } },
	{ 'x',	{ 0x58, 0 } },
	{ 'y',	{ 0x59, 0 } },
	{ 'z',	{ 0x5A, 0 } },

	/* alpha capital set*/
	{ 'A',	{ 0x41, 1 } },
	{ 'B',	{ 0x42, 1 } },
	{ 'C',	{ 0x43, 1 } },
	{ 'D',	{ 0x44, 1 } },
	{ 'E',	{ 0x45, 1 } },
	{ 'F',	{ 0x46, 1 } },
	{ 'G',	{ 0x47, 1 } },
	{ 'H',	{ 0x48, 1 } },
	{ 'I',	{ 0x49, 1 } },
	{ 'J',	{ 0x4A, 1 } },
	{ 'K',	{ 0x4B, 1 } },
	{ 'L',	{ 0x4C, 1 } },
	{ 'M',	{ 0x4D, 1 } },
	{ 'N',	{ 0x4E, 1 } },
	{ 'O',	{ 0x4F, 1 } },
	{ 'P',	{ 0x50, 1 } },
	{ 'Q',	{ 0x51, 1 } },
	{ 'R',	{ 0x52, 1 } },
	{ 'S',	{ 0x53, 1 } },
	{ 'T',	{ 0x54, 1 } },
	{ 'U',	{ 0x55, 1 } },
	{ 'V',	{ 0x56, 1 } },
	{ 'W',	{ 0x57, 1 } },
	{ 'X',	{ 0x58, 1 } },
	{ 'Y',	{ 0x59, 1 } },
	{ 'Z',	{ 0x5A, 1 } },

	/* non-alpha set */
	{ '\t', { VK_TAB, 0 } },
	{ ' ',	{ VK_SPACE, 0 } },
	{ '0',	{ 0x30, 0 } },
	{ '1',	{ 0x31, 0 } },
	{ '2',	{ 0x32, 0 } },
	{ '3',	{ 0x33, 0 } },
	{ '4',	{ 0x34, 0 } },
	{ '5',	{ 0x35, 0 } },
	{ '6',	{ 0x36, 0 } },
	{ '7',	{ 0x37, 0 } },
	{ '8',	{ 0x38, 0 } },
	{ '9',	{ 0x39, 0 } },
	{ '-',	{ VK_OEM_MINUS, 0 } },
	{ '=',	{ VK_OEM_PLUS, 0} },
	{ '\\',	{ VK_OEM_5, 0} },
	{ '[',	{ VK_OEM_4, 0} },
	{ ']',	{ VK_OEM_6, 0} },
	{ ';',	{ VK_OEM_1, 0} },
	{ '\'',	{ VK_OEM_7, 0} },
	{ ',',	{ VK_OEM_COMMA, 0} },
	{ '.',	{ VK_OEM_PERIOD, 0} },
	{ '/',	{ VK_OEM_2, 0} },
	{ '`',	{ VK_OEM_3, 0} },

	/* non-alpha set with shift */
	{ ')',	{ 0x30, 1} },
	{ '!',	{ 0x31, 1} },
	{ '@',	{ 0x32, 1} },
	{ '#',	{ 0x33, 1} },
	{ '$',	{ 0x34, 1} },
	{ '%',	{ 0x35, 1} },
	{ '^',	{ 0x36, 1} },
	{ '&',	{ 0x37, 1} },
	{ '*',	{ 0x38, 1} },
	{ '(',	{ 0x39, 1} },
	{ '_',	{ VK_OEM_MINUS, 1} },
	{ '+',	{ VK_OEM_PLUS, 1} },
	{ '|',	{ VK_OEM_5, 1} },
	{ '{',	{ VK_OEM_4, 1} },
	{ '}',	{ VK_OEM_6, 1} },
	{ ':',	{ VK_OEM_1, 1} },
	{ '"',	{ VK_OEM_7, 1} },
	{ '<',	{ VK_OEM_COMMA, 1} },
	{ '>',	{ VK_OEM_PERIOD, 1} },
	{ '?',	{ VK_OEM_2, 1} },
	{ '~',	{ VK_OEM_3, 1} },
};

void MacroBot::mouseEvent(DWORD dwFlags)
{
	/* get number of arguments */
	INPUT ip;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = dwFlags;
	SendInput(1, &ip, sizeof(INPUT));
}

int MacroBot::mouseLeftClick(lua_State * L)
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

int MacroBot::mouseRightClick(lua_State * L)
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



int MacroBot::mouse(lua_State * L)
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

int MacroBot::mouse_move(lua_State *L)
{
	int n = lua_gettop(L);
	LONG x, y;

	if (n < 2)
		return 0;

	x = (LONG) lua_tonumber(L, 1);
	y = (LONG) lua_tonumber(L, 2);

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

int MacroBot::mouse_move_rel(lua_State *L)
{
	int n = lua_gettop(L);
	LONG x, y;

	if (n < 2)
		return 0;

	x = (LONG) lua_tonumber(L, 1);
	y = (LONG) lua_tonumber(L, 2);

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

int MacroBot::getVkFromStr(const char * key_name, DWORD & vk)
{
	int ret;
	map<string, WORD>::const_iterator it;

	if (key_name == NULL)
		return -1;

	it = keymap.find(key_name);

	if (it == keymap.end()) {
		ret = sscanf_s(key_name, "%2x", &vk);
	}
	else {
		vk = it->second;
		ret = 1;
	}

	return ret;
}

int MacroBot::keyStroke(lua_State * L)
{
	DWORD vk = 0x00;
	INPUT ip;
	const char * key_name;
	int ret;
	KeyAction action = KeyAction::STROKE;
	const char * action_name;

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 1)
		return 0;

	key_name = lua_tostring(L, 1);

	if (key_name == NULL)
		return 0;

	ret = getVkFromStr(key_name, vk);

	if (ret < 1) {
		return 0;
	}

	if (n >= 2)
		action_name = lua_tostring(L, 2);
	else
		action_name = NULL;

	if (action_name == NULL) {
		action = KeyAction::STROKE;
	}
	else if (_strnicmp(action_name, "DOWN", strlen("DOWN")) == 0) {
		action = KeyAction::DOWN;
	}
	else if (_strnicmp(action_name, "UP", strlen("UP")) == 0) {
		action = KeyAction::UP;
	}
	else {
		action = KeyAction::UNDEFINED;
	}

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = (WORD) vk;

	//press
	if (action == KeyAction::DOWN || action == KeyAction::STROKE) {
		ip.ki.dwFlags = 0;
		SendInput(1, &ip, sizeof(INPUT));
	}

	//release
	if (action == KeyAction::UP || action == KeyAction::STROKE) {
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}

	return 0;
}

void MacroBot::typeVk(INPUT & ip, int shift, WORD vk)
{
	//shift-down
	if (shift) {
		ip.ki.wVk = VK_LSHIFT;
		ip.ki.dwFlags = 0;
		SendInput(1, &ip, sizeof(INPUT));
	}

	//press
	ip.ki.wVk = vk;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	//release
	ip.ki.wVk = vk;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	//shift-up
	if (shift) {
		ip.ki.wVk = VK_LSHIFT;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}
}

int MacroBot::typeString(lua_State * L)
{
	DWORD vk = 0x00;
	INPUT ip;
	const char * key_name;
	char c;
	map<char, CharProperty>::const_iterator it;
	int shift = 0;
	CharProperty prop;

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

		//search alphaset
		it = charToVK.find(c);

		if (it == charToVK.end())
			continue;

		prop = it->second;

		typeVk(ip, prop.shift, prop.type);
	}

	return 0;
}

int MacroBot::sleep(lua_State * L)
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

int MacroBot::setcursor(lua_State *L)
{
	int n = lua_gettop(L);
	int x, y;

	if (n < 2)
		return 0;

	x = (int) lua_tonumber(L, 1);
	y = (int) lua_tonumber(L, 2);

	SetCursorPos(x, y);

	return 0;
}

int MacroBot::readcursor(lua_State *L)
{
	POINT pt;

	GetCursorPos(&pt);

	lua_pushnumber(L, pt.x);
	lua_pushnumber(L, pt.y);

	return 2;
}

int MacroBot::movecursor(lua_State *L)
{
	POINT pt;

	int n = lua_gettop(L);

	if (n < 2)
		return 0;

	GetCursorPos(&pt);

	pt.x += (LONG) lua_tointeger(L, 1);
	pt.y += (LONG) lua_tointeger(L, 2);

	SetCursorPos(pt.x, pt.y);

	return 0;
}

int MacroBot::docmd(lua_State *L)
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

int MacroBot::keyon(lua_State *L)
{
	const char * key_name;
	map<string, WORD>::const_iterator it;
	int onoff;
	int toggle;
	WORD vk;
	SHORT state;
	INPUT ip;

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 2)
		return 0;

	key_name = lua_tostring(L, 1);
	/* insure 0 or 1 */
	onoff = !!lua_toboolean(L, 2);

	if (key_name == NULL)
		return 0;

	it = toggleKey.find(key_name);

	if (it == toggleKey.end())
		return 0;

	vk =it->second;

	state = GetKeyState(vk);

	toggle = !!(state & 0x00FF);

	if (onoff == toggle)
		return 0;

	ZeroMemory(&ip, sizeof(INPUT));

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	typeVk(ip, 0, vk);

	return 0;
}

int MacroBot::readkey(lua_State *L)
{
	SHORT state;
	DWORD vk;
	int ret;
	const char * key_name;

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 1)
		return 0;

	key_name = lua_tostring(L, 1);

	ret = getVkFromStr(key_name, vk);

	if (ret < 1)
		return 0;


	state = GetKeyState(vk);

	/* up/down */
	lua_pushinteger(L, (state & 0xFF00) >> 8);

	/* toggle */
	lua_pushinteger(L, state & 0x00FF);

	return 2;
}

int MacroBot::get_foreground_wnd(lua_State *L)
{
	HWND hwnd;

	hwnd = GetForegroundWindow();

	lua_pushinteger(L, (lua_Integer) hwnd);

	return 1;
}

int MacroBot::set_foreground_wnd(lua_State *L)
{
	HWND hwnd;
	BYTE keyState[255] = { 0 };

	/* get number of arguments */
	int n = lua_gettop(L);

	if (n < 1)
		return 0;

	hwnd = (HWND) lua_tointeger(L, 1);

	if (!IsWindow(hwnd))
		return 0;

	//to unlock SetForegroundWindow we need to imitate Alt pressing
	if (GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		}
	}

	SetForegroundWindow(hwnd);

	if (GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}

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
	lua_register(L, "type", typeString);
	lua_register(L, "sleep", sleep);
	lua_register(L, "setcursor", setcursor);
	lua_register(L, "readcursor", readcursor);
	lua_register(L, "movecursor", movecursor);
	lua_register(L, "docmd", docmd);
	lua_register(L, "keyon", keyon);
	lua_register(L, "readkey", readkey);
	lua_register(L, "get_foreground_wnd", get_foreground_wnd);
	lua_register(L, "set_foreground_wnd", set_foreground_wnd);
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
