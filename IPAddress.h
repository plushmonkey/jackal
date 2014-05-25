#ifndef IPADDRESS_H_
#define IPADDRESS_H_

#include <vector>
#include <ostream>
#include "Common.h"

namespace jackal {
namespace net {


class IPAddress {
    private:
        typedef std::vector<u8> Octets;
        u32 m_IP;

        Octets GetOctets() const;
        void SetIP(Octets o);
        void SetIP(const std::string &ip);
    public:
        IPAddress();
        IPAddress(const std::string &ip);
        IPAddress(const std::wstring &ip);
        IPAddress(u32 ip);
        IPAddress(u8 o1, u8 o2, u8 o3, u8 o4);

        std::wstring ToWString() const;
        std::string ToString() const;

        friend std::ostream& operator<<(std::ostream &out, const IPAddress &ip);
        friend std::wostream& operator<<(std::wostream &out, const IPAddress &ip);

        bool operator==(const IPAddress &right);
        bool operator!=(const IPAddress &right);

        static const IPAddress None;
};

typedef std::vector<IPAddress> IPAddressList;
typedef std::vector<IPAddress>::iterator IPIterator;


}
}

#endif
