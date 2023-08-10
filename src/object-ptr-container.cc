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

#include "object-ptr-container.h"

#include "log.h"


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("ObjectPtrContainer");

ObjectPtrContainerValue::ObjectPtrContainerValue()
{
    NS_LOG_FUNCTION(this);
}

ObjectPtrContainerValue::Iterator
ObjectPtrContainerValue::Begin() const
{
    NS_LOG_FUNCTION(this);
    return m_objects.begin();
}

ObjectPtrContainerValue::Iterator
ObjectPtrContainerValue::End() const
{
    NS_LOG_FUNCTION(this);
    return m_objects.end();
}

std::size_t
ObjectPtrContainerValue::GetN() const
{
    NS_LOG_FUNCTION(this);
    return m_objects.size();
}

Ptr<Object>
ObjectPtrContainerValue::Get(std::size_t i) const
{
    NS_LOG_FUNCTION(this << i);
    Iterator it = m_objects.find(i);
    Ptr<Object> value = nullptr;
    if (it != m_objects.end())
    {
        value = m_objects.find(i)->second;
    }
    return value;
}

Ptr<AttributeValue>
ObjectPtrContainerValue::Copy() const
{
    NS_LOG_FUNCTION(this);
    return nsim2023::Create<ObjectPtrContainerValue>(*this);
}

std::string
ObjectPtrContainerValue::SerializeToString(Ptr<const AttributeChecker> checker) const
{
    NS_LOG_FUNCTION(this << checker);
    std::ostringstream oss;
    Iterator it;
    for (it = Begin(); it != End(); ++it)
    {
        oss << (*it).second;
        if (it != End())
        {
            oss << " ";
        }
    }
    return oss.str();
}

bool
ObjectPtrContainerValue::DeserializeFromString(std::string value,
                                               Ptr<const AttributeChecker> checker)
{
    NS_LOG_FUNCTION(this << value << checker);
    NS_FATAL_ERROR("cannot deserialize a set of object pointers.");
    return true;
}

bool
ObjectPtrContainerAccessor::Set(ObjectBase* object, const AttributeValue& value) const
{
    // not allowed.
    NS_LOG_FUNCTION(this << object << &value);
    return false;
}

bool
ObjectPtrContainerAccessor::Get(const ObjectBase* object, AttributeValue& value) const
{
    NS_LOG_FUNCTION(this << object << &value);
    ObjectPtrContainerValue* v = dynamic_cast<ObjectPtrContainerValue*>(&value);
    if (v == nullptr)
    {
        return false;
    }
    v->m_objects.clear();
    std::size_t n;
    bool ok = DoGetN(object, &n);
    if (!ok)
    {
        return false;
    }
    for (std::size_t i = 0; i < n; i++)
    {
        std::size_t index;
        Ptr<Object> o = DoGet(object, i, &index);
        v->m_objects[index] = o;
    }
    return true;
}

bool
ObjectPtrContainerAccessor::HasGetter() const
{
    NS_LOG_FUNCTION(this);
    return true;
}

bool
ObjectPtrContainerAccessor::HasSetter() const
{
    NS_LOG_FUNCTION(this);
    return false;
}

}

