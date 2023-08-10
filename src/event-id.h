/*
    Copyright © 2023 <Pingzhou Ming>

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the “Software”), to deal in 
    the Software without restriction, including without limitation the rights to use, 
    copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
    and to permit persons to whom the Software is furnished to do so, subject to the 
    following conditions:

    The above copyright notice and this permission notice shall be included in all copies 
    or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS 
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
    THE SOFTWARE.
*/

#ifndef EVENT_ID_H
#define EVENT_ID_H

#include "event-impl.h"
#include "ptr.h"

#include <stdint.h>

namespace nsim2023
{

class EventImpl;


class EventId
{
  public:
    /** Special values of the event UID. */
    enum UID
    {
        /** Invalid UID value. */
        INVALID = 0,
        /** ScheduleNow() events. */
        NOW = 1,
        /** ScheduleDestroy() events. */
        DESTROY = 2,
        /** Reserved UID. */
        RESERVED = 3,
        /** Schedule(), etc. events. */
        VALID = 4
    };

    /** Default constructor. This EventId does nothing. */
    EventId();

    EventId(const Ptr<EventImpl>& impl, uint64_t ts, uint32_t context, uint32_t uid);

    void Cancel();

    void Remove();

    bool IsExpired() const;

    bool IsRunning() const;

  public:

    EventImpl* PeekEventImpl() const;

    uint64_t GetTs() const;

    uint32_t GetContext() const;

    uint32_t GetUid() const;

    friend bool operator==(const EventId& a, const EventId& b);

    friend bool operator!=(const EventId& a, const EventId& b);

    friend bool operator<(const EventId& a, const EventId& b);

  private:
    Ptr<EventImpl> m_eventImpl;
    uint64_t m_ts;
    uint32_t m_context;
    uint32_t m_uid;
};

/*************************************************
 **  Inline implementations
 ************************************************/

inline bool
operator==(const EventId& a, const EventId& b)
{
    return a.m_uid == b.m_uid && a.m_context == b.m_context && a.m_ts == b.m_ts &&
           a.m_eventImpl == b.m_eventImpl;
}

inline bool
operator!=(const EventId& a, const EventId& b)
{
    return !(a == b);
}

inline bool
operator<(const EventId& a, const EventId& b)
{
    return (a.GetTs() < b.GetTs());
}

}

#endif /* EVENT_ID_H */

