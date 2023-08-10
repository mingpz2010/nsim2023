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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "event-id.h"
#include "event-impl.h"
#include "make-event.h"
#include "nstime.h"
#include "object-factory.h"
#include "string"

#include <stdint.h>
#include <cstring>


namespace nsim2023
{

class SimulatorImpl;
class Scheduler;


/**
 * The internal simulation clock is maintained
 * as a 64-bit integer in a unit specified by the user
 * through the Time::SetResolution function. This means that it is
 * not possible to specify event expiration times with anything better
 * than this user-specified accuracy. Events whose expiration time is
 * the same modulo this accuracy are scheduled in FIFO order: the
 * first event inserted in the scheduling queue is scheduled to
 * expire first.
 */
class Simulator
{
  public:
    // Delete default constructor and destructor to avoid misuse
    Simulator() = delete;
    ~Simulator() = delete;

    /**
     * The simulator provides a mechanism to swap out different implementations.
     * For example, the default implementation is a single-threaded simulator
     * that performs no realtime synchronization.  By calling this method, you
     * can substitute in a new simulator implementation that might be multi-
     * threaded and synchronize events to a realtime clock.
     *
     * The simulator implementation can be set when the simulator is not
     * running.
     */
    static void SetImplementation(Ptr<SimulatorImpl> impl);

    /**
     * If the SimulatorImpl singleton hasn't been created yet,
     * this function does so.  At the same time it also creates
     * the Scheduler.  Both of these respect the global values
     * which may have been set from the command line or through
     * the Config system.
     *
     * As a side effect we also call LogSetTimePrinter() and
     * LogSetNodePrinter() with the default implementations
     * since we can't really do any logging until we have
     * a SimulatorImpl and Scheduler.
     */
    static Ptr<SimulatorImpl> GetImplementation();

    /**
     * The event scheduler can be set at any time: the events scheduled
     * in the previous scheduler will be transferred to the new scheduler
     * before we start to use it.
     */
    static void SetScheduler(ObjectFactory schedulerFactory);

    /**
     * Execute the events scheduled with ScheduleDestroy().
     *
     * This method is typically invoked at the end of a simulation
     * to avoid false-positive reports by a leak checker.
     * After this method has been invoked, it is actually possible
     * to restart a new simulation with a set of calls to Simulator::Run,
     * Simulator::Schedule and Simulator::ScheduleWithContext.
     */
    static void Destroy();

    /**
     * Check if the simulation should finish.
     *
     * Reasons to finish are because there are
     * no more events lefts to be scheduled, or if simulation
     * time has already reached the "stop time" (see Simulator::Stop()).
     */
    static bool IsFinished();

    /**
     * Run the simulation.
     *
     * The simulation will run until one of:
     *   - No events are present anymore
     *   - The user called Simulator::Stop
     *   - The user called Simulator::Stop with a stop time and the
     *     expiration time of the next event to be processed
     *     is greater than or equal to the stop time.
     */
    static void Run();

    /**
     * Tell the Simulator the calling event should be the last one
     * executed.
     *
     * If a running event invokes this method, it will be the last
     * event executed by the Simulator::Run method before
     * returning to the caller.
     */
    static void Stop();

    /**
     * Schedule the time delay until the Simulator should stop.
     *
     * Force the Simulator::Run method to return to the caller when the
     * expiration time of the next event to be processed is greater than
     * or equal to the stop time.  The stop time is relative to the
     * current simulation time.
     */
    static void Stop(const Time& delay);

    /**
     * Get the current simulation context.
     *
     * The simulation context is the ns-3 notion of a Logical Process.
     * Events in a single context should only modify state associated with
     * that context. Events for objects in other contexts should be
     * scheduled with ScheduleWithContext() to track the context switches.
     * In other words, events in different contexts should be mutually
     * thread safe, by not modify overlapping model state.
     */
    static uint32_t GetContext();

    /**
     * Context enum values.
     * This enum type is fixed to match the representation size
     * of simulation context.
     */
    enum : uint32_t
    {
        /**
         * Flag for events not associated with any particular context.
         */
        NO_CONTEXT = 0xffffffff
    };

