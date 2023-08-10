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

#include "core-config.h"

#if !defined(INT64X64_128_H) && defined(INT64X64_USE_128) && !defined(PYTHON_SCAN)

#define INT64X64_128_H

#include <cmath> // pow
#include <stdint.h>

#if defined(HAVE___UINT128_T) && !defined(HAVE_UINT128_T)

typedef __uint128_t uint128_t;
typedef __int128_t int128_t;

#endif


namespace nsim2023
{

/**
 * The implementation documented here is based on native 128-bit integers.
 */
class int64x64_t
{
    /// uint128_t high bit (sign bit).
    static const uint128_t HP128_MASK_HI_BIT = (((int128_t)1) << 127);
    /// Mask for fraction part.
    static const uint64_t HP_MASK_LO = 0xffffffffffffffffULL;
    /// Mask for sign + integer part.
    static const uint64_t HP_MASK_HI = ~HP_MASK_LO;

#define HP_MAX_64 (std::pow(2.0L, 64))

  public:
    /**
     * Type tag for the underlying implementation.
     *
     * A few testcases are are sensitive to implementation,
     * specifically the double implementation.  To handle this,
     * we expose the underlying implementation type here.
     */
    enum impl_type
    {
        int128_impl,
        cairo_impl,
        ld_impl,
    };

    /// Type tag for this implementation.
    static const enum impl_type implementation = int128_impl;

    /// Default constructor.
    inline int64x64_t()
        : _v(0)
    {
    }

    inline int64x64_t(const double value)
    {
        const int64x64_t tmp((long double)value);
        _v = tmp._v;
    }

    inline int64x64_t(const long double value)
    {
        const bool negative = value < 0;
        const long double v = negative ? -value : value;

        long double fhi;
        long double flo = std::modf(v, &fhi);
        // Add 0.5 to round, which improves the last count
        // This breaks these tests:
        //   TestSuite devices-mesh-dot11s-regression
        //   TestSuite devices-mesh-flame-regression
        //   TestSuite routing-aodv-regression
        //   TestSuite routing-olsr-regression
        // Setting round = 0; breaks:
        //   TestSuite int64x64
        const long double round = 0.5;
        flo = flo * HP_MAX_64 + round;
        int128_t hi = fhi;
        const uint64_t lo = flo;
        if (flo >= HP_MAX_64)
        {
            // conversion to uint64 rolled over
            ++hi;
        }
        _v = hi << 64;
        _v |= lo;
        _v = negative ? -_v : _v;
    }

    inline int64x64_t(const int v)
        : _v(v)
    {
        _v <<= 64;
    }

    inline int64x64_t(const long int v)
        : _v(v)
    {
        _v <<= 64;
    }

    inline int64x64_t(const long long int v)
        : _v(v)
    {
        _v <<= 64;
    }

    inline int64x64_t(const unsigned int v)
        : _v(v)
    {
        _v <<= 64;
    }

    inline int64x64_t(const unsigned long int v)
        : _v(v)
    {
        _v <<= 64;
    }

    inline int64x64_t(const unsigned long long int v)
        : _v(v)
    {
        _v <<= 64;
    }

    inline int64x64_t(const int128_t v)
        : _v(v)
    {
    }

    explicit inline int64x64_t(const int64_t hi, const uint64_t lo)
    {
        _v = (int128_t)hi << 64;
        _v |= lo;
    }

    inline int64x64_t(const int64x64_t& o)
        : _v(o._v)
    {
    }

    inline int64x64_t& operator=(const int64x64_t& o)
    {
        _v = o._v;
        return *this;
    }

    inline explicit operator bool() const
    {
        return (_v != 0);
    }

    /**
     * Get this value as a double.
     */
    inline double GetDouble() const
    {
        const bool negative = _v < 0;
        const uint128_t value = negative ? -_v : _v;
        const long double fhi = value >> 64;
        const long double flo = (value & HP_MASK_LO) / HP_MAX_64;
        long double retval = fhi;
        retval += flo;
        retval = negative ? -retval : retval;
        return retval;
    }

