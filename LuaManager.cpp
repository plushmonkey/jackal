#include "LuaManager.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include "LuaConnection.h"

std::map<std::string, std::vector<int>> references;

LuaManager::LuaManager() {
    m_State = luaL_newstate();

    luaL_openlibs(m_State);

    lua_register(m_State, "RegisterEvent", RegisterEvent);
    lua_register(m_State, "PrintStack", PrintStack);
    lua_register(m_State, "Shutdown", Shutdown);

    RegisterGlobals();
    LuaConnection::Register(m_State);
}

LuaManager::~LuaManager() {
    lua_close(m_State);
}

void LuaManager::LoadFile(const std::string &filename) {
    int rv = luaL_loadfile(m_State, filename.c_str());

    if (rv) {
        std::stringstream ss;
        ss << "Error loading " << filename << std::endl;
        ss << lua_tostring(m_State, -1) << std::endl;
        throw std::runtime_error(ss.str());
    }
}

void LuaManager::Execute() {
    int rv = lua_pcall(m_State, 0, LUA_MULTRET, 0);

    if (rv) {
        std::stringstream ss;
        ss << "Error executing." << std::endl;
        ss << lua_tostring(m_State, -1) << std::endl;
        throw std::runtime_error(ss.str());
    }
}

int LuaManager::RegisterEvent(lua_State *lua) {
    int argc = lua_gettop(lua);

    if (argc == 2) {
        const char *str = lua_tostring(lua, 1);
        int ref = luaL_ref(lua, LUA_REGISTRYINDEX);

        references[str].push_back(ref);
    } else {
        std::cerr << "Error with RegisterEvent arguments" << std::endl;
    }

    return 0;
}

int LuaManager::Shutdown(lua_State *lua) {
    exit(0);
}

int LuaManager::PrintStack(lua_State *lua, int neg) {
    for (int i = -1; i > -neg; i--) {
        int t = lua_type(lua, i);
        switch (t) {
            case LUA_TSTRING:
                std::cout << lua_tostring(lua, i) << std::endl;
                break;
            case LUA_TBOOLEAN:
                std::cout << (lua_toboolean(lua, i) ? "true" : "false") << std::endl;
                break;
            case LUA_TNUMBER:
                std::cout << lua_tonumber(lua, i) << std::endl;
                break;
            default:
                std::cout << lua_typename(lua, i) << std::endl;
                break;
        }
    }
    return 0;
}

int LuaManager::PrintStack(lua_State *lua) {
    int top = lua_gettop(lua);

    for (int i = 1; i <= top; i++) {
        int t = lua_type(lua, i);
        switch (t) {
            case LUA_TSTRING:
                std::cout << lua_tostring(lua, i) << std::endl;
                break;
            case LUA_TBOOLEAN:
                std::cout << (lua_toboolean(lua, i) ? "true" : "false") << std::endl;
                break;
            case LUA_TNUMBER:
                std::cout << lua_tonumber(lua, i) << std::endl;
                break;
            default:
                std::cout << lua_typename(lua, i) << std::endl;
                break;
        }
    }

    return 0;
}

void LuaManager::RegisterGlobals() {
    const char* globals[] = {"ChatArena", "ChatCommand", "ChatPublic", 
                             "ChatPublicMacro", "ChatPrivate", "ChatFrequency",
                             "ChatChannel", "ChatModerator", "ChatSysop",
                             "ChatSquad", "ChatPrivateCommand", "ChatUnknown"};
    for (int i = 0; i <= ChatEvent::ChatUnknown; i++) {
        lua_pushinteger(m_State, i);
        lua_setglobal(m_State, globals[i]);
    }
}

void LuaManager::OnEvent(LoginEvent *event) {
    for (auto it = references["Login"].begin(); it != references["Login"].end(); it++) {
        lua_rawgeti(m_State, LUA_REGISTRYINDEX, *it);

        lua_pushvalue(m_State, 1);

        lua_newtable(m_State);
        int top = lua_gettop(m_State);

        lua_pushstring(m_State, "name");
        lua_pushstring(m_State, event->GetName().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "success");
        lua_pushboolean(m_State, event->GetSuccess());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "message");
        lua_pushstring(m_State, event->GetMessage().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "conn");
        LuaConnection conn(m_State, event->GetConnection());
        conn.ToTable();
        lua_settable(m_State, top);

        if (lua_pcall(m_State, 1, 0, 0) != 0) {
            std::cerr << "Error with login callback" << std::endl;
            std::cerr << lua_tostring(m_State, -1) << std::endl;
        }

        *it = luaL_ref(m_State, LUA_REGISTRYINDEX);
    }
}

