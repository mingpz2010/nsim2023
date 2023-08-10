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

#ifndef GLOBAL_VALUE_H
#define GLOBAL_VALUE_H

#include "attribute.h"
#include "ptr.h"
#include "string"

#include <vector>


namespace nsim2023
{

/* Forward declaration */
namespace tests
{
class GlobalValueTestCase;
}

class GlobalValue
{
    /** Container type for holding all the GlobalValues. */
    typedef std::vector<GlobalValue*> Vector;

  public:
    /** Iterator type for the list of all global values. */
    typedef Vector::const_iterator Iterator;

    GlobalValue(std::string name,
                std::string help,
                const AttributeValue& initialValue,
                Ptr<const AttributeChecker> checker);

    std::string GetName() const;

    std::string GetHelp() const;

    void GetValue(AttributeValue& value) const;

    Ptr<const AttributeChecker> GetChecker() const;

    bool SetValue(const AttributeValue& value);

    /** Reset to the initial value. */
    void ResetInitialValue();

    static void Bind(std::string name, const AttributeValue& value);

    /**
     * Iterate over the set of GlobalValues until a matching name is found
     * and then set its value with GlobalValue::SetValue.
     */
    static bool BindFailSafe(std::string name, const AttributeValue& value);

    static Iterator Begin();

    static Iterator End();

    /**
     * Finds the GlobalValue with the given name and returns its value
     */
    static bool GetValueByNameFailSafe(std::string name, AttributeValue& value);

    /**
     * Finds the GlobalValue with the given name and returns its
     * value.
     *
     * This method cannot fail, i.e., it will trigger a
     * NS_FATAL_ERROR if the requested GlobalValue is not found.
     */
    static void GetValueByName(std::string name, AttributeValue& value);

  private:

    friend class tests::GlobalValueTestCase;

    static Vector* GetVector();

    /** Initialize from the NS_GLOBAL_VALUE environment variable. */
    void InitializeFromEnv();

    /** The name of this GlobalValue. */
    std::string m_name;
    /** The help string. */
    std::string m_help;
    /** The initial value. */
    Ptr<AttributeValue> m_initialValue;
    /** The current value. */
    Ptr<AttributeValue> m_currentValue;
    /** The AttributeChecker for this GlobalValue. */
    Ptr<const AttributeChecker> m_checker;
};

}

#endif /* GLOBAL_VALUE_H */

