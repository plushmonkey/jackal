#include "LuaConnection.h"
#include <iostream>
#include "LuaManager.h"

LuaConnection::LuaConnection(lua_State *lua, CNConnection *conn)
: m_State(lua), m_Connection(conn) {

}

void LuaConnection::Register(lua_State *lua) {
    // Register socket status
    lua_pushinteger(lua, 0);
    lua_setglobal(lua, "None");

    lua_pushinteger(lua, 1);
    lua_setglobal(lua, "Connected");

    lua_pushinteger(lua, 2);
    lua_setglobal(lua, "Disconnected");

    lua_pushinteger(lua, 3);
    lua_setglobal(lua, "ForcedClose");
}

void LuaConnection::ToTable() {
    int status = m_Connection->GetConnectionStatus();
    
    lua_newtable(m_State);

    int top = lua_gettop(m_State);

    lua_pushstring(m_State, "status");
    lua_pushinteger(m_State, status);
    lua_settable(m_State, top);

    lua_pushstring(m_State, "CNConnection");
    lua_pushinteger(m_State, (int)m_Connection);
    lua_settable(m_State, top);

    lua_pushstring(m_State, "SendRaw");
    lua_pushcfunction(m_State, SendRaw);
    lua_settable(m_State, top);

    lua_pushstring(m_State, "SendChatPublic");
    lua_pushcfunction(m_State, SendChatPublic);
    lua_settable(m_State, top);

    lua_pushstring(m_State, "SendChatPrivate");
    lua_pushcfunction(m_State, SendChatPrivate);
    lua_settable(m_State, top);

    lua_pushstring(m_State, "SendGo");
    lua_pushcfunction(m_State, SendGo);
    lua_settable(m_State, top);
}

int LuaConnection::SendGo(lua_State *lua) {
    const char *arena = lua_tostring(lua, -1);

    if (lua_istable(lua, -2)) {
        lua_getfield(lua, -2, "CNConnection");

        CNConnection *conn = (CNConnection *)(int)lua_tonumber(lua, 3);

        int top = lua_gettop(lua);
        lua_settop(lua, 0);
        conn->SendGo(arena);
        lua_settop(lua, top);
    }

    return 0;
}

int LuaConnection::SendChatPublic(lua_State *lua) {
    const char *data = lua_tostring(lua, -1);

    if (lua_istable(lua, -2)) {
        lua_getfield(lua, -2, "CNConnection");
        
        CNConnection *conn = (CNConnection *)(int)lua_tonumber(lua, 3);

        int top = lua_gettop(lua);
        lua_settop(lua, 0);
        conn->SendChatPublic(data);
        lua_settop(lua, top);
    }

    return 0;
}

int LuaConnection::SendChatPrivate(lua_State *lua) {
    const char *target = lua_tostring(lua, -2);
    const char *mesg = lua_tostring(lua, -1);

    if (lua_istable(lua, -3)) {
        lua_getfield(lua, -3, "CNConnection");

        CNConnection *conn = (CNConnection *)(int)lua_tonumber(lua, 4);

        int top = lua_gettop(lua);
        lua_settop(lua, 0);
        conn->SendChatPrivate(target, mesg);
        lua_settop(lua, top);
    }

    return 0;
}

int LuaConnection::SendRaw(lua_State *lua) {
    const char *data = lua_tostring(lua, -1);

    if (lua_istable(lua, -2)) {
        lua_getfield(lua, -2, "CNConnection");
        CNConnection *conn = (CNConnection *)(int)lua_tonumber(lua, 2);

        int top = lua_gettop(lua);
        lua_settop(lua, 0);
        conn->SendRaw(data);
        lua_settop(lua, top);
    }

    return 0;
}