    /**
     * Get the number of events executed.
     */
    static uint64_t GetEventCount();

    /**
     * Schedule events (in the same context) to run at a future time.
     */
    /**
     * Schedule an event to expire after delay.
     * This can be thought of as scheduling an event
     * for the current simulation time plus the delay  passed as a parameter.
     */
    template <
        typename FUNC,
        typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type = 0,
        typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                                int>::type = 0,
        typename... Ts>
    static EventId Schedule(const Time& delay, FUNC f, Ts&&... args);

    /**
     * Schedule an event to expire after delay.
     * This can be thought of as scheduling an event
     * for the current simulation time plus the delay  passed as a
     * parameter.
     */
    template <typename... Us, typename... Ts>
    static EventId Schedule(const Time& delay, void (*f)(Us...), Ts&&... args);

    /**
     * Schedule events (in a different context) to run now or at a future time.
     *
     * Schedule an event with the given context.
     * A context of 0xffffffff means no context is specified.
     * This method is thread-safe: it can be called from any thread.
     *
     * We leverage SFINAE to discard this overload if the second argument is
     * convertible to Ptr<EventImpl> or is a function pointer.
     */
    template <
        typename FUNC,
        typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type = 0,
        typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                                int>::type = 0,
        typename... Ts>
    static void ScheduleWithContext(uint32_t context, const Time& delay, FUNC f, Ts&&... args);

    /**
     * Schedule an event with the given context.
     * A context of 0xffffffff means no context is specified.
     * This method is thread-safe: it can be called from any thread.
     */
    template <typename... Us, typename... Ts>
    static void ScheduleWithContext(uint32_t context,
                                    const Time& delay,
                                    void (*f)(Us...),
                                    Ts&&... args);

    /**
     * Schedule an event to expire Now. All events scheduled to
     * to expire "Now" are scheduled FIFO, after all normal events
     * have expired.
     *
     * We leverage SFINAE to discard this overload if the second argument is
     * convertible to Ptr<EventImpl> or is a function pointer.
     */
    template <
        typename FUNC,
        typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type = 0,
        typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                                int>::type = 0,
        typename... Ts>
    static EventId ScheduleNow(FUNC f, Ts&&... args);

    /**
     * Schedule an event to expire Now. All events scheduled to
     * to expire "Now" are scheduled FIFO, after all normal events
     * have expired.
     */
    template <typename... Us, typename... Ts>
    static EventId ScheduleNow(void (*f)(Us...), Ts&&... args);

    /**
     * Schedule events to run at the end of the simulation, when Simulator:Destroy() is
     * called.
     */
    /**
     * Schedule an event to run at the end of the simulation, when Simulator::Destroy() is called.
     * All events scheduled to expire at "Destroy" time are scheduled FIFO,
     * after all normal events have expired and only when
     * Simulator::Destroy is invoked.
     */
    template <
        typename FUNC,
        typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type = 0,
        typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                                int>::type = 0,
        typename... Ts>
    static EventId ScheduleDestroy(FUNC f, Ts&&... args);

    /**
     * Schedule an event to run at the end of the simulation, when Simulator::Destroy() is called.
     * All events scheduled to expire at "Destroy" time are scheduled FIFO,
     * after all normal events have expired and only when
     * Simulator::Destroy is invoked.
     */
    template <typename... Us, typename... Ts>
    static EventId ScheduleDestroy(void (*f)(Us...), Ts&&... args);


    /**
     * Remove an event from the event list.
     */
    static void Remove(const EventId& id);

    /**
     * Set the cancel bit on this event: the event's associated function
     * will not be invoked when it expires.
     */
    static void Cancel(const EventId& id);

    /**
     * Check if an event has already run or been cancelled.
     *
     * This method has O(1) complexity.
     * Note that it is not possible to test for the expiration of
     * events which were scheduled for the "destroy" time. Doing so
     * will result in a program error (crash).
     * An event is said to "expire" when it starts being scheduled
     * which means that if the code executed by the event calls
     * this function, it will get true.
     */
    static bool IsExpired(const EventId& id);

    /**
     * Return the current simulation virtual time.
     */
    static Time Now();

