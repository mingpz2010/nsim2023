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

#ifndef HASH_FNV_H
#define HASH_FNV_H

#include "hash-function.h"


namespace nsim2023
{

namespace Hash
{

namespace Function
{

/**
 *  This is the venerable Fowler-Noll-Vo hash, version 1A.  (See the
 *  <a href="http://isthe.com/chongo/tech/comp/fnv/">FNV page</a>.)
 *
 *  The implementation here is taken directly from the published FNV
 *  <a href="http://isthe.com/chongo/tech/comp/fnv/#FNV-reference-source">
 *  reference code</a>,
 *  with minor modifications to wrap into this class.  See the
 *  hash-fnv.cc file for details.
 *
 */
class Fnv1a : public Implementation
{
  public:
    /**
     * Constructor
     */
    Fnv1a();

    /**
     * Compute 32-bit hash of a byte buffer
     */
    uint32_t GetHash32(const char* buffer, const size_t size) override;

    /**
     * Compute 64-bit hash of a byte buffer.
     */
    uint64_t GetHash64(const char* buffer, const size_t size) override;

    void clear() override;

  private:
    /**
     * Seed value
     */
    enum seed
    {
        SEED = 0x8BADF00D
    };

    /** Cache last hash value, for incremental hashing. */
    uint32_t m_hash32;
    uint64_t m_hash64;

}; // class Fnv1a

} // namespace Function

} // namespace Hash

}

#endif /* HASH_FNV_H */

