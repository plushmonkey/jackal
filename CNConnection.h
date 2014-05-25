#ifndef CNCONNECTION_H_
#define CNCONNECTION_H_

#include "Common.h"
#include "Net.h"

class EventHandlerBase;
typedef std::vector<EventHandlerBase *> EventHandlers;
#define DISPATCH(handlers, event) { for (auto iter = (handlers).begin(); iter != (handlers).end(); iter++) (event).Dispatch(*iter); }

class CNConnection {
    private:
        jackal::net::TCPSocket m_Socket;
        jackal::net::IPAddress m_Addr;

    protected:
        std::string m_Username;
        std::string m_Password;

        std::string m_ClientVer;

        std::string m_Host;
        u16 m_Port;

        EventHandlers m_Handlers;
    public:
        CNConnection(const std::string &host, u16 port);
        virtual ~CNConnection();

        bool Connect();
        void Disconnect();

        jackal::net::TCPSocket::Status GetConnectionStatus() const;

        size_t SendRaw(const std::string &data);
        size_t ReceiveRaw(std::string &data, size_t amount);

        void SendLogin();
        void SendGo(const std::string &arena);
        void SendChangeFreq(int freq);
        void SendNoop();

        void SendChatPublic(const std::string &mesg);
        void SendChatPrivate(const std::string &target, const std::string &mesg);
        void SendChatFrequency(u32 freq, const std::string &mesg);
        void SendChatChannel(u8 channel, const std::string &mesg);
        void SendChatCommand(const std::string &mesg);
        void SendChatPrivateCommand(const std::string &target, const std::string &mesg);
        void SendChatSquad(const std::string &squad, const std::string &mesg);

        bool RegisterHandler(EventHandlerBase *handler);
};

#endif
