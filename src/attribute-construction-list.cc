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

#include "attribute-construction-list.h"

#include "log.h"


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("AttributeConstructionList");

AttributeConstructionList::AttributeConstructionList()
{
    NS_LOG_FUNCTION(this);
}

void
AttributeConstructionList::Add(std::string name,
                               Ptr<const AttributeChecker> checker,
                               Ptr<AttributeValue> value)
{
    // get rid of any previous value stored in this
    // vector of values.
    NS_LOG_FUNCTION(this << name << checker << value);

    for (std::list<struct Item>::iterator k = m_list.begin(); k != m_list.end(); k++)
    {
        if (k->checker == checker)
        {
            m_list.erase(k);
            break;
        }
    }
    // store the new value.
    struct Item attr;
    attr.checker = checker;
    attr.value = value;
    attr.name = name;
    m_list.push_back(attr);
}

Ptr<AttributeValue> 
AttributeConstructionList::Find(Ptr<const AttributeChecker> checker) const
{
    NS_LOG_FUNCTION(this << checker);
    for (CIterator k = m_list.begin(); k != m_list.end(); k++)
    {
        NS_LOG_DEBUG("Found " << k->name << " " << k->checker << " " << k->value);
        if (k->checker == checker)
        {
            return k->value;
        }
    }
    return nullptr;
}

AttributeConstructionList::CIterator AttributeConstructionList::Begin() const
{
    NS_LOG_FUNCTION(this);
    return m_list.begin();
}

AttributeConstructionList::CIterator AttributeConstructionList::End() const
{
    NS_LOG_FUNCTION(this);
    return m_list.end();
}

}

