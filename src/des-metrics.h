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

#ifndef DESMETRICS_H
#define DESMETRICS_H


#include "nstime.h"
#include "singleton.h"

#include <fstream>
#include <mutex>
#include <stdint.h> // uint32_t
#include <string>
#include <vector>

namespace nsim2023
{

/**
 *
 * This feature generates a JSON file with event trace data,
 * including the source and destination context for each
 * event, and the (virtual) times when the event was scheduled and
 * when it will execute.
 *
 * See the DES Metrics Project page: https://github.com/wilseypa/desMetrics
 * for more information and analysis tools.
 *
 */
class DesMetrics : public Singleton<DesMetrics>
{
  public:
    /**
     * Open the DesMetrics trace file and print the header.
     *
     * The trace file will have the same base name as the main program,
     * '.json' as the extension.
     */
    void Initialize(std::vector<std::string> args, std::string outDir = "");

    /**
     * Trace an event to self at the time it is scheduled.
     */
    void Trace(const Time& now, const Time& delay);

    /**
     * Trace an event (with context) at the time it is scheduled.
     */
    void TraceWithContext(uint32_t context, const Time& now, const Time& delay);

    /**
     * Destructor, closes the trace file.
     */
    ~DesMetrics() override;

  private:
    /** Close the output file. */
    void Close();

    /**
     * Cache the last-used output directory.
     */
    static std::string m_outputDir;

    bool m_initialized;
    std::ofstream m_os;
    char m_separator;

    /** Mutex to control access to the output file. */
    std::mutex m_mutex;

}; // class DesMetrics

}

#endif /* DESMETRICS_H */

