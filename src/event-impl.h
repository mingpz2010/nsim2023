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

#ifndef EVENT_IMPL_H
#define EVENT_IMPL_H

#include "simple-ref-count.h"

#include <stdint.h>


namespace nsim2023
{

class EventImpl : public SimpleRefCount<EventImpl>
{
  public:
    /** Default constructor. */
    EventImpl();
    /** Destructor. */
    virtual ~EventImpl() = 0;

    void Invoke();

    void Cancel();

    bool IsCancelled();

  protected:
    /**
     * Implementation for Invoke().
     *
     * This typically calls a method or function pointer with the
     * arguments bound by a call to one of the MakeEvent() functions.
     */
    virtual void Notify() = 0;

  private:
    bool m_cancel;
};

}

#endif /* EVENT_IMPL_H */

