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

#ifndef NSIM2023_FATAL_ERROR_H
#define NSIM2023_FATAL_ERROR_H

#include "fatal-impl.h"
#include "log.h"

#include <cstdlib>
#include <exception>
#include <iostream>


#define NS_FATAL_ERROR_IMPL_NO_MSG(fatal)                                                          \
    do                                                                                             \
    {                                                                                              \
        NS_LOG_APPEND_TIME_PREFIX_IMPL;                                                            \
        NS_LOG_APPEND_NODE_PREFIX_IMPL;                                                            \
        std::cerr << "file=" << __FILE__ << ", line=" << __LINE__ << std::endl;                    \
        ::nsim2023::FatalImpl::FlushStreams();                                                          \
        if (fatal)                                                                                 \
            std::terminate();                                                                      \
    } while (false)


#define NS_FATAL_ERROR_IMPL(msg, fatal)                                                            \
    do                                                                                             \
    {                                                                                              \
        std::cerr << "msg=\"" << msg << "\", ";                                                    \
        NS_FATAL_ERROR_IMPL_NO_MSG(fatal);                                                         \
    } while (false)


#define NS_FATAL_ERROR_NO_MSG() NS_FATAL_ERROR_IMPL_NO_MSG(true)



#define NS_FATAL_ERROR_NO_MSG_CONT() NS_FATAL_ERROR_IMPL_NO_MSG(false)



#define NS_FATAL_ERROR(msg) NS_FATAL_ERROR_IMPL(msg, true)



#define NS_FATAL_ERROR_CONT(msg) NS_FATAL_ERROR_IMPL(msg, false)


#endif /* FATAL_ERROR_H */

