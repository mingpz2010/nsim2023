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

#include "boolean.h"

#include "fatal-error.h"
#include "log.h"


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("Boolean");

BooleanValue::BooleanValue()
    : m_value(false)
{
    NS_LOG_FUNCTION(this);
}

BooleanValue::BooleanValue(bool value)
    : m_value(value)
{
    NS_LOG_FUNCTION(this << value);
}

void
BooleanValue::Set(bool value)
{
    NS_LOG_FUNCTION(this << value);
    m_value = value;
}

bool
BooleanValue::Get() const
{
    NS_LOG_FUNCTION(this);
    return m_value;
}

BooleanValue::operator bool() const
{
    return m_value;
}

std::ostream&
operator<<(std::ostream& os, const BooleanValue& value)
{
    if (value.Get())
    {
        os << "true";
    }
    else
    {
        os << "false";
    }
    return os;
}

Ptr<AttributeValue>
BooleanValue::Copy() const
{
    NS_LOG_FUNCTION(this);

    return Create<BooleanValue>(*this);
}

std::string
BooleanValue::SerializeToString(Ptr<const AttributeChecker> checker) const
{
    NS_LOG_FUNCTION(this << checker);

    if (m_value)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

bool
BooleanValue::DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker)
{
    NS_LOG_FUNCTION(this << value << checker);

    if (value == "true" || value == "1" || value == "t")
    {
        m_value = true;
        return true;
    }
    else if (value == "false" || value == "0" || value == "f")
    {
        m_value = false;
        return true;
    }
    else
    {
        return false;
    }
}

ATTRIBUTE_CHECKER_IMPLEMENT_WITH_NAME(Boolean, "bool");

}

