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
#include "system-path.h"

#include "assert.h"
#include "fatal-error.h"
#include "log.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <regex>
#include <sstream>
#include <tuple>

// Some compilers such as GCC < 8 (Ubuntu 18.04
// ships with GCC 7) do not ship with the
// std::filesystem header,  but with the
// std::experimental::filesystem header.
// Since Clang reuses these headers and the libstdc++
// from GCC, we need to either use the experimental
// version or require a more up-to-date GCC.
// we use the "fs" namespace to prevent collisions
// with musl libc.
#ifdef __has_include
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error "No support for filesystem library"
#endif
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif /* __APPLE__ */

#ifdef __FreeBSD__
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

#ifdef __linux__
#include <unistd.h>
#endif

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <regex>
#include <windows.h>
#endif

/**
 * System-specific path separator used between directory names.
 */
#if defined(__WIN32__)
constexpr auto SYSTEM_PATH_SEP = "\\";
#else
constexpr auto SYSTEM_PATH_SEP = "/";
#endif


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("SystemPath");

// unnamed namespace for internal linkage
namespace
{
/**
 * Get the list of files located in a file system directory with error.
 */
std::tuple<std::list<std::string>, bool>
ReadFilesNoThrow(std::string path)
{
    NS_LOG_FUNCTION(path);
    std::list<std::string> files;
    if (!fs::exists(path))
    {
        return std::make_tuple(files, true);
    }
    for (auto& it : fs::directory_iterator(path))
    {
        if (!fs::is_directory(it.path()))
        {
            files.push_back(it.path().filename().string());
        }
    }
    return std::make_tuple(files, false);
}

} // unnamed namespace

