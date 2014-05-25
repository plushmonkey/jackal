#include "Net.h"

using namespace jackal::net;

TCPSocket::TCPSocket() : m_Status(None), m_Handle(-1), m_RemoteIP(IPAddress::None), m_Port(0) {
    if (!jackal::net::initialized)
        jackal::net::Initialize();

    m_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

TCPSocket::~TCPSocket() { Disconnect(); }

IPAddress TCPSocket::Connect(const std::string &ip, Port port) {
    return this->Connect(IPAddress(ip), port);
}

IPAddress TCPSocket::Connect(IPAddress &ip, Port port) {
    if (m_Handle < 0) {
        m_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_Handle < 0)
            return false;
    }

    sockaddr_in in = { 0 };

    in.sin_port = htons(port);
    in.sin_addr.s_addr = inet_addr(ip.ToString().c_str());
    in.sin_family = AF_INET;

    if (connect(m_Handle, (const sockaddr *)&in, sizeof(sockaddr_in)) != 0) {
        m_Port = 0;
        m_Status = None;
        m_RemoteIP = IPAddress::None;
        return IPAddress::None;
    }

    m_RemoteIP = ip;
    m_Port = port;
    m_Status = Connected;
    return ip;
}

IPAddress TCPSocket::Connect(IPAddressList &iplist, Port port) {
    IPIterator iter = iplist.begin();

    for (; iter != iplist.end(); iter++) {
        if (Connect(*iter, port) != IPAddress::None)
            return *iter;
    }

    m_RemoteIP = IPAddress::None;

    return IPAddress::None;
}

void TCPSocket::Disconnect() {
    closesocket(m_Handle);
    m_Handle = -1;
    m_Status = Disconnected;
}

IPAddress TCPSocket::RemoteAddress() const {
    return m_RemoteIP;
}

void TCPSocket::SetBlocking(bool block) {
    unsigned long mode = block ? 0 : 1;
    ioctlsocket(m_Handle, FIONBIO, &mode);
}

TCPSocket::Status TCPSocket::GetStatus() const {
    return m_Status;
}

size_t TCPSocket::Send(const std::string &data) {
    size_t toSend = data.length();
    size_t sent = 0;

    while (sent < data.length()) {
        int cur = send(m_Handle, data.c_str() + sent, data.length() - sent, 0);
        if (cur < 0) {
            Disconnect();
            m_Status = ForcedClose;
            return -1;
        }
        sent += cur;
    }
    return sent;
}

size_t TCPSocket::Receive(std::string &data, size_t amount) {
    fd_set readfds;
    timeval tv;

    FD_ZERO(&readfds);
    FD_SET(m_Handle, &readfds);

    tv.tv_sec = 0;
    tv.tv_usec = 100;

    data = "";

    if (select(1, &readfds, NULL, NULL, &tv) == 0)
        return 0;

    char *buffer = new char[amount];

    memset(buffer, 0, amount);

    int cur = recv(m_Handle, buffer, amount, 0);
    if (cur < 0) {
        delete[] buffer;
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK)
            return 0;
        Disconnect();
        m_Status = ForcedClose;
        return -1;
    } else if (cur == 0) {
        delete[] buffer;
        Disconnect();
        return -1;
    }

    data.append(buffer, cur);
    delete[] buffer;

    return cur;
}
