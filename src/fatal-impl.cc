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

#include "fatal-impl.h"

#include "log.h"

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>

#ifdef __WIN32__
struct sigaction
{
    void (*sa_handler)(int);
    int sa_flags;
    int sa_mask;
};

int
sigaction(int sig, struct sigaction* action, struct sigaction* old)
{
    if (sig == -1)
    {
        return 0;
    }
    if (old == nullptr)
    {
        if (signal(sig, SIG_DFL) == SIG_ERR)
        {
            return -1;
        }
    }
    else
    {
        if (signal(sig, action->sa_handler) == SIG_ERR)
        {
            return -1;
        }
    }
    return 0;
}
#endif

namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("FatalImpl");

namespace FatalImpl
{

 // Unnamed namespace for fatal streams memory implementation
 // and signal handler.
namespace
{

std::list<std::ostream*>** PeekStreamList()
{
    NS_LOG_FUNCTION_NOARGS();
    static std::list<std::ostream*>* streams = nullptr;
    return &streams;
}


std::list<std::ostream*>* GetStreamList()
{
    NS_LOG_FUNCTION_NOARGS();
    std::list<std::ostream*>** pstreams = PeekStreamList();
    if (*pstreams == nullptr)
    {
        *pstreams = new std::list<std::ostream*>();
    }
    return *pstreams;
}

}

void RegisterStream(std::ostream* stream)
{
    NS_LOG_FUNCTION(stream);
    GetStreamList()->push_back(stream);
}


void UnregisterStream(std::ostream* stream)
{
    NS_LOG_FUNCTION(stream);
    std::list<std::ostream*>** pl = PeekStreamList();
    if (*pl == nullptr)
    {
        return;
    }
    (*pl)->remove(stream);
    if ((*pl)->empty())
    {
        delete *pl;
        *pl = nullptr;
    }
}


// Unnamed namespace for fatal streams signal handler.
namespace
{


void sigHandler(int sig)
{
    NS_LOG_FUNCTION(sig);
    FlushStreams();
    std::abort();
}
}

void FlushStreams()
{
    NS_LOG_FUNCTION_NOARGS();
    std::list<std::ostream*>** pl = PeekStreamList();
    if (*pl == nullptr)
    {
        return;
    }

    /* Override default SIGSEGV handler - will flush subsequent
     * streams even if one of the stream pointers is bad.
     * The SIGSEGV override should only be active for the
     * duration of this function. */
    struct sigaction hdl;
    hdl.sa_handler = sigHandler;
    sigaction(SIGSEGV, &hdl, nullptr);

    std::list<std::ostream*>* l = *pl;

    /* Need to do it this way in case any of the ostream* causes SIGSEGV */
    while (!l->empty())
    {
        std::ostream* s(l->front());
        l->pop_front();
        s->flush();
    }

    /* Restore default SIGSEGV handler (Not that it matters anyway) */
    hdl.sa_handler = SIG_DFL;
    sigaction(SIGSEGV, &hdl, nullptr);

    /* Flush all opened FILE* */
    std::fflush(nullptr);

    /* Flush stdandard streams - shouldn't be required (except for clog) */
    std::cout.flush();
    std::cerr.flush();
    std::clog.flush();

    delete l;
    *pl = nullptr;
}

} // namespace FatalImpl

}

