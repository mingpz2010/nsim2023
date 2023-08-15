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

#ifndef HEADER_H
#define HEADER_H

#include "buffer.h"
#include "chunk.h"

#include <stdint.h>

namespace nsim2023
{

/**
 *
 * Every Protocol header which needs to be inserted or removed
 * from a Packet instance must derive from this base class and
 * implement the pure virtual methods defined here.
 *
 * Sample code which shows how to create a new type of Header, and how to use it,
 * is shown in the sample file samples/main-packet-header.cc
 */
class Header : public Chunk
{
  public:

    static TypeId GetTypeId();
    ~Header() override;

    using Chunk::Deserialize;

    virtual uint32_t GetSerializedSize() const = 0;

    virtual void Serialize(Buffer::Iterator start) const = 0;

    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     * The data read is expected to
     * match bit-for-bit the representation of this header in real
     * networks.
     *
     * Note that data is not actually removed from the buffer to
     * which the iterator points.  Both Packet::RemoveHeader() and
     * Packet::PeekHeader() call Deserialize(), but only the RemoveHeader()
     * has additional statements to remove the header bytes from the
     * underlying buffer and associated metadata.
     */
    uint32_t Deserialize(Buffer::Iterator start) override = 0;

    /**
     * This method is used by Packet::Print to print the
     * content of a header as ascii data to a c++ output stream.
     * Although the header is free to format its output as it
     * wishes, it is recommended to follow a few rules to integrate
     * with the packet pretty printer: start with flags, small field
     * values located between a pair of parens. Values should be separated
     * by whitespace. Follow the parens with the important fields,
     * separated by whitespace.
     * i.e.: (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
     */
    void Print(std::ostream& os) const override = 0;
};

std::ostream& operator<<(std::ostream& os, const Header& header);

}

#endif /* HEADER_H */

