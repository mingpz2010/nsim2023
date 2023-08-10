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

#include "uinteger.h"

#include "fatal-error.h"
#include "log.h"

#include <sstream>


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("Uinteger");

ATTRIBUTE_VALUE_IMPLEMENT_WITH_NAME(uint64_t, Uinteger);

namespace internal
{

/**
 * Make an Uinteger attribute checker with embedded numeric type name.
 */
Ptr<const AttributeChecker>
MakeUintegerChecker(uint64_t min, uint64_t max, std::string name)
{
    NS_LOG_FUNCTION(min << max << name);

    struct Checker : public AttributeChecker
    {
        Checker(uint64_t minValue, uint64_t maxValue, std::string name)
            : m_minValue(minValue),
              m_maxValue(maxValue),
              m_name(name)
        {
        }

        bool Check(const AttributeValue& value) const override
        {
            NS_LOG_FUNCTION(&value);
            const UintegerValue* v = dynamic_cast<const UintegerValue*>(&value);
            if (v == nullptr)
            {
                return false;
            }
            return v->Get() >= m_minValue && v->Get() <= m_maxValue;
        }

        std::string GetValueTypeName() const override
        {
            NS_LOG_FUNCTION_NOARGS();
            return "nsim2023::UintegerValue";
        }

        bool HasUnderlyingTypeInformation() const override
        {
            NS_LOG_FUNCTION_NOARGS();
            return true;
        }

        std::string GetUnderlyingTypeInformation() const override
        {
            NS_LOG_FUNCTION_NOARGS();
            std::ostringstream oss;
            oss << m_name << " " << m_minValue << ":" << m_maxValue;
            return oss.str();
        }

        Ptr<AttributeValue> Create() const override
        {
            NS_LOG_FUNCTION_NOARGS();
            return nsim2023::Create<UintegerValue>();
        }

        bool Copy(const AttributeValue& source, AttributeValue& destination) const override
        {
            NS_LOG_FUNCTION(&source << &destination);
            const UintegerValue* src = dynamic_cast<const UintegerValue*>(&source);
            UintegerValue* dst = dynamic_cast<UintegerValue*>(&destination);
            if (src == nullptr || dst == nullptr)
            {
                return false;
            }
            *dst = *src;
            return true;
        }

        uint64_t m_minValue;
        uint64_t m_maxValue;
        std::string m_name;
    }* checker = new Checker(min, max, name);

    return Ptr<const AttributeChecker>(checker, false);
}

} // namespace internal

}

