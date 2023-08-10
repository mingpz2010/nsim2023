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

#ifndef CALLBACK_H
#define CALLBACK_H

#include "attribute-helper.h"
#include "attribute.h"
#include "fatal-error.h"
#include "ptr.h"
#include "simple-ref-count.h"

#include <functional>
#include <memory>
#include <typeinfo>
#include <utility>
#include <vector>

namespace nsim2023
{

class CallbackImplBase : public SimpleRefCount<CallbackImplBase>
{
  public:
    /** Virtual destructor */
    virtual ~CallbackImplBase()
    {
    }

    virtual bool IsEqual(Ptr<const CallbackImplBase> other) const = 0;

    virtual std::string GetTypeid() const = 0;

  protected:

    static std::string Demangle(const std::string& mangled);

    template <typename T>
    static std::string GetCppTypeid()
    {
        std::string typeName;
        try
        {
            typeName = typeid(T).name();
            typeName = Demangle(typeName);
        }
        catch (const std::bad_typeid& e)
        {
            typeName = e.what();
        }
        return typeName;
    }
};

class CallbackComponentBase
{
  public:
    /** Virtual destructor */
    virtual ~CallbackComponentBase()
    {
    }

    virtual bool IsEqual(std::shared_ptr<const CallbackComponentBase> other) const = 0;
};

template <typename T, bool isComparable = true>
class CallbackComponent : public CallbackComponentBase
{
  public:

    CallbackComponent(const T& t)
        : m_comp(t)
    {
    }

    bool IsEqual(std::shared_ptr<const CallbackComponentBase> other) const override
    {
        auto p = std::dynamic_pointer_cast<const CallbackComponent<T>>(other);

        // other must have the same type and value as ours
        if (p == nullptr || p->m_comp != m_comp)
        {
            return false;
        }

        return true;
    }

  private:
    T m_comp;
};

template <typename T>
class CallbackComponent<T, false> : public CallbackComponentBase
{
  public:
    CallbackComponent(const T& t)
    {
    }

    bool IsEqual(std::shared_ptr<const CallbackComponentBase> other) const override
    {
        return false;
    }
};

// Vector of callback components
typedef std::vector<std::shared_ptr<CallbackComponentBase>> CallbackComponentVector;


template <typename R, typename... UArgs>
class CallbackImpl : public CallbackImplBase
{
  public:
    CallbackImpl(std::function<R(UArgs...)> func, const CallbackComponentVector& components)
        : m_func(func),
          m_components(components)
    {
    }

    const std::function<R(UArgs...)>& GetFunction() const
    {
        return m_func;
    }

    const CallbackComponentVector& GetComponents() const
    {
        return m_components;
    }

    R operator()(UArgs... uargs) const
    {
        return m_func(uargs...);
    }

    bool IsEqual(Ptr<const CallbackImplBase> other) const override
    {
        const CallbackImpl<R, UArgs...>* otherDerived =
            dynamic_cast<const CallbackImpl<R, UArgs...>*>(PeekPointer(other));

        if (otherDerived == nullptr)
        {
            return false;
        }

        // if the two callback implementations are made of a distinct number of
        // components, they are different
        if (m_components.size() != otherDerived->GetComponents().size())
        {
            return false;
        }

        // the two functions are equal if they compare equal or the shared pointers
        // point to the same locations
        if (!m_components.at(0)->IsEqual(otherDerived->GetComponents().at(0)) &&
            m_components.at(0) != otherDerived->GetComponents().at(0))
        {
            return false;
        }

        // check if the remaining components are equal one by one
        for (std::size_t i = 1; i < m_components.size(); i++)
        {
            if (!m_components.at(i)->IsEqual(otherDerived->GetComponents().at(i)))
            {
                return false;
            }
        }

        return true;
    }

    std::string GetTypeid() const override
    {
        return DoGetTypeid();
    }

    /** \copydoc GetTypeid(). */
    static std::string DoGetTypeid()
    {
        static std::vector<std::string> vec = {GetCppTypeid<R>(), GetCppTypeid<UArgs>()...};

        static std::string id("CallbackImpl<");
        for (auto& s : vec)
        {
            id.append(s + ",");
        }
        if (id.back() == ',')
        {
            id.pop_back();
        }
        id.push_back('>');

        return id;
    }

  private:
    /// Stores the callable object associated with this callback (as a lambda)
    std::function<R(UArgs...)> m_func;

