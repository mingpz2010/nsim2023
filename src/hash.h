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

#ifndef HASH_H
#define HASH_H

#include "assert.h"
#include "hash-fnv.h"
#include "hash-function.h"
#include "hash-murmur3.h"
#include "ptr.h"

#include <string>

namespace nsim2023
{

class Hasher
{
  public:
    /**
     * Constructor using the default implementation.
     */
    Hasher();

    Hasher(Ptr<Hash::Implementation> hp);

    uint32_t GetHash32(const char* buffer, const std::size_t size);
    /**
     * Compute 64-bit hash of a byte buffer.
     *
     */
    uint64_t GetHash64(const char* buffer, const std::size_t size);

    /**
     * Compute 32-bit hash of a string.
     *
     */
    uint32_t GetHash32(const std::string s);
    /**
     * Compute 64-bit hash of a string.
     *
     */
    uint64_t GetHash64(const std::string s);

    Hasher& clear();

  private:
    Ptr<Hash::Implementation> m_impl;
};

/*************************************************
 **  Global functions declarations
 ************************************************/
uint32_t Hash32(const char* buffer, const std::size_t size);

uint64_t Hash64(const char* buffer, const std::size_t size);

uint32_t Hash32(const std::string s);

// Compute 64-bit hash of a string, using the default hash function
uint64_t Hash64(const std::string s);

}

/*************************************************
 **  Inline implementations for rvo
 ************************************************/
namespace nsim2023
{

inline uint32_t
Hasher::GetHash32(const char* buffer, const std::size_t size)
{
    NS_ASSERT(m_impl);
    return m_impl->GetHash32(buffer, size);
}

inline uint64_t
Hasher::GetHash64(const char* buffer, const std::size_t size)
{
    NS_ASSERT(m_impl);
    return m_impl->GetHash64(buffer, size);
}

inline uint32_t
Hasher::GetHash32(const std::string s)
{
    NS_ASSERT(m_impl);
    return m_impl->GetHash32(s.c_str(), s.size());
}

inline uint64_t
Hasher::GetHash64(const std::string s)
{
    NS_ASSERT(m_impl);
    return m_impl->GetHash64(s.c_str(), s.size());
}


Hasher& GetStaticHash();

inline uint32_t
Hash32(const char* buffer, const std::size_t size)
{
    return GetStaticHash().GetHash32(buffer, size);
}

inline uint64_t
Hash64(const char* buffer, const std::size_t size)
{
    return GetStaticHash().GetHash64(buffer, size);
}

inline uint32_t
Hash32(const std::string s)
{
    return GetStaticHash().GetHash32(s);
}

inline uint64_t
Hash64(const std::string s)
{
    return GetStaticHash().GetHash64(s);
}

}

#endif /* HASH_H */

