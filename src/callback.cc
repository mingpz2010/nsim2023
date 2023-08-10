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

#include "callback.h"

#include "log.h"


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("Callback");

CallbackValue::CallbackValue()
    : m_value()
{
    NS_LOG_FUNCTION(this);
}

CallbackValue::CallbackValue(const CallbackBase& base)
    : m_value(base)
{
}

CallbackValue::~CallbackValue()
{
    NS_LOG_FUNCTION(this);
}

void
CallbackValue::Set(CallbackBase base)
{
    NS_LOG_FUNCTION(&base);

    m_value = base;
}

Ptr<AttributeValue>
CallbackValue::Copy() const
{
    NS_LOG_FUNCTION(this);
    return Create<CallbackValue>(m_value);
}

std::string
CallbackValue::SerializeToString(Ptr<const AttributeChecker> checker) const
{
    NS_LOG_FUNCTION(this << checker);
    std::ostringstream oss;
    oss << PeekPointer(m_value.GetImpl());
    return oss.str();
}

bool
CallbackValue::DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker)
{
    NS_LOG_FUNCTION(this << value << checker);
    return false;
}

ATTRIBUTE_CHECKER_IMPLEMENT(Callback);

}

#if (__GNUC__ >= 3)

#include <cstdlib>
#include <cxxabi.h>

namespace nsim2023
{

std::string
CallbackImplBase::Demangle(const std::string& mangled)
{
    NS_LOG_FUNCTION(mangled);

    int status;
    char* demangled = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);

    std::string ret;
    if (status == 0)
    {
        NS_ASSERT(demangled);
        ret = demangled;
    }
    else if (status == -1)
    {
        NS_LOG_UNCOND("Callback demangling failed: Memory allocation failure occurred.");
        ret = mangled;
    }
    else if (status == -2)
    {
        NS_LOG_UNCOND("Callback demangling failed: Mangled name is not a valid under the C++ ABI "
                      "mangling rules.");
        ret = mangled;
    }
    else if (status == -3)
    {
        NS_LOG_UNCOND("Callback demangling failed: One of the arguments is invalid.");
        ret = mangled;
    }
    else
    {
        NS_LOG_UNCOND("Callback demangling failed: status " << status);
        ret = mangled;
    }

    if (demangled)
    {
        std::free(demangled);
    }
    return ret;
}

}

#else

std::string
nsim2023::CallbackImplBase::Demangle(const std::string& mangled)
{
    NS_LOG_FUNCTION(this << mangled);
    return mangled;
}

#endif