    /**
     * Get the remaining time until this event will execute.
     */
    static Time GetDelayLeft(const EventId& id);

    /**
     * Get the maximum representable simulation time.
     */
    static Time GetMaximumSimulationTime();

    /**
     * Schedule a future event execution (in the same context).
     */
    static EventId Schedule(const Time& delay, const Ptr<EventImpl>& event);

    /**
     * Schedule a future event execution (in a different context).
     * This method is thread-safe: it can be called from any thread.
     */
    static void ScheduleWithContext(uint32_t context, const Time& delay, EventImpl* event);

    /**
     * Schedule an event to run at the end of the simulation, after
     * the Stop() time or condition has been reached.
     */
    static EventId ScheduleDestroy(const Ptr<EventImpl>& event);

    /**
     * Schedule an event to run at the current virtual time.
     */
    static EventId ScheduleNow(const Ptr<EventImpl>& event);

    /**
     * Get the system id of this simulator.
     *
     * The system id is the identifier for this simulator instance
     * in a distributed simulation.  For MPI this is the MPI rank.
     */
    static uint32_t GetSystemId();

  private:
    /**
     * Implementation of the various Schedule methods.
     */
    static EventId DoSchedule(const Time& delay, EventImpl* event);

    /**
     * Implementation of the various ScheduleNow methods.
     */
    static EventId DoScheduleNow(EventImpl* event);

    /**
     * Implementation of the various ScheduleDestroy methods.
     */
    static EventId DoScheduleDestroy(EventImpl* event);

};

/**
 * This is really a shortcut for the ns3::Simulator::Now method.
 * It is typically used as shown below to schedule an event
 * which expires at the absolute time "2 seconds":
 */
Time Now();

}

/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/
namespace nsim2023
{


template <
    typename FUNC,
    typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type,
    typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                            int>::type,
    typename... Ts>
EventId
Simulator::Schedule(const Time& delay, FUNC f, Ts&&... args)
{
    return DoSchedule(delay, MakeEvent(f, std::forward<Ts>(args)...));
}

template <typename... Us, typename... Ts>
EventId
Simulator::Schedule(const Time& delay, void (*f)(Us...), Ts&&... args)
{
    return DoSchedule(delay, MakeEvent(f, std::forward<Ts>(args)...));
}

template <
    typename FUNC,
    typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type,
    typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                            int>::type,
    typename... Ts>
void
Simulator::ScheduleWithContext(uint32_t context, const Time& delay, FUNC f, Ts&&... args)
{
    return ScheduleWithContext(context, delay, MakeEvent(f, std::forward<Ts>(args)...));
}

template <typename... Us, typename... Ts>
void
Simulator::ScheduleWithContext(uint32_t context, const Time& delay, void (*f)(Us...), Ts&&... args)
{
    return ScheduleWithContext(context, delay, MakeEvent(f, std::forward<Ts>(args)...));
}

template <
    typename FUNC,
    typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type,
    typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                            int>::type,
    typename... Ts>
EventId
Simulator::ScheduleNow(FUNC f, Ts&&... args)
{
    return DoScheduleNow(MakeEvent(f, std::forward<Ts>(args)...));
}

template <typename... Us, typename... Ts>
EventId
Simulator::ScheduleNow(void (*f)(Us...), Ts&&... args)
{
    return DoScheduleNow(MakeEvent(f, std::forward<Ts>(args)...));
}

template <
    typename FUNC,
    typename std::enable_if<!std::is_convertible<FUNC, Ptr<EventImpl>>::value, int>::type,
    typename std::enable_if<!std::is_function<typename std::remove_pointer<FUNC>::type>::value,
                            int>::type,
    typename... Ts>
EventId
Simulator::ScheduleDestroy(FUNC f, Ts&&... args)
{
    return DoScheduleDestroy(MakeEvent(f, std::forward<Ts>(args)...));
}

template <typename... Us, typename... Ts>
EventId
Simulator::ScheduleDestroy(void (*f)(Us...), Ts&&... args)
{
    return DoScheduleDestroy(MakeEvent(f, std::forward<Ts>(args)...));
}

}

#endif /* SIMULATOR_H */

