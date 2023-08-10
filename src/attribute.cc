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

#include "attribute.h"

#include "fatal-error.h"
#include "log.h"
#include "nsim-string.h"

#include <sstream>


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("AttributeValue");

AttributeValue::AttributeValue()
{
}

AttributeValue::~AttributeValue()
{
}

AttributeAccessor::AttributeAccessor()
{
}

AttributeAccessor::~AttributeAccessor()
{
}

AttributeChecker::AttributeChecker()
{
}

AttributeChecker::~AttributeChecker()
{
}

Ptr<AttributeValue>
AttributeChecker::CreateValidValue(const AttributeValue& value) const
{
    NS_LOG_FUNCTION(this << &value);
    if (Check(value))
    {
        return value.Copy();
    }
    // attempt to convert to string.
    const StringValue* str = dynamic_cast<const StringValue*>(&value);
    if (str == nullptr)
    {
        return nullptr;
    }
    // attempt to convert back to value.
    Ptr<AttributeValue> v = Create();
    bool ok = v->DeserializeFromString(str->Get(), this);
    if (!ok)
    {
        return nullptr;
    }
    ok = Check(*v);
    if (!ok)
    {
        return nullptr;
    }
    return v;
}

EmptyAttributeValue::EmptyAttributeValue()
{
    NS_LOG_FUNCTION(this);
}

Ptr<AttributeValue>
EmptyAttributeValue::Copy() const
{
    NS_LOG_FUNCTION(this);
    return Create<EmptyAttributeValue>();
}

std::string
EmptyAttributeValue::SerializeToString(Ptr<const AttributeChecker> checker) const
{
    NS_LOG_FUNCTION(this << checker);
    return "";
}

bool
EmptyAttributeValue::DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker)
{
    NS_LOG_FUNCTION(this << value << checker);
    return true;
}

EmptyAttributeAccessor::EmptyAttributeAccessor()
    : AttributeAccessor()
{
}

EmptyAttributeAccessor::~EmptyAttributeAccessor()
{
}

bool
EmptyAttributeAccessor::Set(ObjectBase* object [[maybe_unused]],
                            const AttributeValue& value [[maybe_unused]]) const
{
    return true;
}

bool
EmptyAttributeAccessor::Get(const ObjectBase* object [[maybe_unused]],
                            AttributeValue& attribute [[maybe_unused]]) const
{
    return true;
}

bool
EmptyAttributeAccessor::HasGetter() const
{
    return false;
}

bool
EmptyAttributeAccessor::HasSetter() const
{
    return false;
}

EmptyAttributeChecker::EmptyAttributeChecker()
    : AttributeChecker()
{
}

EmptyAttributeChecker::~EmptyAttributeChecker()
{
}

bool
EmptyAttributeChecker::Check(const AttributeValue& value [[maybe_unused]]) const
{
    return true;
}

std::string
EmptyAttributeChecker::GetValueTypeName() const
{
    return "EmptyAttribute";
}

bool
EmptyAttributeChecker::HasUnderlyingTypeInformation() const
{
    return false;
}

std::string
EmptyAttributeChecker::GetUnderlyingTypeInformation() const
{
    return "";
}

Ptr<AttributeValue>
EmptyAttributeChecker::Create() const
{
    static EmptyAttributeValue t;
    return Ptr<AttributeValue>(&t, false);
}

bool
EmptyAttributeChecker::Copy(const AttributeValue& source [[maybe_unused]],
                            AttributeValue& destination [[maybe_unused]]) const
{
    return true;
}

}

