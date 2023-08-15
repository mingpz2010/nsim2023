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

#ifndef TRACED_CALLBACK_H
#define TRACED_CALLBACK_H

#include "callback.h"

#include <list>

/**
 * nsim2023::TracedCallback declaration and template implementation.
 */
namespace nsim2023
{

/**
 * A TracedCallback has almost exactly the same API as a normal
 * Callback but instead of forwarding calls to a single function
 * (as a Callback normally does), it forwards calls to a chain
 * of Callback.  Connect adds a Callback at the end of the chain
 * of callbacks.  Disconnect removes a Callback from the chain of callbacks.
 *
 * This is a functor: the chain of Callbacks is invoked by
 * calling the operator() form with the appropriate
 * number of arguments.
 */
template <typename... Ts>
class TracedCallback
{
  public:
    /** Constructor. */
    TracedCallback();
    /**
     * Append a Callback to the chain (without a context).
     *
     * \param [in] callback Callback to add to chain.
     */
    void ConnectWithoutContext(const CallbackBase& callback);
    /**
     * Append a Callback to the chain with a context.
     *
     * The context string will be provided as the first argument
     * to the Callback.
     *
     * \param [in] callback Callback to add to chain.
     * \param [in] path Context string to provide when invoking the Callback.
     */
    void Connect(const CallbackBase& callback, std::string path);
    /**
     * Remove from the chain a Callback which was connected without a context.
     *
     * \param [in] callback Callback to remove from the chain.
     */
    void DisconnectWithoutContext(const CallbackBase& callback);
    /**
     * Remove from the chain a Callback which was connected with a context.
     *
     * \param [in] callback Callback to remove from the chain.
     * \param [in] path Context path which was used to connect the Callback.
     */
    void Disconnect(const CallbackBase& callback, std::string path);
    /**
     * \brief Functor which invokes the chain of Callbacks.
     * \tparam Ts \deduced Types of the functor arguments.
     * \param [in] args The arguments to the functor
     */
    void operator()(Ts... args) const;
    /**
     * \brief Checks if the Callbacks list is empty.
     * \return true if the Callbacks list is empty.
     */
    bool IsEmpty() const;

    /**
     *  TracedCallback signature for POD.
     *
     * \param [in] value Value of the traced variable.
     * @{
     */
    // Uint32Callback appears to be the only one used at the moment.
    // Feel free to add typedef's for any other POD you need.
    typedef void (*Uint32Callback)(const uint32_t value);
    /**@}*/

  private:
    /**
     * Container type for holding the chain of Callbacks.
     */
    typedef std::list<Callback<void, Ts...>> CallbackList;

    /** The chain of Callbacks. */
    CallbackList m_callbackList;
};

}

/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/
namespace nsim2023
{

template <typename... Ts>
TracedCallback<Ts...>::TracedCallback()
    : m_callbackList()
{
}

template <typename... Ts>
void
TracedCallback<Ts...>::ConnectWithoutContext(const CallbackBase& callback)
{
    Callback<void, Ts...> cb;
    if (!cb.Assign(callback))
    {
        NS_FATAL_ERROR_NO_MSG();
    }
    m_callbackList.push_back(cb);
}

template <typename... Ts>
void
TracedCallback<Ts...>::Connect(const CallbackBase& callback, std::string path)
{
    Callback<void, std::string, Ts...> cb;
    if (!cb.Assign(callback))
    {
        NS_FATAL_ERROR("when connecting to " << path);
    }
    Callback<void, Ts...> realCb = cb.Bind(path);
    m_callbackList.push_back(realCb);
}

template <typename... Ts>
void
TracedCallback<Ts...>::DisconnectWithoutContext(const CallbackBase& callback)
{
    for (typename CallbackList::iterator i = m_callbackList.begin(); i != m_callbackList.end();
         /* empty */)
    {
        if ((*i).IsEqual(callback))
        {
            i = m_callbackList.erase(i);
        }
        else
        {
            i++;
        }
    }
}

template <typename... Ts>
void
TracedCallback<Ts...>::Disconnect(const CallbackBase& callback, std::string path)
{
    Callback<void, std::string, Ts...> cb;
    if (!cb.Assign(callback))
    {
        NS_FATAL_ERROR("when disconnecting from " << path);
    }
    Callback<void, Ts...> realCb = cb.Bind(path);
    DisconnectWithoutContext(realCb);
}

template <typename... Ts>
void
TracedCallback<Ts...>::operator()(Ts... args) const
{
    for (typename CallbackList::const_iterator i = m_callbackList.begin();
         i != m_callbackList.end();
         i++)
    {
        (*i)(args...);
    }
}

template <typename... Ts>
bool
TracedCallback<Ts...>::IsEmpty() const
{
    return m_callbackList.empty();
}

}

#endif /* TRACED_CALLBACK_H */

