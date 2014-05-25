#ifndef SHIPFREQCHANGE_EVENT_H_
#define SHIPFREQCHANGE_EVENT_H_

#include "Event.h"

class ShipFreqChangeEvent : public Event {
    private:
        std::string m_Player;
        int m_Ship;
        int m_Freq;

    public:
        ShipFreqChangeEvent(CNConnection *conn) : Event(conn), m_Player("") { }

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

#endif
