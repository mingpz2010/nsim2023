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

#include "integer.h"

#include "fatal-error.h"
#include "log.h"

#include <sstream>


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("Integer");

ATTRIBUTE_VALUE_IMPLEMENT_WITH_NAME(int64_t, Integer);

namespace internal
{

Ptr<const AttributeChecker>
MakeIntegerChecker(int64_t min, int64_t max, std::string name)
{
    NS_LOG_FUNCTION(min << max << name);

    struct IntegerChecker : public AttributeChecker
    {
        IntegerChecker(int64_t minValue, int64_t maxValue, std::string name)
            : m_minValue(minValue),
              m_maxValue(maxValue),
              m_name(name)
        {
        }

        bool Check(const AttributeValue& value) const override
        {
            NS_LOG_FUNCTION(&value);
            const IntegerValue* v = dynamic_cast<const IntegerValue*>(&value);
            if (v == nullptr)
            {
                return false;
            }
            return v->Get() >= m_minValue && v->Get() <= m_maxValue;
        }

        std::string GetValueTypeName() const override
        {
            NS_LOG_FUNCTION_NOARGS();
            return "nsim2023::IntegerValue";
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
            return nsim2023::Create<IntegerValue>();
        }

        bool Copy(const AttributeValue& src, AttributeValue& dst) const override
        {
            NS_LOG_FUNCTION(&src << &dst);
            const IntegerValue* source = dynamic_cast<const IntegerValue*>(&src);
            IntegerValue* destination = dynamic_cast<IntegerValue*>(&dst);
            if (source == nullptr || destination == nullptr)
            {
                return false;
            }
            *destination = *source;
            return true;
        }

        int64_t m_minValue;
        int64_t m_maxValue;
        std::string m_name;
    }* checker = new IntegerChecker(min, max, name);

    return Ptr<AttributeChecker>(checker, false);
}

} // namespace internal

}