    /// Stores the original callable object and the bound arguments, if any
    std::vector<std::shared_ptr<CallbackComponentBase>> m_components;
};

class CallbackBase
{
  public:
    CallbackBase()
        : m_impl()
    {
    }

    /** \return The impl pointer */
    Ptr<CallbackImplBase> GetImpl() const
    {
        return m_impl;
    }

  protected:
    /**
     * Construct from a pimpl
     * \param [in] impl The CallbackImplBase Ptr
     */
    CallbackBase(Ptr<CallbackImplBase> impl)
        : m_impl(impl)
    {
    }

    Ptr<CallbackImplBase> m_impl; //!< the pimpl
};

template <typename R, typename... UArgs>
class Callback : public CallbackBase
{
  public:
    Callback()
    {
    }

    Callback(const Ptr<CallbackImpl<R, UArgs...>>& impl)
        : CallbackBase(impl)
    {
    }

    template <typename... BArgs>
    Callback(const CallbackBase& cb, BArgs... bargs)
    {
        auto cbDerived =
            static_cast<const CallbackImpl<R, BArgs..., UArgs...>*>(PeekPointer(cb.GetImpl()));

        std::function<R(BArgs..., UArgs...)> f(cbDerived->GetFunction());

        CallbackComponentVector components(cbDerived->GetComponents());
        components.insert(components.end(), {std::make_shared<CallbackComponent<BArgs>>(bargs)...});

        m_impl = Create<CallbackImpl<R, UArgs...>>(
            [f, bargs...](UArgs... uargs) -> R { return f(bargs..., uargs...); },
            components);
    }

    template <typename T,
              std::enable_if_t<!std::is_base_of_v<CallbackBase, T>, int> = 0,
              typename... BArgs>
    Callback(T func, BArgs... bargs)
    {
        // store the function in a std::function object
        std::function<R(BArgs..., UArgs...)> f(func);

        // The original function is comparable if it is a function pointer or
        // a pointer to a member function or a pointer to a member data.
        constexpr bool isComp =
            std::is_function_v<std::remove_pointer_t<T>> || std::is_member_pointer_v<T>;

        CallbackComponentVector components({std::make_shared<CallbackComponent<T, isComp>>(func),
                                            std::make_shared<CallbackComponent<BArgs>>(bargs)...});

        m_impl = Create<CallbackImpl<R, UArgs...>>(
            [f, bargs...](UArgs... uargs) -> R { return f(bargs..., uargs...); },
            components);
    }

  private:

    template <std::size_t... INDEX, typename... BoundArgs>
    auto BindImpl(std::index_sequence<INDEX...> seq, BoundArgs... bargs)
    {
        return Callback<R, std::tuple_element_t<sizeof...(bargs) + INDEX, std::tuple<UArgs...>>...>(
            *this,
            bargs...);
    }

  public:

    template <typename... BoundArgs>
    auto Bind(BoundArgs... bargs)
    {
        static_assert(sizeof...(UArgs) > 0);
        return BindImpl(std::make_index_sequence<sizeof...(UArgs) - sizeof...(BoundArgs)>{},
                        std::forward<BoundArgs>(bargs)...);
    }

    /**
     * Check for null implementation
     *
     * \return \c true if I don't have an implementation
     */
    bool IsNull() const
    {
        return (DoPeekImpl() == nullptr);
    }

    /** Discard the implementation, set it to null */
    void Nullify()
    {
        m_impl = nullptr;
    }


    R operator()(UArgs... uargs) const
    {
        return (*(DoPeekImpl()))(uargs...);
    }

    /**
     * Equality test.
     *
     * \param [in] other Callback
     * \return \c true if we are equal
     */
    bool IsEqual(const CallbackBase& other) const
    {
        return m_impl->IsEqual(other.GetImpl());
    }

    /**
     * Check for compatible types
     *
     * \param [in] other Callback Ptr
     * \return \c true if other can be dynamic_cast to my type
     */
    bool CheckType(const CallbackBase& other) const
    {
        return DoCheckType(other.GetImpl());
    }

