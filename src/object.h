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
#ifndef OBJECT_H
#define OBJECT_H

#include "attribute-construction-list.h"
#include "attribute.h"
#include "object-base.h"
#include "ptr.h"
#include "simple-ref-count.h"

#include <stdint.h>
#include <string>
#include <vector>


namespace nsim2023
{

class Object;
class AttributeAccessor;
class AttributeValue;
class TraceSourceAccessor;


struct ObjectDeleter
{
    /**
     * Smart pointer deleter implementation for Object.
     *
     * Delete implementation, forwards to the Object::DoDelete()
     * method.
     */
    inline static void Delete(Object* object);
};

/**
 * The memory management scheme is based on reference-counting with
 * dispose-like functionality to break the reference cycles.
 * The reference count is incremented and decremented with
 * the methods Ref() and Unref(). If a reference cycle is
 * present, the user is responsible for breaking it
 * by calling Dispose() in a single location. This will
 * eventually trigger the invocation of DoDispose() on itself and
 * all its aggregates. The DoDispose() method is always automatically
 * invoked from the Unref() method before destroying the Object,
 * even if the user did not call Dispose() directly.
 */
class Object : public SimpleRefCount<Object, ObjectBase, ObjectDeleter>
{
  public:

    static TypeId GetTypeId();

    class AggregateIterator
    {
      public:
        /** Default constructor, which has no Object. */
        AggregateIterator();

        bool HasNext() const;

        Ptr<const Object> Next();

      private:

        friend class Object;

        AggregateIterator(Ptr<const Object> object);
        Ptr<const Object> m_object; //!< Parent Object.
        uint32_t m_current;         //!< Current position in parent's aggregates.
    };

    /** Constructor. */
    Object();
    /** Destructor. */
    ~Object() override;

    TypeId GetInstanceTypeId() const override;

    template <typename T>
    inline Ptr<T> GetObject() const;

    template <typename T>
    Ptr<T> GetObject(TypeId tid) const;

    void Dispose();

    void AggregateObject(Ptr<Object> other);


    AggregateIterator GetAggregateIterator() const;


    void Initialize();

    /**
     * Check if the object has been initialized.
     *
     * \brief Check if the object has been initialized.
     * \returns \c true if the object has been initialized.
     */
    bool IsInitialized() const;

  protected:
    /**
     * Notify all Objects aggregated to this one of a new Object being
     * aggregated.
     *
     * This method is invoked whenever two sets of Objects are aggregated
     * together.  It is invoked exactly once for each Object in both sets.
     * This method can be overridden by subclasses who wish to be notified
     * of aggregation events. These subclasses must chain up to their
     * base class NotifyNewAggregate() method.
     *
     * It is safe to call GetObject() and AggregateObject() from within
     * this method.
     */
    virtual void NotifyNewAggregate();
    /**
     * Initialize() implementation.
     *
     * This method is called only once by Initialize(). If the user
     * calls Initialize() multiple times, DoInitialize() is called only the
     * first time.
     *
     * Subclasses are expected to override this method and chain up
     * to their parent's implementation once they are done. It is
     * safe to call GetObject() and AggregateObject() from within this method.
     */
    virtual void DoInitialize();
    /**
     * Destructor implementation.
     *
     * This method is called by Dispose() or by the Object's
     * destructor, whichever comes first.
     *
     * Subclasses are expected to implement their real destruction
     * code in an overridden version of this method and chain
     * up to their parent's implementation once they are done.
     * _i.e_, for simplicity, the destructor of every subclass should
     * be empty and its content should be moved to the associated
     * DoDispose() method.
     *
     * It is safe to call GetObject() from within this method.
     */
    virtual void DoDispose();
    /**
     * Copy an Object.
     *
     * \param [in] o the Object to copy.
     *
     * Allow subclasses to implement a copy constructor.
     *
     * While it is technically possible to implement a copy
     * constructor in a subclass, we strongly discourage you
     * from doing so. If you really want to do it anyway, you have
     * to understand that this copy constructor will _not_
     * copy aggregated Objects, _i.e_, if your Object instance
     * is already aggregated to another Object and if you invoke
     * this copy constructor, the new Object instance will be
     * a pristine standalone Object instance not aggregated to
     * any other Object. It is thus _your_ responsibility
     * as a caller of this method to do what needs to be done
     * (if it is needed) to ensure that the Object stays in a
     * valid state.
     */
    Object(const Object& o);

  private:

    template <typename T>
    friend Ptr<T> CopyObject(Ptr<T> object);

    template <typename T>
    friend Ptr<T> CopyObject(Ptr<const T> object);


    template <typename T>
    friend Ptr<T> CompleteConstruct(T* object);


    friend class ObjectFactory;
    friend class AggregateIterator;
    friend struct ObjectDeleter;


