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

#ifndef NSIM2023_LOG_MACROS_ENABLED_H
#define NSIM2023_LOG_MACROS_ENABLED_H


#define NS_LOG_APPEND_TIME_PREFIX_IMPL                                                             \
    do                                                                                             \
    {                                                                                              \
        nsim2023::TimePrinter printer = nsim2023::LogGetTimePrinter();                                       \
        if (printer != 0)                                                                          \
        {                                                                                          \
            (*printer)(std::clog);                                                                 \
            std::clog << " ";                                                                      \
        }                                                                                          \
    } while (false)


#define NS_LOG_APPEND_NODE_PREFIX_IMPL                                                             \
    do                                                                                             \
    {                                                                                              \
        nsim2023::NodePrinter printer = nsim2023::LogGetNodePrinter();                                       \
        if (printer != 0)                                                                          \
        {                                                                                          \
            (*printer)(std::clog);                                                                 \
            std::clog << " ";                                                                      \
        }                                                                                          \
    } while (false)

#ifdef NSIM2023_LOG_ENABLE

#define NS_LOG_APPEND_TIME_PREFIX                                                                  \
    if (g_log.IsEnabled(nsim2023::LOG_PREFIX_TIME))                                                     \
    {                                                                                              \
        NS_LOG_APPEND_TIME_PREFIX_IMPL;                                                            \
    }


#define NS_LOG_APPEND_NODE_PREFIX                                                                  \
    if (g_log.IsEnabled(nsim2023::LOG_PREFIX_NODE))                                                     \
    {                                                                                              \
        NS_LOG_APPEND_NODE_PREFIX_IMPL;                                                            \
    }


#define NS_LOG_APPEND_FUNC_PREFIX                                                                  \
    if (g_log.IsEnabled(nsim2023::LOG_PREFIX_FUNC))                                                     \
    {                                                                                              \
        std::clog << g_log.Name() << ":" << __FUNCTION__ << "(): ";                                \
    }


#define NS_LOG_APPEND_LEVEL_PREFIX(level)                                                          \
    if (g_log.IsEnabled(nsim2023::LOG_PREFIX_LEVEL))                                                    \
    {                                                                                              \
        std::clog << "[" << g_log.GetLevelLabel(level) << "] ";                                    \
    }

#ifndef NS_LOG_APPEND_CONTEXT
    #define NS_LOG_APPEND_CONTEXT
#endif /* NS_LOG_APPEND_CONTEXT */

#ifndef NS_LOG_CONDITION
    #define NS_LOG_CONDITION
#endif


#define NS_LOG(level, msg)                                                                         \
    NS_LOG_CONDITION                                                                               \
    do                                                                                             \
    {                                                                                              \
        if (g_log.IsEnabled(level))                                                                \
        {                                                                                          \
            NS_LOG_APPEND_TIME_PREFIX;                                                             \
            NS_LOG_APPEND_NODE_PREFIX;                                                             \
            NS_LOG_APPEND_CONTEXT;                                                                 \
            NS_LOG_APPEND_FUNC_PREFIX;                                                             \
            NS_LOG_APPEND_LEVEL_PREFIX(level);                                                     \
            std::clog << msg << std::endl;                                                         \
        }                                                                                          \
    } while (false)


// Output the name of the function
#define NS_LOG_FUNCTION_NOARGS()                                                                   \
    NS_LOG_CONDITION                                                                               \
    do                                                                                             \
    {                                                                                              \
        if (g_log.IsEnabled(nsim2023::LOG_FUNCTION))                                                    \
        {                                                                                          \
            NS_LOG_APPEND_TIME_PREFIX;                                                             \
            NS_LOG_APPEND_NODE_PREFIX;                                                             \
            NS_LOG_APPEND_CONTEXT;                                                                 \
            std::clog << g_log.Name() << ":" << __FUNCTION__ << "()" << std::endl;                 \
        }                                                                                          \
    } while (false)



#define NS_LOG_FUNCTION(parameters)                                                                \
    NS_LOG_CONDITION                                                                               \
    do                                                                                             \
    {                                                                                              \
        if (g_log.IsEnabled(nsim2023::LOG_FUNCTION))                                                    \
        {                                                                                          \
            NS_LOG_APPEND_TIME_PREFIX;                                                             \
            NS_LOG_APPEND_NODE_PREFIX;                                                             \
            NS_LOG_APPEND_CONTEXT;                                                                 \
            std::clog << g_log.Name() << ":" << __FUNCTION__ << "(";                               \
            nsim2023::ParameterLogger(std::clog) << parameters;                                         \
            std::clog << ")" << std::endl;                                                         \
        }                                                                                          \
    } while (false)


#define NS_LOG_UNCOND(msg)                                                                         \
    NS_LOG_CONDITION                                                                               \
    do                                                                                             \
    {                                                                                              \
        std::clog << msg << std::endl;                                                             \
    } while (false)

#endif 

#endif 

