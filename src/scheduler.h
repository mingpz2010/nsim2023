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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "object.h"

#include <stdint.h>


namespace nsim2023
{

class EventImpl;


class Scheduler : public Object
{
  public:

    static TypeId GetTypeId();

    struct EventKey
    {
        uint64_t m_ts;      /**< Event time stamp. */
        uint32_t m_uid;     /**< Event unique id. */
        uint32_t m_context; /**< Event context. */
    };

    /**
     * An Event consists of an EventKey, used for maintaining the schedule,
     * and an EventImpl which is the actual implementation.
     */
    struct Event
    {
        EventImpl* impl; /**< Pointer to the event implementation. */
        EventKey key;    /**< Key for sorting and ordering Events. */
    };

    /** Destructor. */
    ~Scheduler() override = 0;

    /**
     * Insert a new Event in the schedule.
     */
    virtual void Insert(const Event& ev) = 0;
    /**
     * Test if the schedule is empty.
     */
    virtual bool IsEmpty() const = 0;
    /**
     * Get a pointer to the next event.
     *
     * This method cannot be invoked if the list is empty.
     */
    virtual Event PeekNext() const = 0;
    /**
     * Remove the earliest event from the event list.
     *
     * This method cannot be invoked if the list is empty.
     */
    virtual Event RemoveNext() = 0;
    /**
     * Remove a specific event from the event list.
     *
     * This method cannot be invoked if the list is empty.
     */
    virtual void Remove(const Event& ev) = 0;
};

/**
 * Compare (equal) two events by EventKey.
 */
inline bool
operator==(const Scheduler::EventKey& a, const Scheduler::EventKey& b)
{
    return a.m_uid == b.m_uid;
}

/**
 * Compare (not equal) two events by EventKey.
 */
inline bool
operator!=(const Scheduler::EventKey& a, const Scheduler::EventKey& b)
{
    return a.m_uid != b.m_uid;
}

/**
 * Compare (less than) two events by EventKey.
 */
inline bool
operator<(const Scheduler::EventKey& a, const Scheduler::EventKey& b)
{
    if (a.m_ts < b.m_ts)
    {
        return true;
    }
    else if (a.m_ts == b.m_ts && a.m_uid < b.m_uid)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Compare (greater than) two events by EventKey.
 */
inline bool
operator>(const Scheduler::EventKey& a, const Scheduler::EventKey& b)
{
    if (a.m_ts > b.m_ts)
    {
        return true;
    }
    else if (a.m_ts == b.m_ts && a.m_uid > b.m_uid)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Compare (equal) two events by Event.
 */
inline bool
operator==(const Scheduler::Event& a, const Scheduler::Event& b)
{
    return a.key == b.key;
}

/**
 * Compare (not equal) two events by Event.
 */
inline bool
operator!=(const Scheduler::Event& a, const Scheduler::Event& b)
{
    return a.key != b.key;
}

/**
 * Compare (less than) two events by Event.
 */
inline bool
operator<(const Scheduler::Event& a, const Scheduler::Event& b)
{
    return a.key < b.key;
}

/**
 * Compare (greater than) two events by Event.
 */
inline bool
operator>(const Scheduler::Event& a, const Scheduler::Event& b)
{
    return a.key > b.key;
}

}

#endif /* SCHEDULER_H */

