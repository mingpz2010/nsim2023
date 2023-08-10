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

#ifndef OBJECT_PTR_CONTAINER_H
#define OBJECT_PTR_CONTAINER_H

#include "attribute.h"
#include "object.h"
#include "ptr.h"

#include <map>


namespace nsim2023
{

/**
 * Container for a set of nsim2023::Object pointers.
 */
class ObjectPtrContainerValue : public AttributeValue
{
  public:
    /** Iterator type for traversing this container. */
    typedef std::map<std::size_t, Ptr<Object>>::const_iterator Iterator;

    /** Default constructor. */
    ObjectPtrContainerValue();

    Iterator Begin() const;

    Iterator End() const;

    std::size_t GetN() const;

    Ptr<Object> Get(std::size_t i) const;

    Ptr<AttributeValue> Copy() const override;

    /**
     * Serialize each of the Object pointers to a string.
     */
    std::string SerializeToString(Ptr<const AttributeChecker> checker) const override;

    /**
     * Deserialize from a string. (Not implemented; raises a fatal error.)
     */
    bool DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker) override;

  private:

    friend class ObjectPtrContainerAccessor;

    std::map<std::size_t, Ptr<Object>> m_objects;
};


template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor> MakeObjectPtrContainerAccessor(Ptr<U> (T::*get)(INDEX) const,
                                                            INDEX (T::*getN)() const);


template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor> MakeObjectPtrContainerAccessor(INDEX (T::*getN)() const,
                                                            Ptr<U> (T::*get)(INDEX) const);

class ObjectPtrContainerChecker : public AttributeChecker
{
  public:
    virtual TypeId GetItemTypeId() const = 0;
};

template <typename T>
Ptr<const AttributeChecker> MakeObjectPtrContainerChecker();

}

/***************************************************************
 *        The implementation of the above functions.
 ***************************************************************/
namespace nsim2023
{

namespace internal
{
/** ObjectPtrContainerChecker implementation class. */
template <typename T>
class ObjectPtrContainerChecker : public nsim2023::ObjectPtrContainerChecker
{
  public:
    TypeId GetItemTypeId() const override
    {
        return T::GetTypeId();
    }

    bool Check(const AttributeValue& value) const override
    {
        return dynamic_cast<const ObjectPtrContainerValue*>(&value) != nullptr;
    }

    std::string GetValueTypeName() const override
    {
        return "nsim2023::ObjectPtrContainerValue";
    }

    bool HasUnderlyingTypeInformation() const override
    {
        return true;
    }

    std::string GetUnderlyingTypeInformation() const override
    {
        return "nsim2023::Ptr< " + T::GetTypeId().GetName() + " >";
    }

    Ptr<AttributeValue> Create() const override
    {
        return nsim2023::Create<ObjectPtrContainerValue>();
    }

    bool Copy(const AttributeValue& source, AttributeValue& destination) const override
    {
        const ObjectPtrContainerValue* src = dynamic_cast<const ObjectPtrContainerValue*>(&source);
        ObjectPtrContainerValue* dst = dynamic_cast<ObjectPtrContainerValue*>(&destination);
        if (src == nullptr || dst == nullptr)
        {
            return false;
        }
        *dst = *src;
        return true;
    }
};

}


class ObjectPtrContainerAccessor : public AttributeAccessor
{
  public:
    bool Set(ObjectBase* object, const AttributeValue& value) const override;
    bool Get(const ObjectBase* object, AttributeValue& value) const override;
    bool HasGetter() const override;
    bool HasSetter() const override;

  private:

    /**
     * Get the number of instances in the container.
     */
    virtual bool DoGetN(const ObjectBase* object, std::size_t* n) const = 0;

    /**
     * Get an instance from the container, identified by index.
     */
    virtual Ptr<Object> DoGet(const ObjectBase* object,
                              std::size_t i,
                              std::size_t* index) const = 0;
};

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectPtrContainerAccessor(Ptr<U> (T::*get)(INDEX) const, INDEX (T::*getN)() const)
{
    struct MemberGetters : public ObjectPtrContainerAccessor
    {
        bool DoGetN(const ObjectBase* object, std::size_t* n) const override
        {
            const T* obj = dynamic_cast<const T*>(object);
            if (obj == nullptr)
            {
                return false;
            }
            *n = (obj->*m_getN)();
            return true;
        }

        Ptr<Object> DoGet(const ObjectBase* object,
                          std::size_t i,
                          std::size_t* index) const override
        {
            const T* obj = static_cast<const T*>(object);
            *index = i;
            return (obj->*m_get)(i);
        }

        Ptr<U> (T::*m_get)(INDEX) const;
        INDEX (T::*m_getN)() const;
    }* spec = new MemberGetters();

    spec->m_get = get;
    spec->m_getN = getN;
    return Ptr<const AttributeAccessor>(spec, false);
}

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectPtrContainerAccessor(INDEX (T::*getN)() const, Ptr<U> (T::*get)(INDEX) const)
{
    return MakeObjectPtrContainerAccessor(get, getN);
}

template <typename T>
Ptr<const AttributeChecker>
MakeObjectPtrContainerChecker()
{
    return Create<internal::ObjectPtrContainerChecker<T>>();
}

}

#endif /* OBJECT_PTR_CONTAINER_H */

