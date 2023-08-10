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

// Definition of empty logging macros and the NS_LOG_NOOP_INTERNAL macro.

#ifndef NSIM2023_LOG_MACROS_DISABLED_H
#define NSIM2023_LOG_MACROS_DISABLED_H

#ifndef NSIM2023_LOG_ENABLE

#define NS_LOG_NOOP_INTERNAL(msg)                                                                  \
    do                                                                                             \
        if (false)                                                                                 \
        {                                                                                          \
            std::clog << msg;                                                                      \
        }                                                                                          \
    while (false)

#define NS_LOG(level, msg) NS_LOG_NOOP_INTERNAL(msg)

#define NS_LOG_FUNCTION_NOARGS()


#define NS_LOG_NOOP_FUNC_INTERNAL(msg)                                                             \
    do                                                                                             \
        if (false)                                                                                 \
        {                                                                                          \
            nsim2023::ParameterLogger(std::clog) << msg;                                                \
        }                                                                                          \
    while (false)

#define NS_LOG_FUNCTION(parameters) NS_LOG_NOOP_FUNC_INTERNAL(parameters)

#define NS_LOG_UNCOND(msg) NS_LOG_NOOP_INTERNAL(msg)

#endif

#endif