    /**
     * Adopt the other's implementation, if type compatible
     *
     * \param [in] other Callback
     * \returns \c true if \pname{other} was type-compatible and could be adopted.
     */
    bool Assign(const CallbackBase& other)
    {
        auto otherImpl = other.GetImpl();
        if (!DoCheckType(otherImpl))
        {
            std::string othTid = otherImpl->GetTypeid();
            std::string myTid = CallbackImpl<R, UArgs...>::DoGetTypeid();
            NS_FATAL_ERROR_CONT("Incompatible types. (feed to \"c++filt -t\" if needed)"
                                << std::endl
                                << "got=" << othTid << std::endl
                                << "expected=" << myTid);
            return false;
        }
        m_impl = const_cast<CallbackImplBase*>(PeekPointer(otherImpl));
        return true;
    }

  private:
    /** \return The pimpl pointer */
    CallbackImpl<R, UArgs...>* DoPeekImpl() const
    {
        return static_cast<CallbackImpl<R, UArgs...>*>(PeekPointer(m_impl));
    }

    /**
     * Check for compatible types
     *
     * \param [in] other Callback Ptr
     * \return \c true if other can be dynamic_cast to my type
     */
    bool DoCheckType(Ptr<const CallbackImplBase> other) const
    {
        if (other && dynamic_cast<const CallbackImpl<R, UArgs...>*>(PeekPointer(other)) != nullptr)
        {
            return true;
        }
        else if (!other)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

template <typename R, typename... Args>
bool
operator!=(Callback<R, Args...> a, Callback<R, Args...> b)
{
    return !a.IsEqual(b);
}

template <typename T, typename OBJ, typename R, typename... Args>
Callback<R, Args...>
MakeCallback(R (T::*memPtr)(Args...), OBJ objPtr)
{
    return Callback<R, Args...>(memPtr, objPtr);
}

template <typename T, typename OBJ, typename R, typename... Args>
Callback<R, Args...>
MakeCallback(R (T::*memPtr)(Args...) const, OBJ objPtr)
{
    return Callback<R, Args...>(memPtr, objPtr);
}

template <typename R, typename... Args>
Callback<R, Args...>
MakeCallback(R (*fnPtr)(Args...))
{
    return Callback<R, Args...>(fnPtr);
}

template <typename R, typename... Args>
Callback<R, Args...>
MakeNullCallback()
{
    return Callback<R, Args...>();
}

template <typename R, typename... Args, typename... BArgs>
auto
MakeBoundCallback(R (*fnPtr)(Args...), BArgs... bargs)
{
    return Callback<R, Args...>(fnPtr).Bind(bargs...);
}

template <typename T, typename OBJ, typename R, typename... Args, typename... BArgs>
auto
MakeCallback(R (T::*memPtr)(Args...), OBJ objPtr, BArgs... bargs)
{
    return Callback<R, Args...>(memPtr, objPtr).Bind(bargs...);
}

template <typename T, typename OBJ, typename R, typename... Args, typename... BArgs>
auto
MakeCallback(R (T::*memPtr)(Args...) const, OBJ objPtr, BArgs... bargs)
{
    return Callback<R, Args...>(memPtr, objPtr).Bind(bargs...);
}

}

namespace nsim2023
{

class CallbackValue : public AttributeValue
{
  public:
    /** Constructor */
    CallbackValue();
    /**
     * Copy constructor
     * \param [in] base Callback to copy
     */
    CallbackValue(const CallbackBase& base);
    /** Destructor */
    ~CallbackValue() override;
    /** \param [in] base The CallbackBase to use */
    void Set(CallbackBase base);
    /* Documented by print-introspected-doxygen.cc */
    template <typename T>
    bool GetAccessor(T& value) const;
    /** \return A copy of this CallBack */
    Ptr<AttributeValue> Copy() const override;
    /**
     * Serialize to string
     * \param [in] checker The checker to validate with
     * \return Serialized form of this Callback.
     */
    std::string SerializeToString(Ptr<const AttributeChecker> checker) const override;
    /**
     * Deserialize from string (not implemented)
     *
     * \param [in] value Source string
     * \param [in] checker Checker to validate with
     * \return \c true if successful
     */
    bool DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker) override;

  private:
    /* Documented by print-introspected-doxygen.cc */
    CallbackBase m_value;
};

ATTRIBUTE_ACCESSOR_DEFINE(Callback);
ATTRIBUTE_CHECKER_DEFINE(Callback);

}

namespace nsim2023
{

template <typename T>
bool
CallbackValue::GetAccessor(T& value) const
{
    if (value.CheckType(m_value))
    {
        if (!value.Assign(m_value))
        {
            NS_FATAL_ERROR_NO_MSG();
        }
        return true;
    }
    return false;
}

}

#endif /* CALLBACK_H */

