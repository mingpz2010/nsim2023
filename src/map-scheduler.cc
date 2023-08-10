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

#include "map-scheduler.h"

#include "assert.h"
#include "event-impl.h"
#include "log.h"

#include <string>


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("MapScheduler");

NS_OBJECT_ENSURE_REGISTERED(MapScheduler);

TypeId
MapScheduler::GetTypeId()
{
    static TypeId tid = TypeId("nsim2023::MapScheduler")
                            .SetParent<Scheduler>()
                            .SetGroupName("Core")
                            .AddConstructor<MapScheduler>();
    return tid;
}

MapScheduler::MapScheduler()
{
    NS_LOG_FUNCTION(this);
}

MapScheduler::~MapScheduler()
{
    NS_LOG_FUNCTION(this);
}

void
MapScheduler::Insert(const Event& ev)
{
    NS_LOG_FUNCTION(this << ev.impl << ev.key.m_ts << ev.key.m_uid);
    std::pair<EventMapI, bool> result;
    result = m_list.insert(std::make_pair(ev.key, ev.impl));
    NS_ASSERT(result.second);
}

bool
MapScheduler::IsEmpty() const
{
    NS_LOG_FUNCTION(this);
    return m_list.empty();
}

Scheduler::Event
MapScheduler::PeekNext() const
{
    NS_LOG_FUNCTION(this);
    EventMapCI i = m_list.begin();
    NS_ASSERT(i != m_list.end());

    Event ev;
    ev.impl = i->second;
    ev.key = i->first;
    NS_LOG_DEBUG(this << ev.impl << ev.key.m_ts << ev.key.m_uid);
    return ev;
}

Scheduler::Event
MapScheduler::RemoveNext()
{
    NS_LOG_FUNCTION(this);
    EventMapI i = m_list.begin();
    NS_ASSERT(i != m_list.end());
    Event ev;
    ev.impl = i->second;
    ev.key = i->first;
    m_list.erase(i);
    NS_LOG_DEBUG(this << ev.impl << ev.key.m_ts << ev.key.m_uid);
    return ev;
}

void
MapScheduler::Remove(const Event& ev)
{
    NS_LOG_FUNCTION(this << ev.impl << ev.key.m_ts << ev.key.m_uid);
    EventMapI i = m_list.find(ev.key);
    NS_ASSERT(i->second == ev.impl);
    m_list.erase(i);
}

}

