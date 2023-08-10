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

#include "time-printer.h"

#include "log.h"
#include "nstime.h"
#include "simulator.h"

#include <iomanip>


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("TimePrinter");

void DefaultTimePrinter(std::ostream& os)
{
    std::ios_base::fmtflags ff = os.flags();
    std::streamsize oldPrecision = os.precision();
    
    os << std::fixed;
    switch (Time::GetResolution())
    {
    case Time::US:
        os << std::setprecision(6);
        break;
    case Time::NS:
        os << std::setprecision(9);
        break;
    case Time::PS:
        os << std::setprecision(12);
        break;
    case Time::FS:
        os << std::setprecision(15);
        break;
    default:
        // default C++ precision of 5
        os << std::setprecision(5);
    }
    os << Simulator::Now().As(Time::S);

    os << std::setprecision(oldPrecision);
    os.flags(ff); // Restore stream flags
}

}

