#include "Jackal.h"
#include "EnterEvent.h"
#include "LoginEvent.h"
#include "KillEvent.h"
#include "ShipFreqChangeEvent.h"
#include <iostream>
#include <stdexcept>

Jackal::Jackal(const std::string &host, u16 port) : CNConnection(host, port) {
    m_DataHandlers["LOGINOK"] = std::bind(&Jackal::LoginHandler, this, std::placeholders::_1);
    m_DataHandlers["LOGINBAD"] = std::bind(&Jackal::LoginHandler, this, std::placeholders::_1);

    m_DataHandlers["MSG"] = std::bind(&Jackal::ChatHandler, this, std::placeholders::_1);

    m_DataHandlers["PLAYER"] = std::bind(&Jackal::PlayerHandler, this, std::placeholders::_1);
    m_DataHandlers["ENTERING"] = std::bind(&Jackal::PlayerHandler, this, std::placeholders::_1);
    m_DataHandlers["LEAVING"] = std::bind(&Jackal::PlayerHandler, this, std::placeholders::_1);

    m_DataHandlers["KILL"] = std::bind(&Jackal::KillHandler, this, std::placeholders::_1);

    m_DataHandlers["SHIPFREQCHANGE"] = std::bind(&Jackal::ShipHandler, this, std::placeholders::_1);

    m_ClientVer = "Jackal v1.0, by monkey";
}

Jackal::~Jackal() {

}

void Jackal::LoginHandler(const jackal::util::Tokenizer &tokens) {
    LoginEvent event(this);

    if (tokens[0].compare("LOGINOK") == 0) {
        event.SetSuccess(true);
        event.SetName(tokens[1]);
    } else {
        event.SetSuccess(false);
        event.SetMessage(tokens[1]);
    }

    DISPATCH(m_Handlers, event);
}

void Jackal::ChatHandler(const jackal::util::Tokenizer &tokens) {
    ChatEvent::ChatType type = ChatEvent::TypeFromString(tokens[1]);
    if (type == ChatEvent::ChatUnknown)
        throw std::runtime_error("Error determining chat type from chat message.");

    std::string raw = "";

    for (auto const &s : tokens) {
        raw += s;
        if (&s != &tokens[tokens.size() - 1])
            raw += ":";
    }

    ChatEvent event(this);

    event.SetType(type);
    event.SetInbound(true);

    switch (type) {
        case ChatEvent::ChatArena:
        case ChatEvent::ChatCommand:
        {
            jackal::util::Tokenizer chat_tokens(raw, ':', 3);
            event.SetMessage(chat_tokens[2]);
        }
        break;
        case ChatEvent::ChatPublic:
        case ChatEvent::ChatPublicMacro:
        case ChatEvent::ChatPrivate:
        case ChatEvent::ChatFrequency:
        case ChatEvent::ChatModerator:
        {
            jackal::util::Tokenizer chat_tokens(raw, ':', 4);
            event.SetPlayer(chat_tokens[2]);
            event.SetMessage(chat_tokens[3]);
        }
        break;
        case ChatEvent::ChatChannel:
        {
            jackal::util::Tokenizer chat_tokens(raw, ':', 4);
            int channel = atoi(chat_tokens[2].c_str());
            event.SetChannel(channel);
            event.SetMessage(chat_tokens[3]);
        }
        break;
        case ChatEvent::ChatSysop:
        {
            jackal::util::Tokenizer chat_tokens(raw, ':', 3);
            event.SetMessage(chat_tokens[2]);
        }
        break;
        case ChatEvent::ChatSquad:
        {
            jackal::util::Tokenizer chat_tokens(raw, ':', 5);
            event.SetSquad(chat_tokens[2]);
            event.SetPlayer(chat_tokens[3]);
            event.SetMessage(chat_tokens[4]);
        }
        break;
    }
    DISPATCH(m_Handlers, event);
}

void Jackal::PlayerHandler(const jackal::util::Tokenizer &tokens) {
    if (tokens[0].compare("PLAYER") == 0 || tokens[0].compare("ENTERING") == 0) {
        int ship = atoi(tokens[2].c_str());
        int freq = atoi(tokens[3].c_str());
        std::string name = tokens[1];

        EnterEvent event(this);
        
        event.SetPlayer(name);
        event.SetShip(ship);
        event.SetFrequency(freq);

        DISPATCH(m_Handlers, event);
    } else if (tokens[0].compare("LEAVING") == 0) {
        std::string name = tokens[1];
        LeaveEvent event(this);

        event.SetPlayer(name);

        DISPATCH(m_Handlers, event);
    }
}

void Jackal::KillHandler(const jackal::util::Tokenizer &tokens) {
    std::string killer = tokens[1];
    std::string killed = tokens[2];
    int bounty = atoi(tokens[3].c_str());
    int flags = atoi(tokens[4].c_str());

    KillEvent event(this);

    event.SetKiller(killer);
    event.SetKilled(killed);
    event.SetBounty(bounty);
    event.SetFlags(flags);

    DISPATCH(m_Handlers, event);
}

void Jackal::ShipHandler(const jackal::util::Tokenizer &tokens) {
    std::string name = tokens[1];
    int ship = atoi(tokens[2].c_str());
    int freq = atoi(tokens[3].c_str());

    ShipFreqChangeEvent event(this);

    event.SetPlayer(name);
    event.SetShip(ship);
    event.SetFrequency(freq);

    DISPATCH(m_Handlers, event);
}

int Jackal::Run(const std::string &username, const std::string &password) {
    std::string unhandled = "";
    std::string data;

    m_Username = username;
    m_Password = password;

    if (!Connect()) {
        std::stringstream ss;
        ss << "Could not connect to " << m_Host << ":" << m_Port << ".";
        throw std::runtime_error(ss.str());
    }

    SendLogin();

    while (GetConnectionStatus() == jackal::net::TCPSocket::Connected) {
        size_t amount = ReceiveRaw(data, 1024);

        unhandled += data;

        while (unhandled.length() > 0) {
            size_t pos = unhandled.find("\n");

            if (pos == std::string::npos)
                break;

            std::string tohandle = unhandled.substr(0, pos + 1);
            unhandled.replace(0, pos + 1, "");

            jackal::util::Tokenizer tokens(tohandle.substr(0, tohandle.size() - 1), ':');

            if (tokens.size() == 0) {
                std::cerr << "Error with data received." << std::endl;
                continue;
            }

            try {
                if (m_DataHandlers.find(tokens[0]) != m_DataHandlers.end())
                    m_DataHandlers[tokens[0]](tokens);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
        Sleep(100);
    }
    return 0;
}
