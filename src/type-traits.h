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

#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include "ptr.h"

/**
 * Introspection declaration and template implementation.
 */
namespace nsim2023
{

template <typename T>
struct TypeTraits
{
  private:

    struct NullType
    {
    };


    template <typename U>
    struct UnConst
    {
        typedef U Result;
    };


    template <typename U>
    struct UnConst<const U>
    {
        typedef U Result; /**< Non-const type.       */
    };


    template <typename U>
    struct ReferenceTraits
    {
        /** Value. */
        enum
        {
            IsReference = 0 /**< Not a reference type. */
        };

        typedef U ReferencedType; /**< Base type.            */
    };


    template <typename U>
    struct ReferenceTraits<U&>
    {
        /** Value. */
        enum
        {
            IsReference = 1 /**< Reference type.       */
        };

        typedef U ReferencedType; /**< Base type.            */
    };


    template <typename U>
    struct PointerTraits
    {
        /** Value. */
        enum
        {
            IsPointer = 0, /**< Not a pointer type.   */
            IsPtr = 0      /**< Not a Ptr type.       */
        };

        typedef U PointeeType; /**< Base type.            */
    };


    template <typename U>
    struct PointerTraits<U*>
    {
        /** Value. */
        enum
        {
            IsPointer = 1, /**< Pointer type.         */
            IsPtr = 0      /**< Not a Ptr type.       */
        };

        typedef U PointeeType; /**< Pointee type.         */
    };


    template <typename U>
    struct PointerTraits<nsim2023::Ptr<U>>
    {
        /** Value. */
        enum
        {
            IsPointer = 0, /**< Not a pointer type.   */
            IsPtr = 1      /**< Ptr type.             */
        };

        typedef U PointeeType; /**< Pointee type.         */
    };


    template <typename U>
    struct Base
    {
        typedef U Type; /**< Base type.            */
    };


    template <typename U>
    struct Base<U&>
    {
        typedef typename Base<U>::Type Type; /**< Base type.            */
    };


    template <typename U>
    struct Base<U*>
    {
        typedef typename Base<U>::Type Type; /**< Base type.            */
    };


    template <typename U>
    struct Base<const U>
    {
        typedef typename Base<U>::Type Type; /**< Base type.            */
    };


    template <typename U>
    struct PtrBase
    {
        typedef U Type; /**< Base type.            */
    };


    template <typename U>
    struct PtrBase<nsim2023::Ptr<U>>
    {
        typedef U Type; /**< Base type.            */
    };


    template <typename U>
    struct FunctionPtrTraits
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 0 /**< Not a function pointer. */
        };

