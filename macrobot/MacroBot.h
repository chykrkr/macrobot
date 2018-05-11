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

class MacroBot
{
private:
	lua_State * L = NULL;

	void regFuncs();

public:
	MacroBot();
	~MacroBot();

	void doFile(char * script_file);
};

