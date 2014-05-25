#ifndef LOGIN_EVENT_H_
#define LOGIN_EVENT_H_

#include "Event.h"

class LoginEvent : public Event {
    private:
        std::string m_Name;
        std::string m_Message;
        bool m_Success;

    public:
        LoginEvent(CNConnection *conn) : Event(conn), m_Name("") { }

        const std::string &GetName() const { return m_Name; }
        const std::string &GetMessage() const { return m_Message; }
        bool GetSuccess() const { return m_Success; }

        void SetName(const std::string &name) { m_Name = name; }
        void SetMessage(const std::string &mesg) { m_Message = mesg; }
        void SetSuccess(bool success) { m_Success = success; }

        void Dispatch(EventHandlerBase *handler) {
            DDispatch(handler, this);
        }
};


#endif
