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

#ifndef CHUNK_H
#define CHUNK_H

#include "buffer.h"

#include "object-base.h"

namespace nsim2023
{

/**
 * packet :
 * abstract base class for Header and Trailer
 */
class Chunk : public ObjectBase
{
  public:

    static TypeId GetTypeId();

    /**
     * Deserialize the object from a buffer iterator
     *
     * This version of Deserialize can be used when the Chunk has a fixed
     * size.  It should not be called for variable-sized Chunk derived types
     * (but must be implemented, for historical reasons).
     *
     */
    virtual uint32_t Deserialize(Buffer::Iterator start) = 0;

    /**
     *
     * This version of Deserialize must be used when the Chunk has a variable
     * size, because the bounds of the Chunk may not be known at the point
     * of deserialization (e.g. a sequence of TLV fields).
     *
     * The size of the chunk should be start.GetDistanceFrom (end);
     *
     */
    virtual uint32_t Deserialize(Buffer::Iterator start, Buffer::Iterator end);

    virtual void Print(std::ostream& os) const = 0;
};

}

#endif /* CHUNK_H */

