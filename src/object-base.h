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

#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "callback.h"
#include "type-id.h"

#include <list>
#include <string>


#define NS_OBJECT_ENSURE_REGISTERED(type)                                                          \
    static struct Object##type##RegistrationClass                                                  \
    {                                                                                              \
        Object##type##RegistrationClass()                                                          \
        {                                                                                          \
            nsim2023::TypeId tid = type::GetTypeId();                                                   \
            tid.SetSize(sizeof(type));                                                             \
            tid.GetParent();                                                                       \
        }                                                                                          \
    } Object##type##RegistrationVariable


#define NS_OBJECT_TEMPLATE_CLASS_DEFINE(type, param)                                               \
    template class type<param>;                                                                    \
    template <>                                                                                    \
    std::string DoGetTemplateClassName<type<param>>()                                              \
    {                                                                                              \
        return std::string("nsim2023::") + std::string(#type) + std::string("<") +                      \
               std::string(#param) + std::string(">");                                             \
    }                                                                                              \
    static struct Object##type##param##RegistrationClass                                           \
    {                                                                                              \
        Object##type##param##RegistrationClass()                                                   \
        {                                                                                          \
            nsim2023::TypeId tid = type<param>::GetTypeId();                                            \
            tid.SetSize(sizeof(type<param>));                                                      \
            tid.GetParent();                                                                       \
        }                                                                                          \
    } Object##type##param##RegistrationVariable


#define NS_OBJECT_TEMPLATE_CLASS_TWO_DEFINE(type, param1, param2)                                  \
    template class type<param1, param2>;                                                           \
    template <>                                                                                    \
    std::string DoGetTemplateClassName<type<param1, param2>>()                                     \
    {                                                                                              \
        return std::string("nsim2023::") + std::string(#type) + std::string("<") +                      \
               std::string(#param1) + std::string(",") + std::string(#param2) + std::string(">");  \
    }                                                                                              \
    static struct Object##type##param1##param2##RegistrationClass                                  \
    {                                                                                              \
        Object##type##param1##param2##RegistrationClass()                                          \
        {                                                                                          \
            nsim2023::TypeId tid = type<param1, param2>::GetTypeId();                                   \
            tid.SetSize(sizeof(type<param1, param2>));                                             \
            tid.GetParent();                                                                       \
        }                                                                                          \
    } Object##type##param1##param2##RegistrationVariable

namespace nsim2023
{

template <typename T>
std::string DoGetTemplateClassName();


template <typename T>
std::string
GetTemplateClassName()
{
    return DoGetTemplateClassName<T>();
}

class AttributeConstructionList;


class ObjectBase
{
  public:

    static TypeId GetTypeId();

    virtual ~ObjectBase();

    virtual TypeId GetInstanceTypeId() const = 0;

    void SetAttribute(std::string name, const AttributeValue& value);

    bool SetAttributeFailSafe(std::string name, const AttributeValue& value);

    void GetAttribute(std::string name, AttributeValue& value) const;

    bool GetAttributeFailSafe(std::string name, AttributeValue& value) const;

    bool TraceConnect(std::string name, std::string context, const CallbackBase& cb);

    bool TraceConnectWithoutContext(std::string name, const CallbackBase& cb);

    bool TraceDisconnect(std::string name, std::string context, const CallbackBase& cb);

    bool TraceDisconnectWithoutContext(std::string name, const CallbackBase& cb);

  protected:

    virtual void NotifyConstructionCompleted();

    void ConstructSelf(const AttributeConstructionList& attributes);

  private:

    bool DoSet(Ptr<const AttributeAccessor> spec,
               Ptr<const AttributeChecker> checker,
               const AttributeValue& value);
};

}

#endif /* OBJECT_BASE_H */

