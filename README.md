# gmcl_luaL
A straightforward module that implements specific quality-of-life functions from the C Lua library into the game. All functions are exported to the global `luaL` table. Inspired by [oink's Lua API](https://oink.industries/gmod_wiki/lua_api/). Please note that while it is unlikely, it is still possible to detect certain functions, such as `luaL.cclosure`. Certain functions may be exploited to perform memory read and write manipulations.

## Functions
```lua
require("luaL"); -- Don't forget about requiring it in the first place lol
```
### `luaL.cclosure(<function: f>): function`
Creates and returns a callable CClosure from the given Lua function.
```lua
local print = print;
_G["print"] = luaL.cclosure(function(...)
    return print("Detoured:", ...);
end);
```

### `luaL.iscfunction(<function: f>): boolean`
Returns whether the given function is a C function.
```lua
print(luaL.iscfunction(print)); -- output: true
print(luaL.iscfunction(function() end)); -- output: false
```

### `luaL.close(): void`
Terminates the game's Lua state, effectively causing the game to crash.
```lua
if (not luaL.iscfunction(debug.getinfo)) then
    return luaL.close();
end
```
