#include "GarrysMod/Lua/Interface.h"

#include "lua.hpp"
#include <lauxlib.h>
#include <lualib.h>

using namespace GarrysMod::Lua;

lua_State* L = nullptr;

class LuaL {
public:
    static int setLocal(lua_State* L) {
        int level = luaL_checkinteger(L, 1);
        int idx = luaL_checkinteger(L, 2);
        lua_Debug ar;
        if (lua_getstack(L, level, &ar)) {
            lua_pushvalue(L, 3);
            lua_pushstring(L, lua_setlocal(L, &ar, idx));
            return 1;
        }
        lua_pushnil(L);
        return 1;
    }

    static int isCFunction(lua_State* L) {
        luaL_checktype(L, 1, LUA_TFUNCTION);
        lua_pushvalue(L, 1);
        lua_pushboolean(L, lua_iscfunction(L, 1));
        return 1;
    }

    static int createCClosure(lua_State* L) {
        luaL_checktype(L, 1, LUA_TFUNCTION);
        lua_pushvalue(L, 1);
        lua_pushcclosure(L, CClosure, 1);
        lua_remove(L, -2);
        return 1;
    }

    static int close(lua_State* L) {
        lua_close(L);
        return 0;
    }
private:
    static int CClosure(lua_State* L) {
        int argc = lua_gettop(L);
        lua_pushvalue(L, lua_upvalueindex(1));
        for (int i = 1; i <= argc; i++)
            lua_pushvalue(L, i);
        lua_call(L, argc, LUA_MULTRET);
        return 1;
    }
};

GMOD_MODULE_OPEN() {
    L = LUA->GetState();
    lua_newtable(L);
    lua_pushcfunction(L, LuaL::setLocal);
    lua_setfield(L, -2, "setlocal");
    lua_pushcfunction(L, LuaL::isCFunction);
    lua_setfield(L, -2, "iscfunction");
    lua_pushcfunction(L, LuaL::createCClosure);
    lua_setfield(L, -2, "cclosure");
    lua_pushcfunction(L, LuaL::close);
    lua_setfield(L, -2, "close");
    lua_setglobal(L, "luaL");
    return 0;
}

GMOD_MODULE_CLOSE() {
    return 0;
}
