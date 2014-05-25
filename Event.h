#ifndef EVENT_H_
#define EVENT_H_

#include <vector>
#include "CNConnection.h"

class EventHandlerBase {
    public:
        virtual ~EventHandlerBase() { }
};

template <typename EventType>
class EventHandler : public virtual EventHandlerBase {
    public:
        virtual void OnEvent(EventType *) = 0;
        virtual ~EventHandler() { }
};

class Event {
    protected:
        CNConnection *m_Connection;

        template <typename EventType>
        void DDispatch(EventHandlerBase *handler, EventType *self) {
            // Try to cast to the handler type that handles the event type
            EventHandler<EventType> *ehandler = dynamic_cast<EventHandler<EventType> *>(handler);

            // Check if the handler can handle this type of event
            if (ehandler)
                ehandler->OnEvent(self);
        }

    public:
        Event(CNConnection *conn) : m_Connection(conn) { }
        virtual ~Event() { }

        CNConnection *GetConnection() { return m_Connection; }
};

#endif