    /**
     * Get the integer portion.
     */
    inline int64_t GetHigh() const
    {
        const int128_t retval = _v >> 64;
        return retval;
    }

    /**
     * Get the fractional portion of this value, unscaled.
     */
    inline uint64_t GetLow() const
    {
        const uint128_t retval = _v & HP_MASK_LO;
        return retval;
    }

    /**
     * Truncate to an integer.
     * Truncation is always toward zero,
     */
    int64_t GetInt() const
    {
        const bool negative = _v < 0;
        const uint128_t value = negative ? -_v : _v;
        int64_t retval = value >> 64;
        retval = negative ? -retval : retval;
        return retval;
    }

    /**
     * Round to the nearest int.
     * Similar to std::round this rounds halfway cases away from zero,
     * regardless of the current (floating) rounding mode.
     */
    int64_t Round() const
    {
        const bool negative = _v < 0;
        int64x64_t value = (negative ? -(*this) : *this);
        const int64x64_t half(0, 1LL << 63);
        value += half;
        int64_t retval = value.GetHigh();
        retval = negative ? -retval : retval;
        return retval;
    }

    /**
     * Multiply this value by a Q0.128 value, presumably representing an inverse,
     * completing a division operation.
     */
    void MulByInvert(const int64x64_t& o);

    /**
     * Compute the inverse of an integer value.
     *
     * Ordinary division by an integer would be limited to 64 bits of precision.
     * Instead, we multiply by the 128-bit inverse of the divisor.
     * This function computes the inverse to 128-bit precision.
     * MulByInvert() then completes the division.
     *
     * (Really this should be a separate type representing Q0.128.)
     */
    static int64x64_t Invert(const uint64_t v);

  private:

    friend inline bool operator==(const int64x64_t& lhs, const int64x64_t& rhs)
    {
        return lhs._v == rhs._v;
    }

    friend inline bool operator<(const int64x64_t& lhs, const int64x64_t& rhs)
    {
        return lhs._v < rhs._v;
    }

    friend inline bool operator>(const int64x64_t& lhs, const int64x64_t& rhs)
    {
        return lhs._v > rhs._v;
    }

    friend inline int64x64_t& operator+=(int64x64_t& lhs, const int64x64_t& rhs)
    {
        lhs._v += rhs._v;
        return lhs;
    }

    friend inline int64x64_t& operator-=(int64x64_t& lhs, const int64x64_t& rhs)
    {
        lhs._v -= rhs._v;
        return lhs;
    }

    friend inline int64x64_t& operator*=(int64x64_t& lhs, const int64x64_t& rhs)
    {
        lhs.Mul(rhs);
        return lhs;
    }

    friend inline int64x64_t& operator/=(int64x64_t& lhs, const int64x64_t& rhs)
    {
        lhs.Div(rhs);
        return lhs;
    }

    friend inline int64x64_t operator+(const int64x64_t& lhs)
    {
        return lhs;
    }

    friend inline int64x64_t operator-(const int64x64_t& lhs)
    {
        return int64x64_t(-lhs._v);
    }

    friend inline int64x64_t operator!(const int64x64_t& lhs)
    {
        return int64x64_t(!lhs._v);
    }

    void Mul(const int64x64_t& o);

    void Div(const int64x64_t& o);
    /**
     * Unsigned multiplication of Q64.64 values.
     *
     * Mathematically this should produce a Q128.128 value;
     * we keep the central 128 bits, representing the Q64.64 result.
     * We assert on integer overflow beyond the 64-bit integer portion.
     */
    static uint128_t Umul(const uint128_t a, const uint128_t b);

    static uint128_t Udiv(const uint128_t a, const uint128_t b);

    static uint128_t UmulByInvert(const uint128_t a, const uint128_t b);

    int128_t _v;

}; // class int64x64_t

}

#endif /* INT64X64_128_H */

