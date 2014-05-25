#ifndef JACKAL_H_
#define JACKAL_H_

#include "CNConnection.h"
#include "Tokenizer.h"
#include "Event.h"
#include "ChatEvent.h"
#include "EnterEvent.h"
#include "KillEvent.h"
#include "LoginEvent.h"
#include "ShipFreqChangeEvent.h"
#include <functional>
#include <map>

typedef std::function<void(const jackal::util::Tokenizer &)> DataHandler;

class Jackal : public CNConnection {
    private:
        std::map<std::string, DataHandler> m_DataHandlers;

    public:
        Jackal(const std::string &host, u16 port);
        ~Jackal();

        void LoginHandler(const jackal::util::Tokenizer &tokens);
        void ChatHandler(const jackal::util::Tokenizer &tokens);
        void PlayerHandler(const jackal::util::Tokenizer &tokens);
        void KillHandler(const jackal::util::Tokenizer &tokens);
        void ShipHandler(const jackal::util::Tokenizer &tokens);

        int Run(const std::string &username, const std::string &password);

        void OnEvent(LoginEvent *event);
};


#endif
