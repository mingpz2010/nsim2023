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

#ifndef HASH_MURMUR3_H
#define HASH_MURMUR3_H

#include "hash-function.h"


namespace nsim2023
{

namespace Hash
{

namespace Function
{

/**
 *  Murmur3 hash function implementation
 *
 *  Adapted from http://code.google.com/p/smhasher/
 *
 *  MurmurHash3 was written by Austin Appleby, and is placed in the public
 *  domain. The author hereby disclaims copyright to this source code.

 *  Note - The x86 and x64 versions do _not_ produce the same results, as the
 *  algorithms are optimized for their respective platforms. You can still
 *  compile and run any of them on any platform, but your performance with the
 *  non-native version will be less than optimal.
 */
class Murmur3 : public Implementation
{
  public:

    Murmur3();

    uint32_t GetHash32(const char* buffer, const std::size_t size) override;

    uint64_t GetHash64(const char* buffer, const std::size_t size) override;

    void clear() override;

  private:

    enum seed
    {
        SEED = 0x8BADF00D // Ate bad food
    };

    uint32_t m_hash32;
    std::size_t m_size32;

    uint64_t m_hash64[2];
    std::size_t m_size64;

};

}

}

}

#endif /* HASH_MURMUR3_H */

