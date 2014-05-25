#ifndef KILL_EVENT_H_
#define KILL_EVENT_H_

#include "Event.h"

class KillEvent : public Event {
private:
    std::string m_Killer;
    std::string m_Killed;
    int m_Bounty;
    int m_Flags;

public:
    KillEvent(CNConnection *conn) : Event(conn) { }

    const std::string &GetKiller() const { return m_Killer; }
    const std::string &GetKilled() const { return m_Killed; }
    int GetBounty() const { return m_Bounty; }
    int GetFlags() const { return m_Flags; }

    void SetKiller(const std::string &name) { m_Killer = name; }
    void SetKilled(const std::string &name) { m_Killed = name; }
    void SetBounty(int bounty) { m_Bounty = bounty; }
    void SetFlags(int flags) { m_Flags = flags; }

    void Dispatch(EventHandlerBase *handler) {
        DDispatch(handler, this);
    }
};


#endif
