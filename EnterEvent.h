#ifndef ENTER_EVENT_H_
#define ENTER_EVENT_H_

#include "Event.h"

class EnterEvent : public Event {
    private:
        std::string m_Player;
        int m_Ship;
        int m_Freq;
        
    public:
        EnterEvent(CNConnection *conn) : Event(conn), m_Player("") { }

        const std::string &GetPlayer() const { return m_Player; }
        int GetShip() const { return m_Ship; }
        int GetFrequency() const { return m_Freq; }

        void SetPlayer(const std::string &name) { m_Player = name; }
        void SetShip(int ship) { m_Ship = ship; }
        void SetFrequency(int freq) { m_Freq = freq; }

        void Dispatch(EventHandlerBase *handler) {
            DDispatch(handler, this);
        }
};

class LeaveEvent : public Event {
    private:
        std::string m_Player;
    public:
        LeaveEvent(CNConnection *conn) : Event(conn), m_Player("") { }

        const std::string &GetPlayer() const { return m_Player; }

        void SetPlayer(const std::string &name) { m_Player = name; }

        void Dispatch(EventHandlerBase *handler) {
            DDispatch(handler, this);
        }
};

#endif
