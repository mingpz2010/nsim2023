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

#ifndef DEFAULT_DELETER_H
#define DEFAULT_DELETER_H


namespace nsim2023
{

/**
 * \ingroup ptr
 * \brief A template used to delete objects
 *        by the ns3::SimpleRefCount templates when the
 *        last reference to an object they manage
 *        disappears.
 *
 * \tparam T \deduced The object type being deleted.
 * \sa ns3::SimpleRefCount
 */
template <typename T>
struct DefaultDeleter
{
    /**
     * The default deleter implementation, which just does a normal
     * \code
     *   delete object;
     * \endcode
     * \tparam T \deduced The object type being deleted.
     * \param [in] object The object to delete.
     */
    inline static void Delete(T* object)
    {
        delete object;
    }
};

}

#endif /* DEFAULT_DELETER_H */
