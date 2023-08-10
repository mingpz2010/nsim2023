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

#ifndef TYPE_NAME_H
#define TYPE_NAME_H

#include "fatal-error.h"

#include <string>


namespace nsim2023
{

template <typename T>
std::string
TypeNameGet()
{
    NS_FATAL_ERROR("Type name not defined.");
    return "unknown";
}


#define TYPENAMEGET_DEFINE(T)                                          \
    template <>                                                                                     \
    inline std::string TypeNameGet<T>()                                   \
    {                                                                                                            \
        return #T;                                                                                     \
    }


TYPENAMEGET_DEFINE(bool);
TYPENAMEGET_DEFINE(int8_t);
TYPENAMEGET_DEFINE(int16_t);
TYPENAMEGET_DEFINE(int32_t);
TYPENAMEGET_DEFINE(int64_t);
TYPENAMEGET_DEFINE(uint8_t);
TYPENAMEGET_DEFINE(uint16_t);
TYPENAMEGET_DEFINE(uint32_t);
TYPENAMEGET_DEFINE(uint64_t);
TYPENAMEGET_DEFINE(float);
TYPENAMEGET_DEFINE(double);
TYPENAMEGET_DEFINE(long double);

}

#endif /* TYPE_NAME_H */

