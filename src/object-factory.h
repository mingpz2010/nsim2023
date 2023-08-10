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

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "attribute-construction-list.h"
#include "object.h"
#include "type-id.h"


namespace nsim2023
{

class AttributeValue;

/**
 * 
 * This class can also hold a set of attributes to set
 * automatically during the object construction.
 * 
 */
class ObjectFactory
{
  public:

    ObjectFactory();

    template <typename... Args>
    ObjectFactory(const std::string& typeId, Args&&... args);

    void SetTypeId(TypeId tid);
    void SetTypeId(std::string tid);

    bool IsTypeIdSet() const;

    template <typename... Args>
    void Set(const std::string& name, const AttributeValue& value, Args&&... args);

    void Set()
    {
    }

    TypeId GetTypeId() const;

    Ptr<Object> Create() const;

    template <typename T>
    Ptr<T> Create() const;

  private:

    void DoSet(const std::string& name, const AttributeValue& value);

    friend std::ostream& operator<<(std::ostream& os, const ObjectFactory& factory);

    friend std::istream& operator>>(std::istream& is, ObjectFactory& factory);

    /** The TypeId this factory will create. */
    TypeId m_tid;

    AttributeConstructionList m_parameters;
};

std::ostream& operator<<(std::ostream& os, const ObjectFactory& factory);
std::istream& operator>>(std::istream& is, ObjectFactory& factory);


template <typename T, typename... Args>
Ptr<T> CreateObjectWithAttributes(Args... args);

ATTRIBUTE_HELPER_HEADER(ObjectFactory);

}

/***************************************************************
 *  Implementation of the templates declared above.
 ***************************************************************/
namespace nsim2023
{

template <typename T>
Ptr<T>
ObjectFactory::Create() const
{
    Ptr<Object> object = Create();
    return object->GetObject<T>();
}

template <typename... Args>
ObjectFactory::ObjectFactory(const std::string& typeId, Args&&... args)
{
    SetTypeId(typeId);
    Set(args...);
}

template <typename... Args>
void
ObjectFactory::Set(const std::string& name, const AttributeValue& value, Args&&... args)
{
    DoSet(name, value);
    Set(args...);
}

template <typename T, typename... Args>
Ptr<T>
CreateObjectWithAttributes(Args... args)
{
    ObjectFactory factory;
    factory.SetTypeId(T::GetTypeId());
    factory.Set(args...);
    return factory.Create<T>();
}

}

#endif /* OBJECT_FACTORY_H */

