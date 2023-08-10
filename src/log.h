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

#ifndef NSIM2023_LOG_H
#define NSIM2023_LOG_H

#include "log-macros-disabled.h"
#include "log-macros-enabled.h"
#include "node-printer.h"
#include "time-printer.h"

#include <iostream>
#include <map>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <vector>


// Debug message logging
namespace nsim2023
{

/**
 *  Logging severity classes and levels.
 */
enum LogLevel
{
    LOG_NONE = 0x00000000, //!< No logging.

    LOG_ERROR = 0x00000001,       //!< Serious error messages only.
    LOG_LEVEL_ERROR = 0x00000001, //!< LOG_ERROR and above.

    LOG_WARN = 0x00000002,       //!< Warning messages.
    LOG_LEVEL_WARN = 0x00000003, //!< LOG_WARN and above.

    LOG_DEBUG = 0x00000004,       //!< Rare ad-hoc debug messages.
    LOG_LEVEL_DEBUG = 0x00000007, //!< LOG_DEBUG and above.

    LOG_INFO = 0x00000008,       //!< Informational messages (e.g., banners).
    LOG_LEVEL_INFO = 0x0000000f, //!< LOG_INFO and above.

    LOG_FUNCTION = 0x00000010,       //!< Function tracing.
    LOG_LEVEL_FUNCTION = 0x0000001f, //!< LOG_FUNCTION and above.

    LOG_LOGIC = 0x00000020,       //!< Control flow tracing within functions.
    LOG_LEVEL_LOGIC = 0x0000003f, //!< LOG_LOGIC and above.

    LOG_ALL = 0x0fffffff,    //!< Print everything.
    LOG_LEVEL_ALL = LOG_ALL, //!< Print everything.

    LOG_PREFIX_FUNC = 0x80000000,  //!< Prefix all trace prints with function.
    LOG_PREFIX_TIME = 0x40000000,  //!< Prefix all trace prints with simulation time.
    LOG_PREFIX_NODE = 0x20000000,  //!< Prefix all trace prints with simulation node.
    LOG_PREFIX_LEVEL = 0x10000000, //!< Prefix all trace prints with log level (severity).
    LOG_PREFIX_ALL = 0xf0000000    //!< All prefixes.
};


// Enable the logging output associated with that log component
void LogComponentEnable(const char* name, enum LogLevel level);


// Enable the logging output for all registered log components
void LogComponentEnableAll(enum LogLevel level);


// Disable the logging output associated with that log component
void LogComponentDisable(const char* name, enum LogLevel level);


//  Disable all logging for all components
void LogComponentDisableAll(enum LogLevel level);

}


#define NS_LOG_COMPONENT_DEFINE(name)                                                              \
    static nsim2023::LogComponent g_log = nsim2023::LogComponent(name, __FILE__)


#define NS_LOG_COMPONENT_DEFINE_MASK(name, mask)                                                   \
    static nsim2023::LogComponent g_log = nsim2023::LogComponent(name, __FILE__, mask)


#define NS_LOG_TEMPLATE_DECLARE LogComponent& g_log


#define NS_LOG_TEMPLATE_DEFINE(name) g_log(GetLogComponent(name))


#define NS_LOG_STATIC_TEMPLATE_DEFINE(name)                                                        \
    static LogComponent& g_log [[maybe_unused]] = GetLogComponent(name)


#define NS_LOG_ERROR(msg) NS_LOG(nsim2023::LOG_ERROR, msg)


#define NS_LOG_WARN(msg) NS_LOG(nsim2023::LOG_WARN, msg)


#define NS_LOG_DEBUG(msg) NS_LOG(nsim2023::LOG_DEBUG, msg)


#define NS_LOG_INFO(msg) NS_LOG(nsim2023::LOG_INFO, msg)


#define NS_LOG_LOGIC(msg) NS_LOG(nsim2023::LOG_LOGIC, msg)


namespace nsim2023
{


void LogComponentPrintList();


void LogSetTimePrinter(TimePrinter lp);


TimePrinter LogGetTimePrinter();


void LogSetNodePrinter(NodePrinter np);


NodePrinter LogGetNodePrinter();


// A single log component configuration
class LogComponent
{
  public:

    LogComponent(const std::string& name,
                 const std::string& file,
                 const enum LogLevel mask = LOG_NONE);
 
    bool IsEnabled(const enum LogLevel level) const;

    bool IsNoneEnabled() const;

    void Enable(const enum LogLevel level);

    void Disable(const enum LogLevel level);

    const char* Name() const;

    std::string File() const;

    static std::string GetLevelLabel(const enum LogLevel level);

    void SetMask(const enum LogLevel level);

    typedef std::map<std::string, LogComponent*> ComponentList;

    static ComponentList* GetComponentList();

  private:
    /**
     * Parse the `NS_LOG` environment variable for options relating to this
     * LogComponent.
     */
    void EnvVarCheck();

    int32_t m_levels;   //!< Enabled LogLevels.
    int32_t m_mask;     //!< Blocked LogLevels.
    std::string m_name; //!< LogComponent name.
    std::string m_file; //!< File defining this LogComponent.

}; // class LogComponent



LogComponent& GetLogComponent(const std::string name);


class ParameterLogger
{
  public:

    ParameterLogger(std::ostream& os);


    template <typename T, typename U = std::enable_if_t<std::is_arithmetic_v<T>>>
    ParameterLogger& operator<<(T param);


    template <typename T, typename U = std::enable_if_t<!std::is_arithmetic_v<T>>>
    ParameterLogger& operator<<(const T& param);


    template <typename T>
    ParameterLogger& operator<<(const std::vector<T>& vector);


    ParameterLogger& operator<<(const char* param);

  private:
    /** Add `, ` before every parameter after the first. */
    void CommaRest();

    bool m_first{true}; //!< First argument flag, doesn't get `, `.
    std::ostream& m_os; //!< Underlying output stream.
};

template <typename T, typename U>
ParameterLogger&
ParameterLogger::operator<<(T param)
{
    CommaRest();
    m_os << param;
    return *this;
}

template <typename T, typename U>
ParameterLogger&
ParameterLogger::operator<<(const T& param)
{
    CommaRest();
    m_os << param;
    return *this;
}

template <typename T>
ParameterLogger&
ParameterLogger::operator<<(const std::vector<T>& vector)
{
    for (const auto& i : vector)
    {
        *this << i;
    }
    return *this;
}


template <>
ParameterLogger& ParameterLogger::operator<< <std::string>(const std::string& param);


template <>
ParameterLogger& ParameterLogger::operator<< <int8_t>(const int8_t param);


template <>
ParameterLogger& ParameterLogger::operator<< <uint8_t>(const uint8_t param);

}

#endif
