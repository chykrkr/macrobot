#pragma once
#include "stdafx.h"
#include "windows.h"


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

typedef struct tagCharProperty {
	WORD   type;
	int		shift;
} CharProperty;

class MacroBot
{
private:
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

	static const map<string, WORD> keymap;
	static const map<string, WORD> toggleKey;
	static const map<char, CharProperty>  charToVK;

	lua_State * L = NULL;

	void regFuncs();

	/*
	   Below methods should be static
	   to be registered with lua_register C function
	*/
	static int mouseLeftClick(lua_State * L);
	static int mouseRightClick(lua_State * L);
	static int mouse(lua_State * L);
	static int mouse_move(lua_State *L);
	static int mouse_move_rel(lua_State *L);
	static int keyStroke(lua_State * L);
	static int typeString(lua_State * L);
	static int sleep(lua_State * L);
	static int setcursor(lua_State *L);
	static int readcursor(lua_State *L);
	static int movecursor(lua_State *L);
	static int docmd(lua_State *L);
	static int keyon(lua_State *L);
	static int readkey(lua_State *L);
	static int get_foreground_wnd(lua_State *L);
	static int set_foreground_wnd(lua_State *L);

	/*
	   Below methods should be static
	   because they are called by above static methods
	*/
	static void mouseEvent(DWORD dwFlags);
	static int getVkFromStr(const char * key_name, DWORD & vk);
	static void typeVk(INPUT & ip, int shift, WORD vk);

public:
	MacroBot();
	~MacroBot();

	void doFile(char * script_file);
	void doString(char * string);

};

