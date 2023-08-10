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

#ifndef ATTRIBUTE_CONSTRUCTION_LIST_H
#define ATTRIBUTE_CONSTRUCTION_LIST_H

#include "attribute.h"

#include <list>


namespace nsim2023
{

class AttributeConstructionList
{
  public:
    /** A single Attribute triple */
    struct Item
    {
        Ptr<const AttributeChecker> checker;

        Ptr<AttributeValue> value;

        std::string name;
    };

    typedef std::list<struct Item>::const_iterator CIterator;

    AttributeConstructionList();


    void Add(std::string name, Ptr<const AttributeChecker> checker, Ptr<AttributeValue> value);


    Ptr<AttributeValue> Find(Ptr<const AttributeChecker> checker) const;


    CIterator Begin() const;

    CIterator End() const;

  private:

    std::list<struct Item> m_list;
};

}

#endif /* ATTRIBUTE_CONSTRUCTION_LIST_H */

