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

#ifndef TYPE_ID_H
#define TYPE_ID_H

#include "attribute-accessor-helper.h"
#include "attribute-helper.h"
#include "attribute.h"
#include "callback.h"
#include "deprecated.h"
#include "hash.h"
#include "trace-source-accessor.h"

#include <stdint.h>
#include <string>

namespace nsim2023
{

class ObjectBase;

//  a unique identifier for an interface.
class TypeId
{
  public:
    /** Flags describing when a given attribute can be read or written. */
    enum AttributeFlag
    {
        ATTR_GET = 1 << 0,       /**< The attribute can be read */
        ATTR_SET = 1 << 1,       /**< The attribute can be written */
        ATTR_CONSTRUCT = 1 << 2, /**< The attribute can be written at construction-time */
        ATTR_SGC = ATTR_GET | ATTR_SET |
                   ATTR_CONSTRUCT, /**< The attribute can be read, and written at any time */
    };

    /** The level of support or deprecation for attributes or trace sources. */
    enum SupportLevel
    {
        SUPPORTED,  /**< Attribute or trace source is currently used. */
        DEPRECATED, /**< Attribute or trace source is deprecated; user is warned. */
        OBSOLETE    /**< Attribute or trace source is not used anymore; simulation fails. */
    };

    /** Attribute implementation. */
    struct AttributeInformation
    {
        /** Attribute name. */
        std::string name;
        /** Attribute help string. */
        std::string help;
        /** AttributeFlags value. */
        uint32_t flags;
        /** Default initial value. */
        Ptr<const AttributeValue> originalInitialValue;
        /** Configured initial value. */
        Ptr<const AttributeValue> initialValue;
        /** Accessor object. */
        Ptr<const AttributeAccessor> accessor;
        /** Checker object. */
        Ptr<const AttributeChecker> checker;
        /** Support level/deprecation. */
        TypeId::SupportLevel supportLevel;
        /** Support message. */
        std::string supportMsg;
    };

    /** TraceSource implementation. */
    struct TraceSourceInformation
    {
        /** Trace name. */
        std::string name;
        /** Trace help string. */
        std::string help;
        /** Callback function signature type. */
        std::string callback;
        /** Trace accessor. */
        Ptr<const TraceSourceAccessor> accessor;
        /** Support level/deprecation. */
        TypeId::SupportLevel supportLevel;
        /** Support message. */
        std::string supportMsg;
    };

    /** Type of hash values. */
    typedef uint32_t hash_t;

    static TypeId LookupByName(std::string name);

    static bool LookupByNameFailSafe(std::string name, TypeId* tid);

    static TypeId LookupByHash(hash_t hash);

    static bool LookupByHashFailSafe(hash_t hash, TypeId* tid);

    static uint16_t GetRegisteredN();

    static TypeId GetRegistered(uint16_t i);

    explicit TypeId(const std::string& name);

    /**
     * Get the parent of this TypeId.
     */
    TypeId GetParent() const;

    bool HasParent() const;

    bool IsChildOf(TypeId other) const;

    /**
     * Get the group name.
     */
    std::string GetGroupName() const;

    std::string GetName() const;

    hash_t GetHash() const;

    /**
     * Get the size of this object.
     */
    std::size_t GetSize() const;

    bool HasConstructor() const;

    std::size_t GetAttributeN() const;

    struct TypeId::AttributeInformation GetAttribute(std::size_t i) const;

    std::string GetAttributeFullName(std::size_t i) const;


    Callback<ObjectBase*> GetConstructor() const;


    bool MustHideFromDocumentation() const;


    std::size_t GetTraceSourceN() const;

    struct TypeId::TraceSourceInformation GetTraceSource(std::size_t i) const;

    /**
     * Set the parent TypeId.
     */
    TypeId SetParent(TypeId tid);

    template <typename T>
    TypeId SetParent();


    TypeId SetGroupName(std::string groupName);


