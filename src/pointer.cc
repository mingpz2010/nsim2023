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

#include "pointer.h"

#include "log.h"
#include "object-factory.h"

#include <sstream>


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("Pointer");

PointerValue::PointerValue()
    : m_value()
{
    NS_LOG_FUNCTION(this);
}

PointerValue::PointerValue(Ptr<Object> object)
    : m_value(object)
{
    NS_LOG_FUNCTION(object);
}

void
PointerValue::SetObject(Ptr<Object> object)
{
    NS_LOG_FUNCTION(object);
    m_value = object;
}

Ptr<Object>
PointerValue::GetObject() const
{
    NS_LOG_FUNCTION(this);
    return m_value;
}

Ptr<AttributeValue>
PointerValue::Copy() const
{
    NS_LOG_FUNCTION(this);
    return Create<PointerValue>(*this);
}

std::string
PointerValue::SerializeToString(Ptr<const AttributeChecker> checker) const
{
    NS_LOG_FUNCTION(this << checker);
    std::ostringstream oss;
    oss << m_value;
    return oss.str();
}

bool
PointerValue::DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker)
{
    NS_LOG_FUNCTION(this << value << checker);
    ObjectFactory factory;
    std::istringstream iss;
    iss.str(value);
    iss >> factory;
    if (iss.fail())
    {
        return false;
    }
    m_value = factory.Create<Object>();
    return true;
}

}

