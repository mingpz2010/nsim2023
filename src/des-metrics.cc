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


#include "des-metrics.h"

#include "simulator.h"
#include "system-path.h"

#include <ctime> // time_t, time()
#include <sstream>
#include <string>

namespace nsim2023
{

/* static */
std::string DesMetrics::m_outputDir;

void
DesMetrics::Initialize(std::vector<std::string> args, std::string outDir /* = "" */)
{
    if (m_initialized)
    {
        // Running multiple tests, so close the previous output file
        Close();
    }

    m_initialized = true;

    std::string model_name("desTraceFile");
    if (args.size() > 0)
    {
        std::string arg0 = args[0];
        model_name = SystemPath::Split(arg0).back();
    }
    std::string jsonFile = model_name + ".json";
    if (outDir != "")
    {
        DesMetrics::m_outputDir = outDir;
    }
    if (DesMetrics::m_outputDir != "")
    {
        jsonFile = SystemPath::Append(DesMetrics::m_outputDir, jsonFile);
    }

    time_t current_time;
    time(&current_time);
    const char* date = ctime(&current_time);
    std::string capture_date(date, 24); // discard trailing newline from ctime

    m_os.open(jsonFile);
    m_os << "{" << std::endl;
    m_os << " \"simulator_name\" : \"ns-3\"," << std::endl;
    m_os << " \"model_name\" : \"" << model_name << "\"," << std::endl;
    m_os << " \"capture_date\" : \"" << capture_date << "\"," << std::endl;
    m_os << " \"command_line_arguments\" : \"";
    if (args.size() == 0)
    {
        for (std::size_t i = 0; i < args.size(); ++i)
        {
            if (i > 0)
            {
                m_os << " ";
            }
            m_os << args[i];
        }
    }
    else
    {
        m_os << "[argv empty or not available]";
    }
    m_os << "\"," << std::endl;
    m_os << " \"events\" : [" << std::endl;

    m_separator = ' ';
}

void
DesMetrics::Trace(const Time& now, const Time& delay)
{
    TraceWithContext(Simulator::GetContext(), now, delay);
}

void
DesMetrics::TraceWithContext(uint32_t context, const Time& now, const Time& delay)
{
    if (!m_initialized)
    {
        std::vector<std::string> args;
        Initialize(args);
    }

    std::ostringstream ss;
    if (m_separator == ',')
    {
        ss << m_separator << std::endl;
    }

    uint32_t sendCtx = Simulator::GetContext();
    // Force to signed so we can show NoContext as '-1'
    int32_t send = (sendCtx != Simulator::NO_CONTEXT) ? (int32_t)sendCtx : -1;
    int32_t recv = (context != Simulator::NO_CONTEXT) ? (int32_t)context : -1;

    ss << "  [\"" << send << "\",\"" << now.GetTimeStep() << "\",\"" << recv << "\",\""
       << (now + delay).GetTimeStep() << "\"]";

    {
        std::unique_lock lock{m_mutex};
        m_os << ss.str();
    }

    m_separator = ',';
}

DesMetrics::~DesMetrics()
{
    Close();
}

void
DesMetrics::Close()
{
    m_os << std::endl; // Finish the last event line

    m_os << " ]" << std::endl;
    m_os << "}" << std::endl;
    m_os.close();

    m_initialized = false;
}

}

