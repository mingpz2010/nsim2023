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
#ifndef SYSTEM_PATH_H
#define SYSTEM_PATH_H

#include <list>
#include <string>


namespace nsim2023
{


namespace SystemPath
{

/**
 * Get the file system path to the current executable.
 *
 * This path is only equivalent to the current working directory when
 * the executable is executed in its parent directory.
 */
std::string FindSelfDirectory();


std::string Append(std::string left, std::string right);

/**
 * Split a file system path into directories according to
 * the local path separator.
 */
std::list<std::string> Split(std::string path);

/**
 * Join a list of file system path directories into a single
 * file system path.
 */
std::string Join(std::list<std::string>::const_iterator begin,
                 std::list<std::string>::const_iterator end);

/**
 * Get the list of files located in a file system directory.
 */
std::list<std::string> ReadFiles(std::string path);

/**
 * Get the name of a temporary directory.
 *
 * The returned path identifies a directory which does not exist yet.
 * Call ns3::SystemPath::MakeDirectories to create it. Yes, there is a
 * well-known security race in this API but we don't care in ns-3.
 *
 * The final path to the directory is going to look something like
 *
 *     /tmp/ns3.14.30.29.32767
 *
 * The first part, "/tmp/" is the absolute path found by inspecting
 * the environment variables `TMP`and `TEMP`, in order. If neither
 * exists the hard-codes root path `/tmp/` is used.
 *
 * The directory name itself starts with the "ns3" identifier telling folks
 * who is making all of the temp directories.
 *
 * The next three number give the hour, minute and second, separated by
 * periods.
 *
 * The final number is randomly generated, to avoid name collisions.
 */
std::string MakeTemporaryDirectoryName();

/**
 * Create all the directories leading to path.
 */
void MakeDirectories(std::string path);

/**
 * Check if a path exists.
 * Path can be a file or directory.
 */
bool Exists(const std::string path);

/**
 * Replace incompatible characters in a path,
 * to get a path compatible with different
 * file systems.
 */
std::string CreateValidSystemPath(const std::string path);

} // namespace SystemPath

}

#endif /* SYSTEM_PATH_H */

