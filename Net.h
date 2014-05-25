#ifndef NET_H_
#define NET_H_

#include "Common.h"
#include "IPAddress.h"
#include "Socket.h"

namespace jackal {
namespace net {
    extern bool initialized;
}
}

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// fucking Windows
#ifdef GetMessage
#undef GetMessage
#endif
    
#pragma comment(lib, "ws2_32.lib")

namespace jackal {
namespace net {
    bool inline Initialize() {
        WORD version = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(version, &wsaData) == 0) {
            initialized = true;
            return true;
        } else {
            return false;
        }
        return WSAStartup(version, &wsaData) == 0;
    }

    void inline Cleanup() {
        WSACleanup();
        initialized = false;
    }
#else
namespace jackal {
namespace net {
    bool inline Initialize() {
        initialized = true;
        return true;
    }
    void inline Cleanup() {
        initialized = false;
    }
#endif
}
}

#endif