namespace SystemPath
{

/**
 * Get the directory path for a file.
 */
std::string
Dirname(std::string path)
{
    NS_LOG_FUNCTION(path);
    std::list<std::string> elements = Split(path);
    std::list<std::string>::const_iterator last = elements.end();
    last--;
    return Join(elements.begin(), last);
}

std::string
FindSelfDirectory()
{
    /**
     * This function returns the path to the running $PREFIX.
     * Mac OS X: _NSGetExecutablePath() (man 3 dyld)
     * Linux: readlink /proc/self/exe
     * Solaris: getexecname()
     * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
     * BSD with procfs: readlink /proc/curproc/file
     * Windows: GetModuleFileName() with hModule = NULL
     */
    NS_LOG_FUNCTION_NOARGS();
    std::string filename;
#if defined(__linux__)
    {
        ssize_t size = 1024;
        char* buffer = (char*)malloc(size);
        memset(buffer, 0, size);
        int status;
        while (true)
        {
            status = readlink("/proc/self/exe", buffer, size);
            if (status != 1 || (status == -1 && errno != ENAMETOOLONG))
            {
                break;
            }
            size *= 2;
            free(buffer);
            buffer = (char*)malloc(size);
            memset(buffer, 0, size);
        }
        if (status == -1)
        {
            NS_FATAL_ERROR("Oops, could not find self directory.");
        }
        filename = buffer;
        free(buffer);
    }
#elif defined(__WIN32__)
    {
        //  LPTSTR = char *
        DWORD size = 1024;
        LPTSTR lpFilename = (LPTSTR)malloc(sizeof(TCHAR) * size);
        DWORD status = GetModuleFileName(nullptr, lpFilename, size);
        while (status == size)
        {
            size = size * 2;
            free(lpFilename);
            lpFilename = (LPTSTR)malloc(sizeof(TCHAR) * size);
            status = GetModuleFileName(nullptr, lpFilename, size);
        }
        NS_ASSERT(status != 0);
        filename = lpFilename;
        free(lpFilename);
    }
#elif defined(__APPLE__)
    {
        uint32_t bufsize = 1024;
        char* buffer = (char*)malloc(bufsize);
        NS_ASSERT(buffer);
        int status = _NSGetExecutablePath(buffer, &bufsize);
        if (status == -1)
        {
            free(buffer);
            buffer = (char*)malloc(bufsize);
            status = _NSGetExecutablePath(buffer, &bufsize);
        }
        NS_ASSERT(status == 0);
        filename = buffer;
        free(buffer);
    }
#elif defined(__FreeBSD__)
    {
        int mib[4];
        std::size_t bufSize = 1024;
        char* buf = (char*)malloc(bufSize);

        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PATHNAME;
        mib[3] = -1;

        sysctl(mib, 4, buf, &bufSize, nullptr, 0);
        filename = buf;
    }
#endif
    return Dirname(filename);
}

std::string
Append(std::string left, std::string right)
{
    // removing trailing separators from 'left'
    NS_LOG_FUNCTION(left << right);
    while (true)
    {
        std::string::size_type lastSep = left.rfind(SYSTEM_PATH_SEP);
        if (lastSep != left.size() - 1)
        {
            break;
        }
        left = left.substr(0, left.size() - 1);
    }
    std::string retval = left + SYSTEM_PATH_SEP + right;
    return retval;
}

std::list<std::string>
Split(std::string path)
{
    NS_LOG_FUNCTION(path);
    std::list<std::string> retval;
    std::string::size_type current = 0;
    std::string::size_type next = 0;
    next = path.find(SYSTEM_PATH_SEP, current);
    while (next != std::string::npos)
    {
        std::string item = path.substr(current, next - current);
        retval.push_back(item);
        current = next + 1;
        next = path.find(SYSTEM_PATH_SEP, current);
    }
    std::string item = path.substr(current, next - current);
    retval.push_back(item);
    return retval;
}

std::string
Join(std::list<std::string>::const_iterator begin, std::list<std::string>::const_iterator end)
{
    NS_LOG_FUNCTION(*begin << *end);
    std::string retval = "";
    for (std::list<std::string>::const_iterator i = begin; i != end; i++)
    {
        if (*i == "")
        {
            // skip empty strings in the path list
            continue;
        }
        else if (i == begin)
        {
            retval = *i;
        }
        else
        {
            retval = retval + SYSTEM_PATH_SEP + *i;
        }
    }
    return retval;
}

std::list<std::string>
ReadFiles(std::string path)
{
    NS_LOG_FUNCTION(path);
    bool err;
    std::list<std::string> files;
    std::tie(files, err) = ReadFilesNoThrow(path);
    if (err)
    {
        NS_FATAL_ERROR("Could not open directory=" << path);
    }
    return files;
}

std::string
MakeTemporaryDirectoryName()
{
    NS_LOG_FUNCTION_NOARGS();
    char* path = nullptr;

    path = std::getenv("TMP");
    if (!path || std::strlen(path) == 0)
    {
        path = std::getenv("TEMP");
        if (!path || std::strlen(path) == 0)
        {
            path = const_cast<char*>("/tmp");
        }
    }

    //
    // Just in case the user wants to go back and find the output, we give
    // a hint as to which dir we created by including a time hint.
    //
    time_t now = time(nullptr);
    struct tm* tm_now = localtime(&now);
    //
    // But we also randomize the name in case there are multiple users doing
    // this at the same time
    //
    srand(time(nullptr));
    long int n = rand();

    //
    // The final path to the directory is going to look something like
    //
    //   /tmp/ns3.14.30.29.32767
    //
    // The first segment comes from one of the temporary directory env
    // variables or /tmp if not found.  The directory name starts with an
    // identifier telling folks who is making all of the temp directories
    // and then the local time (in this case 14.30.29 -- which is 2:30 and
    // 29 seconds PM).
    //
    std::ostringstream oss;
    oss << path << SYSTEM_PATH_SEP << "ns-3." << tm_now->tm_hour << "." << tm_now->tm_min << "."
        << tm_now->tm_sec << "." << n;

    return oss.str();
}

void
MakeDirectories(std::string path)
{
    NS_LOG_FUNCTION(path);

    std::error_code ec;
    if (!fs::exists(path))
    {
        fs::create_directories(path, ec);
    }

    if (ec.value())
    {
        NS_FATAL_ERROR("failed creating directory " << path);
    }
}

bool
Exists(const std::string path)
{
    NS_LOG_FUNCTION(path);

    bool err;
    auto dirpath = Dirname(path);
    std::list<std::string> files;
    tie(files, err) = ReadFilesNoThrow(dirpath);
    if (err)
    {
        // Directory doesn't exist
        NS_LOG_LOGIC("directory doesn't exist: " << dirpath);
        return false;
    }
    NS_LOG_LOGIC("directory exists: " << dirpath);

    // Check if the file itself exists
    auto tokens = Split(path);
    std::string file = tokens.back();

    if (file == "")
    {
        // Last component was a directory, not a file name
        // We already checked that the directory exists,
        // so return true
        NS_LOG_LOGIC("directory path exists: " << path);
        return true;
    }

    files = ReadFiles(dirpath);

    auto it = std::find(files.begin(), files.end(), file);
    if (it == files.end())
    {
        // File itself doesn't exist
        NS_LOG_LOGIC("file itself doesn't exist: " << file);
        return false;
    }

    NS_LOG_LOGIC("file itself exists: " << file);
    return true;

} // Exists()

std::string
CreateValidSystemPath(const std::string path)
{
    // Windows and its file systems, e.g. NTFS and (ex)FAT(12|16|32),
    // do not like paths with empty spaces or special symbols.
    // Some of these symbols are allowed in test names, checked in TestCase::AddTestCase.
    // We replace them with underlines to ensure they work on Windows.
    std::regex incompatible_characters(" |:[^\\\\]|<|>|\\*");
    std::string valid_path;
    std::regex_replace(std::back_inserter(valid_path),
                       path.begin(),
                       path.end(),
                       incompatible_characters,
                       "_");
    return valid_path;
} // CreateValidSystemPath

} // namespace SystemPath

}