void LuaManager::OnEvent(ChatEvent *event) {
    for (auto it = references["Chat"].begin(); it != references["Chat"].end(); it++) {
        lua_rawgeti(m_State, LUA_REGISTRYINDEX, *it);

        lua_pushvalue(m_State, 1);

        lua_newtable(m_State);
        int top = lua_gettop(m_State);

        lua_pushstring(m_State, "player");
        lua_pushstring(m_State, event->GetPlayer().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "target");
        lua_pushstring(m_State, event->GetTarget().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "message");
        lua_pushstring(m_State, event->GetMessage().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "freq");
        lua_pushinteger(m_State, event->GetFrequency());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "channel");
        lua_pushinteger(m_State, event->GetChannel());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "squad");
        lua_pushstring(m_State, event->GetSquad().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "inbound");
        lua_pushboolean(m_State, event->GetInbound());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "type");
        lua_pushinteger(m_State, event->GetType());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "conn");
        LuaConnection conn(m_State, event->GetConnection());
        conn.ToTable();
        lua_settable(m_State, top);

        if (lua_pcall(m_State, 1, 0, 0) != 0) {
            std::cerr << "Error with chat callback" << std::endl;
            std::cerr << lua_tostring(m_State, -1) << std::endl;
        }
        
        *it = luaL_ref(m_State, LUA_REGISTRYINDEX);
    }
}

void LuaManager::OnEvent(EnterEvent *event) {
    for (auto it = references["Enter"].begin(); it != references["Enter"].end(); it++) {
        lua_rawgeti(m_State, LUA_REGISTRYINDEX, *it);

        lua_pushvalue(m_State, 1);

        lua_newtable(m_State);
        int top = lua_gettop(m_State);

        lua_pushstring(m_State, "player");
        lua_pushstring(m_State, event->GetPlayer().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "ship");
        lua_pushinteger(m_State, event->GetShip());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "freq");
        lua_pushinteger(m_State, event->GetFrequency());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "conn");
        LuaConnection conn(m_State, event->GetConnection());
        conn.ToTable();
        lua_settable(m_State, top);

        if (lua_pcall(m_State, 1, 0, 0) != 0) {
            std::cerr << "Error with login callback" << std::endl;
            std::cerr << lua_tostring(m_State, -1) << std::endl;
        }

        *it = luaL_ref(m_State, LUA_REGISTRYINDEX);
    }
}

void LuaManager::OnEvent(LeaveEvent *event) {
    for (auto it = references["Leave"].begin(); it != references["Leave"].end(); it++) {
        lua_rawgeti(m_State, LUA_REGISTRYINDEX, *it);

        lua_pushvalue(m_State, 1);

        lua_newtable(m_State);
        int top = lua_gettop(m_State);

        lua_pushstring(m_State, "player");
        lua_pushstring(m_State, event->GetPlayer().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "conn");
        LuaConnection conn(m_State, event->GetConnection());
        conn.ToTable();
        lua_settable(m_State, top);
        
        if (lua_pcall(m_State, 1, 0, 0) != 0) {
            std::cerr << "Error with login callback" << std::endl;
            std::cerr << lua_tostring(m_State, -1) << std::endl;
        }

        *it = luaL_ref(m_State, LUA_REGISTRYINDEX);
    }
}

void LuaManager::OnEvent(KillEvent *event) {
    for (auto it = references["Kill"].begin(); it != references["Kill"].end(); it++) {
        lua_rawgeti(m_State, LUA_REGISTRYINDEX, *it);

        lua_pushvalue(m_State, 1);

        lua_newtable(m_State);
        int top = lua_gettop(m_State);

        lua_pushstring(m_State, "killer");
        lua_pushstring(m_State, event->GetKiller().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "killed");
        lua_pushstring(m_State, event->GetKilled().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "bounty");
        lua_pushinteger(m_State, event->GetBounty());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "flags");
        lua_pushinteger(m_State, event->GetFlags());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "conn");
        LuaConnection conn(m_State, event->GetConnection());
        conn.ToTable();
        lua_settable(m_State, top);

        if (lua_pcall(m_State, 1, 0, 0) != 0) {
            std::cerr << "Error with login callback" << std::endl;
            std::cerr << lua_tostring(m_State, -1) << std::endl;
        }

        *it = luaL_ref(m_State, LUA_REGISTRYINDEX);
    }
}

void LuaManager::OnEvent(ShipFreqChangeEvent *event) {
    for (auto it = references["ShipFreqChange"].begin(); it != references["ShipFreqChange"].end(); it++) {
        lua_rawgeti(m_State, LUA_REGISTRYINDEX, *it);

        lua_pushvalue(m_State, 1);

        lua_newtable(m_State);
        int top = lua_gettop(m_State);

        lua_pushstring(m_State, "player");
        lua_pushstring(m_State, event->GetPlayer().c_str());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "ship");
        lua_pushinteger(m_State, event->GetShip());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "freq");
        lua_pushinteger(m_State, event->GetFrequency());
        lua_settable(m_State, top);

        lua_pushstring(m_State, "conn");
        LuaConnection conn(m_State, event->GetConnection());
        conn.ToTable();
        lua_settable(m_State, top);

        if (lua_pcall(m_State, 1, 0, 0) != 0) {
            std::cerr << "Error with login callback" << std::endl;
            std::cerr << lua_tostring(m_State, -1) << std::endl;
        }

        *it = luaL_ref(m_State, LUA_REGISTRYINDEX);
    }
}
