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

#ifndef NS_DOUBLE_H
#define NS_DOUBLE_H

#include "attribute-helper.h"
#include "attribute.h"

#include <limits>
#include <stdint.h>


namespace nsim2023
{

//  Additional docs for class DoubleValue:
/**
 * This class can be used to hold variables of floating point type
 * such as 'double' or 'float'. The internal format is 'double'.
 */
ATTRIBUTE_VALUE_DEFINE_WITH_NAME(double, Double);
ATTRIBUTE_ACCESSOR_DEFINE(Double);

template <typename T>
Ptr<const AttributeChecker> MakeDoubleChecker();

/**
 * Make a checker with a minimum value.
 *
 * The minimum value is included in the allowed range.
 */
template <typename T>
Ptr<const AttributeChecker> MakeDoubleChecker(double min);

/**
 * Make a checker with a minimum and a maximum value.
 *
 * The minimum and maximum values are included in the allowed range.
 */
template <typename T>
Ptr<const AttributeChecker> MakeDoubleChecker(double min, double max);

}

/***************************************************************
 *  Implementation of the templates declared above.
 ***************************************************************/
#include "type-name.h"

namespace nsim2023
{

namespace internal
{

Ptr<const AttributeChecker> MakeDoubleChecker(double min, double max, std::string name);

} // namespace internal

template <typename T>
Ptr<const AttributeChecker>
MakeDoubleChecker()
{
    return internal::MakeDoubleChecker(-std::numeric_limits<T>::max(),
                                       std::numeric_limits<T>::max(),
                                       TypeNameGet<T>());
}

template <typename T>
Ptr<const AttributeChecker>
MakeDoubleChecker(double min)
{
    return internal::MakeDoubleChecker(min, std::numeric_limits<T>::max(), TypeNameGet<T>());
}

template <typename T>
Ptr<const AttributeChecker>
MakeDoubleChecker(double min, double max)
{
    return internal::MakeDoubleChecker(min, max, TypeNameGet<T>());
}

}

#endif /* NS_DOUBLE_H */

