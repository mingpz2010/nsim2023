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

#ifndef INT64X64_H
#define INT64X64_H

#include "core-config.h"

// Order is important here, as it determines which implementation
// will generate doxygen API docs.  This order mimics the
// selection logic in CMakeLists.txt, so we generate docs from the
// implementation actually chosen by the configuration.
#if defined(INT64X64_USE_128) && !defined(PYTHON_SCAN)
    #include "int64x64-128.h"
#elif defined(INT64X64_USE_CAIRO) && !defined(PYTHON_SCAN)
    #include "int64x64-cairo.h"
#elif defined(INT64X64_USE_DOUBLE) || defined(PYTHON_SCAN)
    #include "int64x64-double.h"
#endif

#include <iostream>


namespace nsim2023
{

// highprec High Precision Q64.64
inline int64x64_t
operator+(const int64x64_t& lhs, const int64x64_t& rhs)
{
    int64x64_t tmp = lhs;
    tmp += rhs;
    return tmp;
}


inline int64x64_t
operator-(const int64x64_t& lhs, const int64x64_t& rhs)
{
    int64x64_t tmp = lhs;
    tmp -= rhs;
    return tmp;
}


inline int64x64_t
operator*(const int64x64_t& lhs, const int64x64_t& rhs)
{
    int64x64_t tmp = lhs;
    tmp *= rhs;
    return tmp;
}


inline int64x64_t
operator/(const int64x64_t& lhs, const int64x64_t& rhs)
{
    int64x64_t tmp = lhs;
    tmp /= rhs;
    return tmp;
}


inline bool
operator!=(const int64x64_t& lhs, const int64x64_t& rhs)
{
    return !(lhs == rhs);
}


inline bool
operator<=(const int64x64_t& lhs, const int64x64_t& rhs)
{
    return !(lhs > rhs);
}


inline bool
operator>=(const int64x64_t& lhs, const int64x64_t& rhs)
{
    return !(lhs < rhs);
}


std::ostream& operator<<(std::ostream& os, const int64x64_t& value);

std::istream& operator>>(std::istream& is, int64x64_t& value);


inline int64x64_t
Abs(const int64x64_t& value)
{
    return (value < 0) ? -value : value;
}


inline int64x64_t
Min(const int64x64_t& a, const int64x64_t& b)
{
    return (a < b) ? a : b;
}


inline int64x64_t
Max(const int64x64_t& a, const int64x64_t& b)
{
    return (a > b) ? a : b;
}

}

#endif /* INT64X64_H */

