#ifndef LUA_MANAGER_H_
#define LUA_MANAGER_H_

#include <lua.hpp>
#include <string>
#include "LoginEvent.h"
#include "ChatEvent.h"
#include "EnterEvent.h"
#include "KillEvent.h"
#include "ShipFreqChangeEvent.h"

class LuaManager : public EventHandler<LoginEvent>
                 , public EventHandler<ChatEvent>
                 , public EventHandler<EnterEvent> 
                 , public EventHandler<LeaveEvent>
                 , public EventHandler<KillEvent>
                 , public EventHandler<ShipFreqChangeEvent> {
    private:
        lua_State *m_State;

    public:
        LuaManager();
        ~LuaManager();

        lua_State *GetState() { return m_State; }

        void LoadFile(const std::string &filename);
        void Execute();
        void RegisterGlobals();

        static int RegisterEvent(lua_State *L);

        static int Shutdown(lua_State *lua);

        // debugging
        static int PrintStack(lua_State *lua);
        static int PrintStack(lua_State *lua, int neg);

        void OnEvent(LoginEvent *event);
        void OnEvent(ChatEvent *event);
        void OnEvent(EnterEvent *event);
        void OnEvent(LeaveEvent *event);
        void OnEvent(KillEvent *event);
        void OnEvent(ShipFreqChangeEvent *event);
};

#endif
