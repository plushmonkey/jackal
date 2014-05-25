#ifndef LUA_CONNECTION_H_
#define LUA_CONNECTION_H_

#include "LuaManager.h"
#include "CNConnection.h"

class LuaConnection {
    private:
        CNConnection *m_Connection;
        lua_State *m_State;

    public:
        LuaConnection(lua_State *m_State, CNConnection *conn);

        static void Register(lua_State *lua);

        static int SendRaw(lua_State *lua);
        static int SendGo(lua_State *lua);
        static int SendChatPublic(lua_State *lua);
        static int SendChatPrivate(lua_State *lua);

        void ToTable();
};

#endif
