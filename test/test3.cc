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

#include "object.h"
#include "ptr.h"

#include <iostream>


using namespace nsim2023;

/**
 * Example class illustrating use of Ptr.
 */
class PtrExample : public Object
{
  public:
    /** Constructor. */
    PtrExample();
    /** Destructor. */
    ~PtrExample() override;
    /** Example class method. */
    void Method();
};

PtrExample::PtrExample()
{
    std::cout << "PtrExample constructor" << std::endl;
}

PtrExample::~PtrExample()
{
    std::cout << "PtrExample destructor" << std::endl;
}

void
PtrExample::Method()
{
    std::cout << "PtrExample method" << std::endl;
}

/**
 *  Example Ptr global variable.
 */
static Ptr<PtrExample> g_ptr = nullptr;


static Ptr<PtrExample>
StorePtr(Ptr<PtrExample> p)
{
    Ptr<PtrExample> prev = g_ptr;
    g_ptr = p;
    return prev;
}


static void
ClearPtr()
{
    g_ptr = nullptr;
}

int main(int argc, char* argv[])
{
    {
        // Create a new object of type PtrExample, store it in global
        // variable g_ptr
        Ptr<PtrExample> p = CreateObject<PtrExample>();
        p->Method();
        Ptr<PtrExample> prev = StorePtr(p);
        NS_ASSERT(!prev);
    }

    {
        // Create a new object of type PtrExample, store it in global
        // variable g_ptr, get a hold on the previous PtrExample object.
        Ptr<PtrExample> p = CreateObject<PtrExample>();
        Ptr<PtrExample> prev = StorePtr(p);
        // call method on object
        prev->Method();
        // Clear the currently-stored object
        ClearPtr();
        // get the raw pointer and release it.
        PtrExample* raw = GetPointer(prev);
        prev = nullptr;
        raw->Method();
        raw->Unref();
    }

    return 0;
}

