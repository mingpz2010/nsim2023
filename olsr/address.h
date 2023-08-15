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

#ifndef ADDRESS_H
#define ADDRESS_H

#include "attribute-helper.h"
#include "attribute.h"
#include "tag-buffer.h"

#include <ostream>
#include <stdint.h>

namespace nsim2023
{

/**
 *
 * Network Address abstractions, including MAC, IPv4 and IPv6.
 *
 */
/**
 *
 * This class is very similar in design and spirit to the BSD sockaddr
 * structure: they are both used to hold multiple types of addresses
 * together with the type of the address.
 *
 * A new address class defined by a user needs to:
 *   - allocate a type id with Address::Register
 *   - provide a method to convert his new address to an Address
 *     instance. This method is typically a member method named ConvertTo:
 *     Address MyAddress::ConvertTo () const;
 *   - provide a method to convert an Address instance back to
 *     an instance of his new address type. This method is typically
 *     a static member method of his address class named ConvertFrom:
 *     static MyAddress MyAddress::ConvertFrom (const Address &address);
 *   - the ConvertFrom method is expected to check that the type of the
 *     input Address instance is compatible with its own type.
 *
 * Typical code to create a new class type looks like:
 *
 * // this class represents addresses which are 2 bytes long.
 * class MyAddress
 * {
 * public:
 *   Address ConvertTo () const;
 *   static MyAddress ConvertFrom ();
 * private:
 *   static uint8_t GetType ();
 * };
 *
 * Address MyAddress::ConvertTo () const
 * {
 *   return Address (GetType (), m_buffer, 2);
 * }
 * MyAddress MyAddress::ConvertFrom (const Address &address)
 * {
 *   MyAddress ad;
 *   NS_ASSERT (address.CheckCompatible (GetType (), 2));
 *   address.CopyTo (ad.m_buffer, 2);
 *   return ad;
 * }
 * uint8_t MyAddress::GetType ()
 * {
 *   static uint8_t type = Address::Register ();
 *   return type;
 * }
 *
 */
class Address
{
  public:
    /**
     * The maximum size of a byte buffer which
     * can be stored in an Address instance.
     */
    enum MaxSize_e
    {
        MAX_SIZE = 20
    };

    /**
     * Create an invalid address
     */
    Address();
    /**
     * Create an address from a type and a buffer.
     *
     * This constructor is typically invoked from the conversion
     * functions of various address types when they have to
     * convert themselves to an Address instance.
     *
     */
    Address(uint8_t type, const uint8_t* buffer, uint8_t len);

    Address(const Address& address);

    Address& operator=(const Address& address);

    /**
     * An address is invalid if and only if it was created
     * through the default constructor and it was never
     * re-initialized.
     */
    bool IsInvalid() const;

    uint8_t GetLength() const;

    uint32_t CopyTo(uint8_t buffer[MAX_SIZE]) const;
    /**
     *
     * Copies the type to buffer[0], the length of the address internal buffer
     * to buffer[1] and copies the internal buffer starting at buffer[2].  len
     * must be at least the size of the internal buffer plus a byte for the type
     * and a byte for the length.
     */
    uint32_t CopyAllTo(uint8_t* buffer, uint8_t len) const;
    /**
     *
     * Copy the address bytes from buffer into to the internal buffer of this
     * address instance.
     */
    uint32_t CopyFrom(const uint8_t* buffer, uint8_t len);
    /**
     *
     * The inverse of CopyAllTo().
     */
    uint32_t CopyAllFrom(const uint8_t* buffer, uint8_t len);
    /**
     * \param type a type id as returned by Address::Register
     * \param len the length associated to this type id.
     *
     * \returns true if the type of the address stored internally
     * is compatible with the requested type, false otherwise.
     */
    bool CheckCompatible(uint8_t type, uint8_t len) const;
    /**
     * \param type a type id as returned by Address::Register
     * \returns true if the type of the address stored internally
     * is compatible with the requested type, false otherwise.
     *
     * This method checks that the types are _exactly_ equal.
     * This method is really used only by the PacketSocketAddress
     * and there is little point in using it otherwise so,
     * you have been warned: DO NOT USE THIS METHOD.
     */
    bool IsMatchingType(uint8_t type) const;
    /**
     * Allocate a new type id for a new type of address.
     * \returns a new type id.
     */
    static uint8_t Register();
    /**
     * Get the number of bytes needed to serialize the underlying Address
     * Typically, this is GetLength () + 2
     *
     * \returns the number of bytes required for an Address in serialized form
     */
    uint32_t GetSerializedSize() const;
    /**
     * Serialize this address in host byte order to a byte buffer
     *
     * \param buffer output buffer that gets written with this Address
     */
    void Serialize(TagBuffer buffer) const;
    /**
     * \param buffer buffer to read address from
     *
     * The input address buffer is expected to be in host byte order format.
     */
    void Deserialize(TagBuffer buffer);

  private:
    /**
     * \brief Equal to operator.
     *
     * \param a the first operand
     * \param b the first operand
     * \returns true if the operands are equal
     */
    friend bool operator==(const Address& a, const Address& b);

    /**
     * \brief Not equal to operator.
     *
     * \param a the first operand
     * \param b the first operand
     * \returns true if the operands are not equal
     */
    friend bool operator!=(const Address& a, const Address& b);

    /**
     * \brief Less than operator.
     *
     * \param a the first operand
     * \param b the first operand
     * \returns true if the operand a is less than operand b
     */
    friend bool operator<(const Address& a, const Address& b);

    /**
     * \brief Stream insertion operator.
     *
     * \param os the stream
     * \param address the address
     * \returns a reference to the stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Address& address);

    /**
     * \brief Stream extraction operator.
     *
     * \param is the stream
     * \param address the address
     * \returns a reference to the stream
     */
    friend std::istream& operator>>(std::istream& is, Address& address);

    uint8_t m_type;           //!< Type of the address
    uint8_t m_len;            //!< Length of the address
    uint8_t m_data[MAX_SIZE]; //!< The address value
};

ATTRIBUTE_HELPER_HEADER(Address);

bool operator==(const Address& a, const Address& b);
bool operator!=(const Address& a, const Address& b);
bool operator<(const Address& a, const Address& b);
std::ostream& operator<<(std::ostream& os, const Address& address);
std::istream& operator>>(std::istream& is, Address& address);

}

#endif /* ADDRESS_H */

