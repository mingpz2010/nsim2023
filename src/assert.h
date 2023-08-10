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

#ifndef NSIM2023_ASSERT_H
#define NSIM2023_ASSERT_H


#ifdef NSIM2023_ASSERT_ENABLE

#include "fatal-error.h"

#include <iostream>

#define NS_ASSERT(condition)                                                                       \
    do                                                                                             \
    {                                                                                              \
        if (!(condition))                                                                          \
        {                                                                                          \
            std::cerr << "assert failed. cond=\"" << #condition << "\", ";                         \
            NS_FATAL_ERROR_NO_MSG();                                                               \
        }                                                                                          \
    } while (false)


#define NS_ASSERT_MSG(condition, message)                                                          \
    do                                                                                             \
    {                                                                                              \
        if (!(condition))                                                                          \
        {                                                                                          \
            std::cerr << "assert failed. cond=\"" << #condition << "\", ";                         \
            NS_FATAL_ERROR(message);                                                               \
        }                                                                                          \
    } while (false)

#else

#define NS_ASSERT(condition)                                                                       \
    do                                                                                             \
    {                                                                                              \
        (void)sizeof(condition);                                                                   \
    } while (false)

#define NS_ASSERT_MSG(condition, message)                                                          \
    do                                                                                             \
    {                                                                                              \
        (void)sizeof(condition);                                                                   \
    } while (false)

#endif

#endif /* ASSERT_H */

