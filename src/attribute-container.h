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

#ifndef ATTRIBUTE_CONTAINER_H
#define ATTRIBUTE_CONTAINER_H

#include "attribute-helper.h"
#include "nsim-string.h"

#include <algorithm>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace nsim2023
{

class AttributeChecker;


template <class A, template <class...> class C = std::list>
class AttributeContainerValue : public AttributeValue
{
  public:
    /** AttributeValue (element) type. */
    typedef A attribute_type;
    /** Type actually stored within the container. */
    typedef Ptr<A> value_type;
    /** Internal container type. */
    typedef std::list<value_type> container_type;
    /** stl-style Const iterator type. */
    typedef typename container_type::const_iterator const_iterator;
    /** stl-style Non-const iterator type. */
    typedef typename container_type::iterator iterator;
    /** Size type for container. */
    typedef typename container_type::size_type size_type;
    /** NS3 style iterator type. */
    typedef typename AttributeContainerValue::const_iterator Iterator;

    // use underlying AttributeValue to get return element type
    /** Item type of container returned by Get. */
    typedef typename std::invoke_result_t<decltype(&A::Get), A> item_type;
    /** Type of container returned. */
    typedef C<item_type> result_type;


    AttributeContainerValue(char sep = ',');


    template <class CONTAINER>
    AttributeContainerValue(const CONTAINER& c);


    template <class ITER>
    AttributeContainerValue(const ITER begin, const ITER end);

    /** Destructor. */
    ~AttributeContainerValue() override;

    // Inherited
    Ptr<AttributeValue> Copy() const override;
    bool DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker) override;
    std::string SerializeToString(Ptr<const AttributeChecker> checker) const override;


    result_type Get() const;

    template <class T>
    void Set(const T& c);

    template <typename T>
    bool GetAccessor(T& value) const;


    size_type GetN() const;

    Iterator Begin();

    Iterator End();

    // STL-interface
    size_type size() const;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

  private:
    template <class ITER>
    Ptr<AttributeContainerValue<A, C>> CopyFrom(const ITER begin, const ITER end);

    char m_sep;                 //!< Item separator
    container_type m_container; //!< Internal container
};

class AttributeContainerChecker : public AttributeChecker
{
  public:
    virtual void SetItemChecker(Ptr<const AttributeChecker> itemchecker) = 0;

    virtual Ptr<const AttributeChecker> GetItemChecker() const = 0;
};


template <class A, template <class...> class C>
Ptr<AttributeChecker> MakeAttributeContainerChecker(const AttributeContainerValue<A, C>& value);


template <class A, template <class...> class C = std::list>
Ptr<const AttributeChecker> MakeAttributeContainerChecker(Ptr<const AttributeChecker> itemchecker);


template <class A, template <class...> class C = std::list>
Ptr<AttributeChecker> MakeAttributeContainerChecker();


template <typename A, template <typename...> class C = std::list, typename T1>
Ptr<const AttributeAccessor> MakeAttributeContainerAccessor(T1 a1);


template <typename A, template <typename...> class C = std::list, typename T1, typename T2>
Ptr<const AttributeAccessor> MakeAttributeContainerAccessor(T1 a1, T2 a2);

}

/*****************************************************************************
 * Implementation below
 *****************************************************************************/
namespace nsim2023
{

namespace internal
{

template <class A, template <class...> class C>
class AttributeContainerChecker : public nsim2023::AttributeContainerChecker
{
  public:
    AttributeContainerChecker();
    /**
     * Explicit constructor
     * \param itemchecker The AttributeChecker.
     */
    explicit AttributeContainerChecker(Ptr<const AttributeChecker> itemchecker);
    void SetItemChecker(Ptr<const AttributeChecker> itemchecker) override;
    Ptr<const AttributeChecker> GetItemChecker() const override;