        typedef NullType ReturnType; /**< Return type.            */
    };


    template <typename U>
    struct FunctionPtrTraits<U (*)()>
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 1 /**< Function pointer.     */
        };

        /** Value. */ enum
        {
            nArgs = 0 /**< Number of arguments.  */
        };

        typedef U ReturnType; /**< Return type.          */
    };


    template <typename U, typename V1>
    struct FunctionPtrTraits<U (*)(V1)>
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 1 /**< Function pointer.     */
        };

        /** Value. */ enum
        {
            nArgs = 1 /**< Number of arguments.  */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef V1 Arg1Type;  /**< First argument type.  */
    };


    template <typename U, typename V1, typename V2>
    struct FunctionPtrTraits<U (*)(V1, V2)>
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 1 /**< Function pointer.     */
        };

        /** Value. */ enum
        {
            nArgs = 2 /**< Number of arguments.  */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef V1 Arg1Type;  /**< First argument type.  */
        typedef V2 Arg2Type;  /**< Second argument type. */
    };


    template <typename U, typename V1, typename V2, typename V3>
    struct FunctionPtrTraits<U (*)(V1, V2, V3)>
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 1 /**< Function pointer.     */
        };

        /** Value. */ enum
        {
            nArgs = 3 /**< Number of arguments.  */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef V1 Arg1Type;  /**< First argument type.  */
        typedef V2 Arg2Type;  /**< Second argument type. */
        typedef V3 Arg3Type;  /**< Third argument type.  */
    };


    template <typename U, typename V1, typename V2, typename V3, typename V4>
    struct FunctionPtrTraits<U (*)(V1, V2, V3, V4)>
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 1 /**< Function pointer.     */
        };

        /** Value. */ enum
        {
            nArgs = 4 /**< Number of arguments.  */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef V1 Arg1Type;  /**< First argument type.  */
        typedef V2 Arg2Type;  /**< Second argument type. */
        typedef V3 Arg3Type;  /**< Third argument type.  */
        typedef V4 Arg4Type;  /**< Fourth argument type. */
    };


    template <typename U, typename V1, typename V2, typename V3, typename V4, typename V5>
    struct FunctionPtrTraits<U (*)(V1, V2, V3, V4, V5)>
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 1 /**< Function pointer.     */
        };

        /** Value. */ enum
        {
            nArgs = 5 /**< Number of arguments.  */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef V1 Arg1Type;  /**< First argument type.  */
        typedef V2 Arg2Type;  /**< Second argument type. */
        typedef V3 Arg3Type;  /**< Third argument type.  */
        typedef V4 Arg4Type;  /**< Fourth argument type. */
        typedef V5 Arg5Type;  /**< Fifth argument type.  */
    };


    template <typename U,
              typename V1,
              typename V2,
              typename V3,
              typename V4,
              typename V5,
              typename V6>
    struct FunctionPtrTraits<U (*)(V1, V2, V3, V4, V5, V6)>
    {
        /** Value. */
        enum
        {
            IsFunctionPointer = 1 /**< Function pointer.     */
        };

        /** Value. */ enum
        {
            nArgs = 6 /**< Number of arguments.  */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef V1 Arg1Type;  /**< First argument type.  */
        typedef V2 Arg2Type;  /**< Second argument type. */
        typedef V3 Arg3Type;  /**< Third argument type.  */
        typedef V4 Arg4Type;  /**< Fourth argument type. */
        typedef V5 Arg5Type;  /**< Fifth argument type.  */
        typedef V6 Arg6Type;  /**< Sixth argument type.  */
    };


    template <typename U>
    struct PtrToMemberTraits
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 0 /**< Not a pointer to member.    */
        };
    };


    template <typename U, typename V>
    struct PtrToMemberTraits<U (V::*)()>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 0 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.                */
    };


    template <typename U, typename V>
    struct PtrToMemberTraits<U (V::*)() const>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 0 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.                */
    };


    template <typename U, typename V, typename W1>
    struct PtrToMemberTraits<U (V::*)(W1)>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 1 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
    };


    template <typename U, typename V, typename W1>
    struct PtrToMemberTraits<U (V::*)(W1) const>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 1 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
    };


    template <typename U, typename V, typename W1, typename W2>
    struct PtrToMemberTraits<U (V::*)(W1, W2)>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 2 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
    };


    template <typename U, typename V, typename W1, typename W2>
    struct PtrToMemberTraits<U (V::*)(W1, W2) const>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 2 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
    };


    template <typename U, typename V, typename W1, typename W2, typename W3>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3)>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 3 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
    };


    template <typename U, typename V, typename W1, typename W2, typename W3>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3) const>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 3 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
    };


    template <typename U, typename V, typename W1, typename W2, typename W3, typename W4>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3, W4)>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 4 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
        typedef W4 Arg4Type;  /**< Fourth argument type. */
    };


    template <typename U, typename V, typename W1, typename W2, typename W3, typename W4>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3, W4) const>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 4 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
        typedef W4 Arg4Type;  /**< Fourth argument type. */
    };


    template <typename U,
              typename V,
              typename W1,
              typename W2,
              typename W3,
              typename W4,
              typename W5>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3, W4, W5)>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 5 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
        typedef W4 Arg4Type;  /**< Fourth argument type. */
        typedef W5 Arg5Type;  /**< Fifth argument type.  */
    };


    template <typename U,
              typename V,
              typename W1,
              typename W2,
              typename W3,
              typename W4,
              typename W5>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3, W4, W5) const>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 5 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
        typedef W4 Arg4Type;  /**< Fourth argument type. */
        typedef W5 Arg5Type;  /**< Fifth argument type.  */
    };


    template <typename U,
              typename V,
              typename W1,
              typename W2,
              typename W3,
              typename W4,
              typename W5,
              typename W6>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3, W4, W5, W6)>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 6 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
        typedef W4 Arg4Type;  /**< Fourth argument type. */
        typedef W5 Arg5Type;  /**< Fifth argument type.  */
        typedef W6 Arg6Type;  /**< Sixth argument type.  */
    };


    template <typename U,
              typename V,
              typename W1,
              typename W2,
              typename W3,
              typename W4,
              typename W5,
              typename W6>
    struct PtrToMemberTraits<U (V::*)(W1, W2, W3, W4, W5, W6) const>
    {
        /** Value. */
        enum
        {
            IsPointerToMember = 1 /**< Pointer to member function. */
        };

        /** Value. */ enum
        {
            nArgs = 6 /**< Number of arguments.        */
        };

        typedef U ReturnType; /**< Return type.          */
        typedef W1 Arg1Type;  /**< First argument type.  */
        typedef W2 Arg2Type;  /**< Second argument type. */
        typedef W3 Arg3Type;  /**< Third argument type.  */
        typedef W4 Arg4Type;  /**< Fourth argument type. */
        typedef W5 Arg5Type;  /**< Fifth argument type.  */
        typedef W6 Arg6Type;  /**< Sixth argument type.  */
    };

  public:
    /** Not a const type. */
    typedef typename UnConst<T>::Result NonConstType;
    /** Referenced type. */
    typedef typename ReferenceTraits<T>::ReferencedType ReferencedType;
    /** Pointee type. */
    typedef typename PointerTraits<T>::PointeeType PointeeType;
    /** Base type, after removing \c &, \c * and \c const. */
    typedef typename Base<T>::Type BaseType;
    /** Ptr base type. */
    typedef typename PtrBase<T>::Type PtrBaseType;

    /** Predicates. */
    enum
    {
        /** Pointer to member predicate. */
        IsPointerToMember = PtrToMemberTraits<T>::IsPointerToMember,
        /** Pointer predicate. */
        IsPointer = PointerTraits<T>::IsPointer,
        /** Ptr predicate. */
        IsPtr = PointerTraits<T>::IsPtr,
        /** Reference predicate. */
        IsReference = ReferenceTraits<T>::IsReference,
        /** Function pointer predicate. */
        IsFunctionPointer = FunctionPtrTraits<T>::IsFunctionPointer
    };

    /** Pointer to member traits type. */
    typedef PtrToMemberTraits<T> PointerToMemberTraits;
    /** Function pointer traits. */
    typedef FunctionPtrTraits<T> FunctionPointerTraits;
};

}

#endif /* TYPE_TRAITS_H */

