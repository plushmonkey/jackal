#ifndef CHAT_EVENT_H_
#define CHAT_EVENT_H_

#include "Event.h"

class ChatEvent : public Event {
    public:
        enum ChatType {
            ChatArena,
            ChatCommand,
            ChatPublic,
            ChatPublicMacro,
            ChatPrivate,
            ChatFrequency,
            ChatChannel,
            ChatModerator,
            ChatSysop,
            ChatSquad,
            ChatPrivateCommand,

            ChatUnknown
        };
    private:
        std::string m_Player;
        std::string m_Target;
        std::string m_Message;

        ChatType m_Type;

        int m_Freq;
        int m_Channel;
        bool m_Inbound;

        std::string m_Squad;
    public:
        ChatEvent(CNConnection *conn) : Event(conn), m_Type(ChatUnknown), m_Inbound(true), m_Channel(0), m_Freq(0), m_Target("") { }
        
        const std::string &GetPlayer() const { return m_Player; }
        const std::string &GetTarget() const { return m_Target; }
        const std::string &GetMessage() const { return m_Message; }
        ChatType GetType() const { return m_Type; }
        int GetFrequency() const { return m_Freq; }
        int GetChannel() const { return m_Channel; }
        const std::string &GetSquad() const { return m_Squad; }
        bool GetInbound() const { return m_Inbound; }

        void SetPlayer(const std::string &player) { m_Player = player; }
        void SetTarget(const std::string &target) { m_Target = target; }
        void SetMessage(const std::string &mesg) { m_Message = mesg; }
        void SetType(ChatType type) { m_Type = type; }
        void SetFrequency(int freq) { m_Freq = freq; }
        void SetChannel(int channel) { m_Channel = channel; }
        void SetSquad(const std::string &squad) { m_Squad = squad; }
        void SetInbound(bool inbound) { m_Inbound = inbound; }

        static ChatType TypeFromString(const std::string &type_str) {
            const char *tinit[] = {"ARENA","CMD", "PUB", "PUBM", "PRIV",
                                   "FREQ", "CHAT", "MOD", "SYSOP", "SQUAD",
                                   "PRIVCMD" };
            std::vector<std::string> types(tinit, tinit + 11);
            
            for (unsigned int i = 0; i < types.size(); i++) {
                if (types.at(i).compare(type_str) == 0)
                    return static_cast<ChatType>(i);
            }

            return ChatUnknown;
        }

        static std::string StringFromType(ChatType type) {
            const char *tinit[] = { "ARENA", "CMD", "PUB", "PUBM", "PRIV",
                                    "FREQ", "CHAT", "MOD", "SYSOP", "SQUAD",
                                    "PRIVCMD" };
            std::vector<std::string> types(tinit, tinit + 11);

            return types.at(type);
        }

        void Dispatch(EventHandlerBase *handler) {
            DDispatch(handler, this);
        }
};

#endif
