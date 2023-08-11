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

#ifndef CONFIG_H
#define CONFIG_H

#include "ptr.h"

#include <string>
#include <vector>


namespace nsim2023
{

class AttributeValue;
class Object;
class CallbackBase;


namespace Config
{

void Reset();

void Set(std::string path, const AttributeValue& value);

bool SetFailSafe(std::string path, const AttributeValue& value);

void SetDefault(std::string name, const AttributeValue& value);

bool SetDefaultFailSafe(std::string name, const AttributeValue& value);

void SetGlobal(std::string name, const AttributeValue& value);

bool SetGlobalFailSafe(std::string name, const AttributeValue& value);

void ConnectWithoutContext(std::string path, const CallbackBase& cb);

bool ConnectWithoutContextFailSafe(std::string path, const CallbackBase& cb);

void DisconnectWithoutContext(std::string path, const CallbackBase& cb);

void Connect(std::string path, const CallbackBase& cb);

bool ConnectFailSafe(std::string path, const CallbackBase& cb);

void Disconnect(std::string path, const CallbackBase& cb);

/**
 * hold a set of objects which match a specific search string.
 *
 * This class also allows you to perform a set of configuration operations
 * on the set of matching objects stored in the container. Specifically,
 * it is possible to perform bulk Connects and Sets.
 */
class MatchContainer
{
  public:
    /** Const iterator over the objects in this container. */
    typedef std::vector<Ptr<Object>>::const_iterator Iterator;
    MatchContainer();

    MatchContainer(const std::vector<Ptr<Object>>& objects,
                   const std::vector<std::string>& contexts,
                   std::string path);

    MatchContainer::Iterator Begin() const;

    MatchContainer::Iterator begin() const
    {
        return Begin();
    }

    MatchContainer::Iterator End() const;

    MatchContainer::Iterator end() const
    {
        return End();
    }

    std::size_t GetN() const;

    std::size_t size() const
    {
        return GetN();
    }

    Ptr<Object> Get(std::size_t i) const;

    std::string GetMatchedPath(uint32_t i) const;

    std::string GetPath() const;

    /**
     * Set the specified attribute value to all the objects stored in this
     * container.  This method will raise a fatal error if no such attribute
     * exists; use SetFailSafe if the absence of the attribute is to be
     * permitted.
     */
    void Set(std::string name, const AttributeValue& value);

    bool SetFailSafe(std::string name, const AttributeValue& value);

    void Connect(std::string name, const CallbackBase& cb);

    bool ConnectFailSafe(std::string name, const CallbackBase& cb);

    void ConnectWithoutContext(std::string name, const CallbackBase& cb);

    bool ConnectWithoutContextFailSafe(std::string name, const CallbackBase& cb);

    void Disconnect(std::string name, const CallbackBase& cb);

    void DisconnectWithoutContext(std::string name, const CallbackBase& cb);

  private:
    /** The list of objects in this container. */
    std::vector<Ptr<Object>> m_objects;
    /** The context for each object. */
    std::vector<std::string> m_contexts;
    /** The path used to perform the object matching. */
    std::string m_path;
};


MatchContainer LookupMatches(std::string path);

/**
 * Each root object is used during path matching as the root of the path by 
 * Config::Connect, and Config::Set.
 */
void RegisterRootNamespaceObject(Ptr<Object> obj);

/**
 * This function undoes the work of Config::RegisterRootNamespaceObject.
 */
void UnregisterRootNamespaceObject(Ptr<Object> obj);

std::size_t GetRootNamespaceObjectN();

Ptr<Object> GetRootNamespaceObject(uint32_t i);

} // namespace Config

} 

#endif /* CONFIG_H */

