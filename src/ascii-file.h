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

#ifndef ASCII_FILE_H
#define ASCII_FILE_H

#include <fstream>
#include <stdint.h>
#include <string>

namespace nsim2023
{

// This class represents an ascii file
class AsciiFile
{
  public:
    AsciiFile();
    ~AsciiFile();

    bool Fail() const;

    bool Eof() const;

    void Open(const std::string& filename, std::ios::openmode mode);

    void Close();

    void Read(std::string& line);

    // Compare two ASCII files line-by-line
    // true if files are different, false otherwise
    static bool Diff(const std::string& f1, const std::string& f2, uint64_t& lineNumber);

  private:
    std::string m_filename; //!< output file name
    std::fstream m_file;    //!< output file
};

}

#endif /* ASCII_FILE_H */

