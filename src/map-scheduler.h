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

#ifndef MAP_SCHEDULER_H
#define MAP_SCHEDULER_H

#include "scheduler.h"

#include <map>
#include <stdint.h>
#include <utility>



namespace nsim2023
{

/**
 * This class implements the an event scheduler using an std::map
 * data structure.
 *
 * Time Complexity
 *
 * Operation    | Amortized %Time | Reason
 * :----------- | :-------------- | :-----
 * Insert()     | Logarithmic     | `std::map::insert()`
 * IsEmpty()    | Constant        | `std::map::empty()`
 * PeekNext()   | Constant        | `std::map::begin()`
 * Remove()     | Logarithmic     | `std::map::find()`
 * RemoveNext() | Constant        | `std::map::begin()`
 *
 * Memory Complexity
 *
 * Category  | Memory                           | Reason
 * :-------- | :------------------------------- | :-----
 * Overhead  | 3 x `sizeof (*)` + 2 x `size_t`<br/>(40 bytes) | red-black tree
 * Per Event | 3 x `sizeof (*)` + `int`<br/>(32 bytes)        | red-black tree
 *
 */
class MapScheduler : public Scheduler
{
  public:

    static TypeId GetTypeId();

    /** Constructor. */
    MapScheduler();
    /** Destructor. */
    ~MapScheduler() override;

    // Inherited
    void Insert(const Scheduler::Event& ev) override;
    bool IsEmpty() const override;
    Scheduler::Event PeekNext() const override;
    Scheduler::Event RemoveNext() override;
    void Remove(const Scheduler::Event& ev) override;

  private:
    /** Event list type: a Map from EventKey to EventImpl. */
    typedef std::map<Scheduler::EventKey, EventImpl*> EventMap;
    /** EventMap iterator. */
    typedef std::map<Scheduler::EventKey, EventImpl*>::iterator EventMapI;
    /** EventMap const iterator. */
    typedef std::map<Scheduler::EventKey, EventImpl*>::const_iterator EventMapCI;

    /** The event list. */
    EventMap m_list;
};

}

#endif /* MAP_SCHEDULER_H */

