#ifndef SOCKET_H_
#define SOCKET_H_

#include "Common.h"
#include "IPAddress.h"

namespace jackal {
namespace net {


class TCPSocket {
    public:
        enum Status {
            None, Connected, Disconnected, ForcedClose
        };

    private:
        SocketHandle m_Handle;
        Status m_Status;
        IPAddress m_RemoteIP;
        Port m_Port;
    public:
        TCPSocket();
        virtual ~TCPSocket();
        IPAddress Connect(const std::string &ip, Port port);
        IPAddress Connect(IPAddress &ip, Port port);
        IPAddress Connect(IPAddressList &iplist, Port port);

        Status GetStatus() const;

        void Disconnect();

        IPAddress RemoteAddress() const;

        void SetBlocking(bool block);

        size_t Send(const std::string &data);
        size_t Receive(std::string &data, size_t amount);
};

}
}


#endif
