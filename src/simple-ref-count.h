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

#ifndef SIMPLE_REF_COUNT_H
#define SIMPLE_REF_COUNT_H

#include "assert.h"
#include "default-deleter.h"

#include <limits>
#include <stdint.h>


namespace nsim2023
{


class empty
{
};


template <typename T, typename PARENT = empty, typename DELETER = DefaultDeleter<T>>
class SimpleRefCount : public PARENT
{
  public:
    /** Default constructor.  */
    SimpleRefCount()
        : m_count(1)
    {
    }

    SimpleRefCount(const SimpleRefCount& o [[maybe_unused]])
        : m_count(1)
    {
    }

    SimpleRefCount& operator=(const SimpleRefCount& o [[maybe_unused]])
    {
        return *this;
    }

    inline void Ref() const
    {
        NS_ASSERT(m_count < std::numeric_limits<uint32_t>::max());
        m_count++;
    }

    inline void Unref() const
    {
        m_count--;
        if (m_count == 0)
        {
            DELETER::Delete(static_cast<T*>(const_cast<SimpleRefCount*>(this)));
        }
    }

    inline uint32_t GetReferenceCount() const
    {
        return m_count;
    }

  private:
    mutable uint32_t m_count;
};

}

#endif /* SIMPLE_REF_COUNT_H */