    /**
     * The list of Objects aggregated to this one.
     *
     * This data structure uses a classic C-style trick to
     * hold an array of variable size without performing
     * two memory allocations: the declaration of the structure
     * declares a one-element array but when we allocate
     * memory for this struct, we effectively allocate a larger
     * chunk of memory than the struct to allow space for a larger
     * variable sized buffer whose size is indicated by the element
     */
    struct Aggregates
    {
        /** The number of entries in buffer. */
        uint32_t n;
        /** The array of Objects. */
        Object* buffer[1];
    };

    /**
     * Find an Object of TypeId tid in the aggregates of this Object.
     */
    Ptr<Object> DoGetObject(TypeId tid) const;

    /**
     * Verify that this Object is still live, by checking it's reference count.
     */
    bool Check() const;

    bool CheckLoose() const;

    /**
     * Set the TypeId of this Object.
     */
    void SetTypeId(TypeId tid);

    /**
     * Initialize all member variables registered as Attributes of this TypeId.
     */
    void Construct(const AttributeConstructionList& attributes);

    /**
     * Keep the list of aggregates in most-recently-used order
     */
    void UpdateSortedArray(struct Aggregates* aggregates, uint32_t i) const;

    /**
     * Attempt to delete this Object.
     *
     * This method iterates over all aggregated Objects to check if they all
     * have a zero refcount. If yes, the Object and all
     * its aggregates are deleted. If not, nothing is done.
     */
    void DoDelete();

    /**
     * Identifies the type of this Object instance.
     */
    TypeId m_tid;


    bool m_disposed;

    bool m_initialized;
    /**
     * A pointer to an array of 'aggregates'.
     *
     * A pointer to each Object aggregated to this Object is stored in this
     * array.  The array is shared by all aggregated Objects
     * so the size of the array is indirectly a reference count.
     */
    struct Aggregates* m_aggregates;
    /**
     * The number of times the Object was accessed with a
     * call to GetObject().
     *
     * This integer is used to implement a heuristic to sort
     * the array of aggregates in most-frequently accessed order.
     */
    uint32_t m_getObjectCount;
};

template <typename T>
Ptr<T> CopyObject(Ptr<const T> object);
template <typename T>
Ptr<T> CopyObject(Ptr<T> object);

}

namespace nsim2023
{

/*************************************************************************
 *   The Object implementation which depends on templates
 *************************************************************************/
void
ObjectDeleter::Delete(Object* object)
{
    object->DoDelete();
}

template <typename T>
Ptr<T>
Object::GetObject() const
{
    // This is an optimization: if the cast works (which is likely),
    // things will be pretty fast.
    T* result = dynamic_cast<T*>(m_aggregates->buffer[0]);
    if (result != nullptr)
    {
        return Ptr<T>(result);
    }
    // if the cast does not work, we try to do a full type check.
    Ptr<Object> found = DoGetObject(T::GetTypeId());
    if (found)
    {
        return Ptr<T>(static_cast<T*>(PeekPointer(found)));
    }
    return nullptr;
}


template <>
inline Ptr<Object>
Object::GetObject() const
{
    return Ptr<Object>(const_cast<Object*>(this));
}

template <typename T>
Ptr<T>
Object::GetObject(TypeId tid) const
{
    Ptr<Object> found = DoGetObject(tid);
    if (found)
    {
        return Ptr<T>(static_cast<T*>(PeekPointer(found)));
    }
    return nullptr;
}


template <>
inline Ptr<Object>
Object::GetObject(TypeId tid) const
{
    if (tid == Object::GetTypeId())
    {
        return Ptr<Object>(const_cast<Object*>(this));
    }
    else
    {
        return DoGetObject(tid);
    }
}

/*************************************************************************
 *   The helper functions which need templates.
 *************************************************************************/
template <typename T>
Ptr<T>
CopyObject(Ptr<T> object)
{
    Ptr<T> p = Ptr<T>(new T(*PeekPointer(object)), false);
    NS_ASSERT(p->GetInstanceTypeId() == object->GetInstanceTypeId());
    return p;
}

template <typename T>
Ptr<T>
CopyObject(Ptr<const T> object)
{
    Ptr<T> p = Ptr<T>(new T(*PeekPointer(object)), false);
    NS_ASSERT(p->GetInstanceTypeId() == object->GetInstanceTypeId());
    return p;
}

template <typename T>
Ptr<T>
CompleteConstruct(T* object)
{
    object->SetTypeId(T::GetTypeId());
    object->Object::Construct(AttributeConstructionList());
    return Ptr<T>(object, false);
}


template <typename T, typename... Args>
Ptr<T>
CreateObject(Args&&... args)
{
    return CompleteConstruct(new T(std::forward<Args>(args)...));
}

}

#endif /* OBJECT_H */

