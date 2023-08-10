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

#ifndef SIMULATOR_IMPL_H
#define SIMULATOR_IMPL_H

#include "event-id.h"
#include "event-impl.h"
#include "nstime.h"
#include "object-factory.h"
#include "object.h"
#include "ptr.h"


namespace nsim2023
{

class Scheduler;


class SimulatorImpl : public Object
{
  public:
    /**
     * Get the registered TypeId for this class.
     */
    static TypeId GetTypeId();


    virtual void Destroy() = 0;

    virtual bool IsFinished() const = 0;

    virtual void Stop() = 0;

    virtual void Stop(const Time& delay) = 0;

    virtual EventId Schedule(const Time& delay, EventImpl* event) = 0;

    virtual void ScheduleWithContext(uint32_t context, const Time& delay, EventImpl* event) = 0;

    virtual EventId ScheduleNow(EventImpl* event) = 0;

    virtual EventId ScheduleDestroy(EventImpl* event) = 0;

    virtual void Remove(const EventId& id) = 0;

    virtual void Cancel(const EventId& id) = 0;

    virtual bool IsExpired(const EventId& id) const = 0;

    virtual void Run() = 0;

    virtual Time Now() const = 0;

    virtual Time GetDelayLeft(const EventId& id) const = 0;

    virtual Time GetMaximumSimulationTime() const = 0;

    /**
     * Set the Scheduler to be used to manage the event list.
     */
    virtual void SetScheduler(ObjectFactory schedulerFactory) = 0;

    virtual uint32_t GetSystemId() const = 0;

    virtual uint32_t GetContext() const = 0;

    virtual uint64_t GetEventCount() const = 0;

    /**
     * Hook called before processing each event.
     */
    virtual void PreEventHook(const EventId& id){};
};

}

#endif /* SIMULATOR_IMPL_H */

