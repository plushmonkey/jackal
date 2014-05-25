#include "IPAddress.h"
#include <sstream>

using namespace jackal::net;

const IPAddress IPAddress::None;

void IPAddress::SetIP(Octets o) {
    m_IP = (o[0] << 24) | (o[1] << 16) | (o[2] << 8) | (o[3]);
}

void IPAddress::SetIP(const std::string &ip) {
    std::stringstream ss(ip);
    std::string token;
    Octets o;

    while (std::getline(ss, token, '.'))
        o.push_back(atoi(token.c_str()));

    if (o.size() != 4) return;

    SetIP(o);
}

IPAddress::IPAddress() : m_IP(0) {

}

IPAddress::IPAddress(const std::string &ip) : m_IP(0) {
    SetIP(ip);
}

IPAddress::IPAddress(const std::wstring &ip) {
    std::string str(ip.begin(), ip.end());
    SetIP(str);
}

IPAddress::IPAddress(u32 ip) : m_IP(ip) {

}

IPAddress::IPAddress(u8 o1, u8 o2, u8 o3, u8 o4) {
    m_IP = (o1 << 24) | (o2 << 16) | (o3 << 8) | (o4);
}

std::vector<u8> IPAddress::GetOctets() const {
    u8 o1 = (m_IP & 0xFF000000) >> 24;
    u8 o2 = (m_IP & 0x00FF0000) >> 16;
    u8 o3 = (m_IP & 0x0000FF00) >> 8;
    u8 o4 = m_IP & 0x000000FF;

    Octets octets;

    octets.push_back(o1);
    octets.push_back(o2);
    octets.push_back(o3);
    octets.push_back(o4);

    return octets;
}

std::wstring IPAddress::ToWString() const {
    std::wstringstream ss;
    Octets o = GetOctets();

    ss << (int)o[0] << "." << (int)o[1] << "." << (int)o[2] << "." << (int)o[3];

    return ss.str();
}

std::string IPAddress::ToString() const {
    Octets o = GetOctets();

    std::stringstream ss;

    ss << (int)o[0] << "." << (int)o[1] << "." << (int)o[2] << "." << (int)o[3];

    return ss.str();
}

namespace jackal {
namespace net {

    std::wostream& operator<<(std::wostream &out, const IPAddress &ip) {
        out << ip.ToWString();
        return out;
    }

    std::ostream& operator<<(std::ostream &out, const IPAddress &ip) {
        out << ip.ToString();
        return out;
    }

    bool IPAddress::operator==(const IPAddress &right) {
        return m_IP == right.m_IP;
    }

    bool IPAddress::operator!=(const IPAddress &right) {
        return m_IP != right.m_IP;
    }
}
}