    TypeId SetSize(std::size_t size);

    /**
     * Record in this TypeId the fact that the default constructor
     * is accessible.
     */
    template <typename T>
    TypeId AddConstructor();

    /**
     * Record in this TypeId the fact that a new attribute exists.
     */
    TypeId AddAttribute(std::string name,
                        std::string help,
                        const AttributeValue& initialValue,
                        Ptr<const AttributeAccessor> accessor,
                        Ptr<const AttributeChecker> checker,
                        SupportLevel supportLevel = SUPPORTED,
                        const std::string& supportMsg = "");

    bool SetAttributeInitialValue(std::size_t i, Ptr<const AttributeValue> initialValue);

    TypeId AddAttribute(std::string name,
                        std::string help,
                        uint32_t flags,
                        const AttributeValue& initialValue,
                        Ptr<const AttributeAccessor> accessor,
                        Ptr<const AttributeChecker> checker,
                        SupportLevel supportLevel = SUPPORTED,
                        const std::string& supportMsg = "");

    TypeId AddTraceSource(std::string name,
                          std::string help,
                          Ptr<const TraceSourceAccessor> accessor,
                          std::string callback,
                          SupportLevel supportLevel = SUPPORTED,
                          const std::string& supportMsg = "");

    TypeId HideFromDocumentation();


    bool LookupAttributeByName(std::string name, struct AttributeInformation* info) const;

    Ptr<const TraceSourceAccessor> LookupTraceSourceByName(std::string name) const;

    Ptr<const TraceSourceAccessor> LookupTraceSourceByName(
        std::string name,
        struct TraceSourceInformation* info) const;

    /**
     * Get the internal id of this TypeId.
     */
    uint16_t GetUid() const;

    void SetUid(uint16_t uid);

    /** Default constructor.  This produces an invalid TypeId. */
    inline TypeId();

    inline TypeId(const TypeId& o);

    inline TypeId& operator=(const TypeId& o);

    inline ~TypeId();

  private:

    friend inline bool operator==(TypeId a, TypeId b);
    friend inline bool operator!=(TypeId a, TypeId b);
    friend bool operator<(TypeId a, TypeId b);

    explicit TypeId(uint16_t tid);
    /**
     * Implementation for AddConstructor().
     */
    void DoAddConstructor(Callback<ObjectBase*> callback);

    /** The TypeId value. */
    uint16_t m_tid;
};

std::ostream& operator<<(std::ostream& os, TypeId tid);

std::istream& operator>>(std::istream& is, TypeId& tid);

inline bool operator==(TypeId a, TypeId b);
inline bool operator!=(TypeId a, TypeId b);
bool operator<(TypeId a, TypeId b);

ATTRIBUTE_HELPER_HEADER(TypeId);

}

namespace nsim2023
{

TypeId::TypeId()
    : m_tid(0)
{
}

TypeId::TypeId(const TypeId& o)
    : m_tid(o.m_tid)
{
}

TypeId&
TypeId::operator=(const TypeId& o)
{
    m_tid = o.m_tid;
    return *this;
}

TypeId::~TypeId()
{
}

inline bool
operator==(TypeId a, TypeId b)
{
    return a.m_tid == b.m_tid;
}

inline bool
operator!=(TypeId a, TypeId b)
{
    return a.m_tid != b.m_tid;
}

/*************************************************************************
 *   The TypeId implementation which depends on templates
 *************************************************************************/
template <typename T>
TypeId
TypeId::SetParent()
{
    return SetParent(T::GetTypeId());
}

template <typename T>
TypeId
TypeId::AddConstructor()
{
    struct Maker
    {
        static ObjectBase* Create()
        {
            ObjectBase* base = new T();
            return base;
        }
    };

    Callback<ObjectBase*> cb = MakeCallback(&Maker::Create);
    DoAddConstructor(cb);
    return *this;
}

}

#endif /* TYPE_ID_H */

