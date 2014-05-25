#include "CNConnection.h"
#include "Net.h"
#include "ChatEvent.h"
#include <sstream>

CNConnection::CNConnection(const std::string &host, u16 port) : m_Host(host), m_Port(port) {
    m_ClientVer = "chatnet";
    m_Username = "";
    m_Password = "";
}

CNConnection::~CNConnection() {
    Disconnect();
}

bool CNConnection::Connect() {
    m_Addr = m_Socket.Connect(m_Host, m_Port);

    if (m_Addr == jackal::net::IPAddress::None)
        return false;

    m_Socket.SetBlocking(false);
    return true;
}

void CNConnection::Disconnect() {
    m_Socket.Disconnect();
}

jackal::net::TCPSocket::Status CNConnection::GetConnectionStatus() const {
    return m_Socket.GetStatus();
}

size_t CNConnection::SendRaw(const std::string &data) {
    return m_Socket.Send(data);
}

size_t CNConnection::ReceiveRaw(std::string &data, size_t amount) {
    return m_Socket.Receive(data, amount);
}

void CNConnection::SendLogin() {
    std::stringstream ss;

    ss << "LOGIN:1;" << m_ClientVer << ":" << m_Username << ":" << m_Password << "\n";
    m_Socket.Send(ss.str());
}

void CNConnection::SendGo(const std::string &arena) {
    m_Socket.Send("GO:" + arena + "\n");
}

void CNConnection::SendChangeFreq(int freq) {
    std::stringstream ss;

    ss << "CHANGEFREQ:" << freq << "\n";

    m_Socket.Send(ss.str());
}

void CNConnection::SendNoop() {
    m_Socket.Send("NOOP\n");
}

void CNConnection::SendChatPublic(const std::string &mesg) {
    std::stringstream ss;

    ss << "SEND:PUB:" << mesg << "\n";

    m_Socket.Send(ss.str());

    ChatEvent event(this);

    event.SetInbound(false);
    event.SetPlayer(m_Username);
    event.SetMessage(mesg);
    event.SetType(ChatEvent::ChatPublic);

    DISPATCH(m_Handlers, event);
}

void CNConnection::SendChatPrivate(const std::string &target, const std::string &mesg) {
    std::stringstream ss;

    ss << "SEND:PRIV:" << target << ":" << mesg << "\n";

    m_Socket.Send(ss.str());

    ChatEvent event(this);

    event.SetInbound(false);
    event.SetPlayer(m_Username);
    event.SetTarget(target);
    event.SetMessage(mesg);
    event.SetType(ChatEvent::ChatPrivate);

    DISPATCH(m_Handlers, event);
}

void CNConnection::SendChatFrequency(u32 freq, const std::string &mesg) {
    std::stringstream ss;

    ss << "SEND:FREQ:" << freq << ":" << mesg << "\n";

    m_Socket.Send(ss.str());

    ChatEvent event(this);

    event.SetInbound(false);
    event.SetPlayer(m_Username);
    event.SetFrequency(freq);
    event.SetMessage(mesg);
    event.SetType(ChatEvent::ChatFrequency);

    DISPATCH(m_Handlers, event);
}

void CNConnection::SendChatChannel(u8 channel, const std::string &mesg) {
    std::stringstream ss;

    ss << "SEND:CHAT:" << channel << ";" << mesg << "\n";

    m_Socket.Send(ss.str());

    ChatEvent event(this);

    event.SetInbound(false);
    event.SetPlayer(m_Username);
    event.SetChannel(channel);
    event.SetMessage(mesg);
    event.SetType(ChatEvent::ChatChannel);

    DISPATCH(m_Handlers, event);
}

void CNConnection::SendChatCommand(const std::string &mesg) {
    std::stringstream ss;

    ss << "SEND:CMD:" << mesg << "\n";

    m_Socket.Send(ss.str());

    ChatEvent event(this);

    event.SetInbound(false);
    event.SetPlayer(m_Username);
    event.SetMessage(mesg);
    event.SetType(ChatEvent::ChatCommand);

    DISPATCH(m_Handlers, event);
}

void CNConnection::SendChatPrivateCommand(const std::string &target, const std::string &mesg) {
    std::stringstream ss;

    ss << "SEND:CMD:" << target << ":" << mesg << "\n";

    m_Socket.Send(ss.str());

    ChatEvent event(this);

    event.SetInbound(false);
    event.SetPlayer(m_Username);
    event.SetTarget(target);
    event.SetMessage(mesg);
    event.SetType(ChatEvent::ChatPrivateCommand);

    DISPATCH(m_Handlers, event);
}

void CNConnection::SendChatSquad(const std::string &squad, const std::string &mesg) {
    std::stringstream ss;

    ss << "SEND:SQUAD:" << squad << ":" << mesg << "\n";

    m_Socket.Send(ss.str());

    ChatEvent event(this);

    event.SetInbound(false);
    event.SetPlayer(m_Username);
    event.SetSquad(squad);
    event.SetMessage(mesg);
    event.SetType(ChatEvent::ChatSquad);

    DISPATCH(m_Handlers, event);
}

bool CNConnection::RegisterHandler(EventHandlerBase *handler) {
    m_Handlers.push_back(handler);
    return true;
}