  private:
    Ptr<const AttributeChecker> m_itemchecker; //!< The AttributeChecker
};

template <class A, template <class...> class C>
AttributeContainerChecker<A, C>::AttributeContainerChecker()
    : m_itemchecker(nullptr)
{
}

template <class A, template <class...> class C>
AttributeContainerChecker<A, C>::AttributeContainerChecker(Ptr<const AttributeChecker> itemchecker)
    : m_itemchecker(itemchecker)
{
}

template <class A, template <class...> class C>
void
AttributeContainerChecker<A, C>::SetItemChecker(Ptr<const AttributeChecker> itemchecker)
{
    m_itemchecker = itemchecker;
}

template <class A, template <class...> class C>
Ptr<const AttributeChecker>
AttributeContainerChecker<A, C>::GetItemChecker() const
{
    return m_itemchecker;
}

} // namespace internal

template <class A, template <class...> class C>
Ptr<AttributeChecker>
MakeAttributeContainerChecker(const AttributeContainerValue<A, C>& value)
{
    return MakeAttributeContainerChecker<A, C>();
}

template <class A, template <class...> class C>
Ptr<const AttributeChecker>
MakeAttributeContainerChecker(Ptr<const AttributeChecker> itemchecker)
{
    auto checker = MakeAttributeContainerChecker<A, C>();
    auto acchecker = DynamicCast<AttributeContainerChecker>(checker);
    acchecker->SetItemChecker(itemchecker);
    return checker;
}

template <class A, template <class...> class C>
Ptr<AttributeChecker>
MakeAttributeContainerChecker()
{
    std::string containerType;
    std::string underlyingType;
    typedef AttributeContainerValue<A, C> T;
    {
        std::ostringstream oss;
        oss << "nsim2023::AttributeContainerValue<" << typeid(typename T::attribute_type).name() << ", "
            << typeid(typename T::container_type).name() << ">";
        containerType = oss.str();
    }

    {
        std::ostringstream oss;
        oss << "nsim2023::Ptr<" << typeid(typename T::attribute_type).name() << ">";
        underlyingType = oss.str();
    }

    return MakeSimpleAttributeChecker<T, internal::AttributeContainerChecker<A, C>>(containerType,
                                                                                    underlyingType);
}

template <class A, template <class...> class C>
AttributeContainerValue<A, C>::AttributeContainerValue(char sep)
    : m_sep(sep)
{
}

template <class A, template <class...> class C>
template <class CONTAINER>
AttributeContainerValue<A, C>::AttributeContainerValue(const CONTAINER& c)
    : AttributeContainerValue<A, C>(c.begin(), c.end())
{
}

template <class A, template <class...> class C>
template <class ITER>
AttributeContainerValue<A, C>::AttributeContainerValue(const ITER begin, const ITER end)
    : AttributeContainerValue()
{
    CopyFrom(begin, end);
}

template <class A, template <class...> class C>
AttributeContainerValue<A, C>::~AttributeContainerValue()
{
    m_container.clear();
}

template <class A, template <class...> class C>
Ptr<AttributeValue>
AttributeContainerValue<A, C>::Copy() const
{
    auto c = Create<AttributeContainerValue<A, C>>();
    c->m_sep = m_sep;
    c->m_container = m_container;
    return c;
}

template <class A, template <class...> class C>
bool
AttributeContainerValue<A, C>::DeserializeFromString(std::string value,
                                                     Ptr<const AttributeChecker> checker)
{
    auto acchecker = DynamicCast<const AttributeContainerChecker>(checker);
    if (!acchecker)
    {
        return false;
    }

    std::istringstream iss(value); // copies value
    while (std::getline(iss, value, m_sep))
    {
        auto avalue = acchecker->GetItemChecker()->CreateValidValue(StringValue(value));
        if (!avalue)
        {
            return false;
        }

        auto attr = DynamicCast<A>(avalue);
        if (!attr)
        {
            return false;
        }

        // TODO(jared): make insertion more generic?
        m_container.push_back(attr);
    }
    return true;
}

template <class A, template <class...> class C>
std::string
AttributeContainerValue<A, C>::SerializeToString(Ptr<const AttributeChecker> checker) const
{
    std::ostringstream oss;
    bool first = true;
    for (auto attr : *this)
    {
        if (!first)
        {
            oss << m_sep;
        }
        oss << attr->SerializeToString(checker);
        first = false;
    }
    return oss.str();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::result_type
AttributeContainerValue<A, C>::Get() const
{
    result_type c;
    for (const value_type& a : *this)
    {
        c.insert(c.end(), a->Get());
    }
    return c;
}

template <class A, template <class...> class C>
template <typename T>
bool
AttributeContainerValue<A, C>::GetAccessor(T& value) const
{
    result_type src = Get();
    value.clear();
    std::copy(src.begin(), src.end(), std::inserter(value, value.end()));
    return true;
}

template <class A, template <class...> class C>
template <class T>
void
AttributeContainerValue<A, C>::Set(const T& c)
{
    m_container.clear();
    CopyFrom(c.begin(), c.end());
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::size_type
AttributeContainerValue<A, C>::GetN() const
{
    return size();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::Iterator
AttributeContainerValue<A, C>::Begin()
{
    return begin();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::Iterator
AttributeContainerValue<A, C>::End()
{
    return end();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::size_type
AttributeContainerValue<A, C>::size() const
{
    return m_container.size();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::iterator
AttributeContainerValue<A, C>::begin()
{
    return m_container.begin();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::iterator
AttributeContainerValue<A, C>::end()
{
    return m_container.end();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::const_iterator
AttributeContainerValue<A, C>::begin() const
{
    return m_container.cbegin();
}

template <class A, template <class...> class C>
typename AttributeContainerValue<A, C>::const_iterator
AttributeContainerValue<A, C>::end() const
{
    return m_container.cend();
}

template <class A, template <class...> class C>
template <class ITER>
Ptr<AttributeContainerValue<A, C>>
AttributeContainerValue<A, C>::CopyFrom(const ITER begin, const ITER end)
{
    for (ITER iter = begin; iter != end; ++iter)
    {
        m_container.push_back(Create<A>(*iter));
    }
    return this;
}

template <typename A, template <typename...> class C, typename T1>
Ptr<const AttributeAccessor>
MakeAttributeContainerAccessor(T1 a1)
{
    return MakeAccessorHelper<AttributeContainerValue<A, C>>(a1);
}

template <typename A, template <typename...> class C, typename T1, typename T2>
Ptr<const AttributeAccessor>
MakeAttributeContainerAccessor(T1 a1, T2 a2)
{
    return MakeAccessorHelper<AttributeContainerValue<A, C>>(a1, a2);
}

}

#endif // ATTRIBUTE_CONTAINER_H

