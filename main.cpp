#include "Net.h"
#include "Jackal.h"
#include "LuaManager.h"
#include <iostream>

int main(void) {
    LuaManager lua;
    int rv = 0;

    try {
        lua.LoadFile("logger.lua");
        lua.Execute();
        lua.LoadFile("main.lua");
        lua.Execute();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    lua_State *L = lua.GetState();

    lua_getglobal(L, "bot");

    if (!lua_istable(L, 1)) {
        std::cerr << "Error loading bot info from lua file." << std::endl;
        return 1;
    }

    lua_getfield(L, 1, "name");
    lua_getfield(L, 1, "password");
    lua_getfield(L, 1, "host");
    lua_getfield(L, 1, "port");

    std::string name(lua_tostring(L, 2));
    std::string password(lua_tostring(L, 3));
    std::string host(lua_tostring(L, 4));
    u16 port = (u16)lua_tonumber(L, 5);

    lua_pop(L, 5);

    Jackal bot(host, port);

    bot.RegisterHandler(&lua);

    try {
        rv = bot.Run(name, password);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cin.get();
    }

    return rv;
}
