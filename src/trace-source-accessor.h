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

#ifndef TRACE_SOURCE_ACCESSOR_H
#define TRACE_SOURCE_ACCESSOR_H

#include "callback.h"
#include "ptr.h"
#include "simple-ref-count.h"

#include <stdint.h>


namespace nsim2023
{

class ObjectBase;


class TraceSourceAccessor : public SimpleRefCount<TraceSourceAccessor>
{
  public:
    /** Constructor. */
    TraceSourceAccessor();
    /** Destructor. */
    virtual ~TraceSourceAccessor();

    virtual bool ConnectWithoutContext(ObjectBase* obj, const CallbackBase& cb) const = 0;

    virtual bool Connect(ObjectBase* obj, std::string context, const CallbackBase& cb) const = 0;

    virtual bool DisconnectWithoutContext(ObjectBase* obj, const CallbackBase& cb) const = 0;

    virtual bool Disconnect(ObjectBase* obj, std::string context, const CallbackBase& cb) const = 0;
};

/**
 * Create a TraceSourceAccessor which will control access to the underlying
 * trace source.
 */
template <typename T>
Ptr<const TraceSourceAccessor> MakeTraceSourceAccessor(T a);

static inline Ptr<const TraceSourceAccessor>
MakeEmptyTraceSourceAccessor()
{
    return Ptr<const TraceSourceAccessor>(nullptr);
}

}

/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/
namespace nsim2023
{

template <typename T, typename SOURCE>
Ptr<const TraceSourceAccessor>
DoMakeTraceSourceAccessor(SOURCE T::*a)
{
    struct Accessor : public TraceSourceAccessor
    {
        bool ConnectWithoutContext(ObjectBase* obj, const CallbackBase& cb) const override
        {
            T* p = dynamic_cast<T*>(obj);
            if (p == nullptr)
            {
                return false;
            }
            (p->*m_source).ConnectWithoutContext(cb);
            return true;
        }

        bool Connect(ObjectBase* obj, std::string context, const CallbackBase& cb) const override
        {
            T* p = dynamic_cast<T*>(obj);
            if (p == nullptr)
            {
                return false;
            }
            (p->*m_source).Connect(cb, context);
            return true;
        }

        bool DisconnectWithoutContext(ObjectBase* obj, const CallbackBase& cb) const override
        {
            T* p = dynamic_cast<T*>(obj);
            if (p == nullptr)
            {
                return false;
            }
            (p->*m_source).DisconnectWithoutContext(cb);
            return true;
        }

        bool Disconnect(ObjectBase* obj, std::string context, const CallbackBase& cb) const override
        {
            T* p = dynamic_cast<T*>(obj);
            if (p == nullptr)
            {
                return false;
            }
            (p->*m_source).Disconnect(cb, context);
            return true;
        }

        SOURCE T::*m_source;
    }* accessor = new Accessor();

    accessor->m_source = a;
    return Ptr<const TraceSourceAccessor>(accessor, false);
}

template <typename T>
Ptr<const TraceSourceAccessor>
MakeTraceSourceAccessor(T a)
{
    return DoMakeTraceSourceAccessor(a);
}

}

#endif /* TRACE_SOURCE_ACCESSOR_H */

