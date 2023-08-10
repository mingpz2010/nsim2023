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

#ifndef UINTEGER_H
#define UINTEGER_H

#include "attribute-helper.h"
#include "attribute.h"

#include <limits>
#include <stdint.h>


namespace nsim2023
{


/**
 * Hold an unsigned integer type.
 *
 * This class can be used to hold variables of unsigned integer
 * type such as uint8_t, uint16_t, uint32_t, uint64_t, or,
 * unsigned int, etc.
 */
ATTRIBUTE_VALUE_DEFINE_WITH_NAME(uint64_t, Uinteger);
ATTRIBUTE_ACCESSOR_DEFINE(Uinteger);

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker();

/**
 * Make a checker with a minimum value.
 *
 * The minimum value is included in the allowed range.
 */
template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker(uint64_t min);

/**
 * Make a checker with a minimum and a maximum value.
 *
 * The minimum and maximum values are included in the allowed range.
 */
template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker(uint64_t min, uint64_t max);

}

/***************************************************************
 *  Implementation of the templates declared above.
 ***************************************************************/

#include "type-name.h"

namespace nsim2023
{

namespace internal
{

Ptr<const AttributeChecker> MakeUintegerChecker(uint64_t min, uint64_t max, std::string name);

}

template <typename T>
Ptr<const AttributeChecker>
MakeUintegerChecker()
{
    return internal::MakeUintegerChecker(std::numeric_limits<T>::min(),
                                         std::numeric_limits<T>::max(),
                                         TypeNameGet<T>());
}

template <typename T>
Ptr<const AttributeChecker>
MakeUintegerChecker(uint64_t min)
{
    return internal::MakeUintegerChecker(min, std::numeric_limits<T>::max(), TypeNameGet<T>());
}

template <typename T>
Ptr<const AttributeChecker>
MakeUintegerChecker(uint64_t min, uint64_t max)
{
    return internal::MakeUintegerChecker(min, max, TypeNameGet<T>());
}

}

#endif /* UINTEGER_H */

