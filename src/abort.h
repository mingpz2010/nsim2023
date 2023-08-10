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

#ifndef NSIM2023_ABORT_H
#define NSIM2023_ABORT_H

#include "fatal-error.h"


#define NS_ABORT_MSG(msg)                                                                          \
    do                                                                                             \
    {                                                                                              \
        std::cerr << "aborted. ";                                                                  \
        NS_FATAL_ERROR(msg);                                                                       \
    } while (false)



#define NS_ABORT_IF(cond)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if (cond)                                                                                  \
        {                                                                                          \
            std::cerr << "aborted. cond=\"" << #cond << ", ";                                      \
            NS_FATAL_ERROR_NO_MSG();                                                               \
        }                                                                                          \
    } while (false)



#define NS_ABORT_MSG_IF(cond, msg)                                                                 \
    do                                                                                             \
    {                                                                                              \
        if (cond)                                                                                  \
        {                                                                                          \
            std::cerr << "aborted. cond=\"" << #cond << "\", ";                                    \
            NS_FATAL_ERROR(msg);                                                                   \
        }                                                                                          \
    } while (false)



#define NS_ABORT_UNLESS(cond) NS_ABORT_IF(!(cond))


#define NS_ABORT_MSG_UNLESS(cond, msg) NS_ABORT_MSG_IF(!(cond), msg)


#endif

