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

#include "event-id.h"

#include "event-impl.h"
#include "log.h"
#include "simulator.h"

namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("EventId");

EventId::EventId()
    : m_eventImpl(nullptr),
      m_ts(0),
      m_context(0),
      m_uid(0)
{
    NS_LOG_FUNCTION(this);
}

EventId::EventId(const Ptr<EventImpl>& impl, uint64_t ts, uint32_t context, uint32_t uid)
    : m_eventImpl(impl),
      m_ts(ts),
      m_context(context),
      m_uid(uid)
{
    NS_LOG_FUNCTION(this << impl << ts << context << uid);
}

void
EventId::Cancel()
{
    NS_LOG_FUNCTION(this);
    Simulator::Cancel(*this);
}

void
EventId::Remove()
{
    NS_LOG_FUNCTION(this);
    Simulator::Remove(*this);
}

bool
EventId::IsExpired() const
{
    NS_LOG_FUNCTION(this);
    return Simulator::IsExpired(*this);
}

bool
EventId::IsRunning() const
{
    NS_LOG_FUNCTION(this);
    return !IsExpired();
}

EventImpl*
EventId::PeekEventImpl() const
{
    NS_LOG_FUNCTION(this);
    return PeekPointer(m_eventImpl);
}

uint64_t
EventId::GetTs() const
{
    NS_LOG_FUNCTION(this);
    return m_ts;
}

uint32_t
EventId::GetContext() const
{
    NS_LOG_FUNCTION(this);
    return m_context;
}

uint32_t
EventId::GetUid() const
{
    NS_LOG_FUNCTION(this);
    return m_uid;
}

}

