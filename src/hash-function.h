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

#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H

#include "simple-ref-count.h"

#include <cstring> // memcpy

namespace nsim2023
{


namespace Hash
{


class Implementation : public SimpleRefCount<Implementation>
{
  public:
    /**
     * Compute 32-bit hash of a byte buffer
     */
    virtual uint32_t GetHash32(const char* buffer, const std::size_t size) = 0;
    /**
     * Compute 64-bit hash of a byte buffer.
     *
     * Default implementation returns 32-bit hash, with a warning.
     *
     */
    virtual uint64_t GetHash64(const char* buffer, const std::size_t size);
    /**
     * Restore initial state.
     */
    virtual void clear() = 0;

    /**
     * Constructor.
     */
    Implementation()
    {
    }

    /**
     * Destructor.
     */
    virtual ~Implementation()
    {
    }
}; // Hashfunction


typedef uint32_t (*Hash32Function_ptr)(const char*, const std::size_t);
typedef uint64_t (*Hash64Function_ptr)(const char*, const std::size_t);

namespace Function
{


class Hash32 : public Implementation
{
  public:
    Hash32(Hash32Function_ptr hp)
        : m_fp(hp)
    {
    }

    uint32_t GetHash32(const char* buffer, const std::size_t size) override
    {
        return (*m_fp)(buffer, size);
    }

    void clear() override
    {
    }

  private:
    Hash32Function_ptr m_fp;
};


class Hash64 : public Implementation
{
  public:

    Hash64(Hash64Function_ptr hp)
        : m_fp(hp)
    {
    }

    uint64_t GetHash64(const char* buffer, const std::size_t size) override
    {
        return (*m_fp)(buffer, size);
    }

    uint32_t GetHash32(const char* buffer, const std::size_t size) override
    {
        uint32_t hash32;
        uint64_t hash64 = GetHash64(buffer, size);

        memcpy(&hash32, &hash64, sizeof(hash32));
        return hash32;
    }

    void clear() override
    {
    }

  private:
    Hash64Function_ptr m_fp;
};

}

}

}

#endif

