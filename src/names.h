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

#ifndef OBJECT_NAMES_H
#define OBJECT_NAMES_H

#include "object.h"
#include "ptr.h"


namespace nsim2023
{


class Names
{
  public:
    /**
     * Add the association between the string "name" and the
     * Ptr<Object> obj.
     *
     * The name may begin either with "/Names" to explicitly call out
     * the fact that the name provided is installed under the root of
     * the name space, or it may begin with the name of the first object
     * in the path.  For example, Names::Add ("/Names/client", obj) and
     * Names::Add ("client", obj) accomplish exactly the same thing.  A
     * name at a given level in the name space path must be unique.  In
     * the case of the example above, it would be illegal to try and
     * associate a different object with the same name: "client" at the
     * same level ("/Names") in the path.
     *
     * As well as specifying a name at the root of the "/Names"
     * namespace, the name parameter can contain a path that fully
     * qualifies the name to be added.  For example, if you previously
     * have named an object "client" in the root namespace as above, you
     * could name an object "under" that name by making a call like
     * Names::Add ("/Names/client/eth0", obj).  This will define the
     * name "eth0" and make it reachable using the path specified.  Note
     * that Names::Add ("client/eth0", obj) would accomplish exactly the
     * same thing.
     *
     * Duplicate names are not allowed at the same level in a path,
     * however you may associate similar names with different paths.
     * For example, if you define "/Names/Client", you may not define
     * another "/Names/Client" just as you may not have two files with
     * the same name in a classical filesystem.  However, you may have
     * "/Names/Client/eth0" and "/Names/Server/eth0" defined at the same
     * time just as you might have different files of the same name
     * under different directories.
     */
    static void Add(std::string name, Ptr<Object> object);


    static void Add(std::string path, std::string name, Ptr<Object> object);


    static void Add(Ptr<Object> context, std::string name, Ptr<Object> object);

    /**
     * Rename a previously associated name.
     *
     * The name may begin either with "/Names" to explicitly call out
     * the fact that the name provided is installed under the root of
     * the name space, or it may begin with the name of the first object
     * in the path.  For example, Names::Rename ("/Names/client",
     * "server") and Names::Rename ("client", "server") accomplish
     * exactly the same thing.  Names at a given level in the name space
     * path must be unique. In the case of the example above, it would
     * be illegal to try and rename a different object to the same name:
     * "server" at the same level ("/Names") in the path.
     *
     * As well as specifying a name at the root of the "/Names"
     * namespace, the name parameter can contain a path that fully
     * qualifies the name to be changed.  For example, if you previously
     * have (re)named an object "server" in the root namespace as above,
     * you could then rename an object "under" that name by making a
     * call like Names::Rename ("/Names/server/csma", "eth0").  This
     * will rename the object previously associated with
     * "/Names/server/csma" to "eth0" and make leave it reachable using
     * the path "/Names/server/eth0".  Note that Names::Rename
     * ("server/csma", "eth0") would accomplish exactly the same thing.
     */
    static void Rename(std::string oldpath, std::string newname);


    static void Rename(std::string path, std::string oldname, std::string newname);


    static void Rename(Ptr<Object> context, std::string oldname, std::string newname);


    static std::string FindName(Ptr<Object> object);

    /**
     * Given a pointer to an object, look to see if that object
     * has a name associated with it and return the fully qualified name
     * path of the object otherwise return an empty string.
     *
     * An object can be referred to in two ways.  Either you can talk
     * about it using its fully qualified path name, for example,
     * "/Names/client/eth0" or you can refer to it by its name, in this
     * case "eth0".
     *
     * This method returns the name path of the object, e.g.,
     * "Names/client/eth0".
     */
    static std::string FindPath(Ptr<Object> object);


    static void Clear();


    template <typename T>
    static Ptr<T> Find(std::string path);


    template <typename T>
    static Ptr<T> Find(std::string path, std::string name);


    template <typename T>
    static Ptr<T> Find(Ptr<Object> context, std::string name);

  private:

    static Ptr<Object> FindInternal(std::string path);


    static Ptr<Object> FindInternal(std::string path, std::string name);


    static Ptr<Object> FindInternal(Ptr<Object> context, std::string name);
};

template <typename T>
Ptr<T>
Names::Find(std::string path)
{
    Ptr<Object> obj = FindInternal(path);
    if (obj)
    {
        return obj->GetObject<T>();
    }
    else
    {
        return nullptr;
    }
}

template <typename T>
Ptr<T>
Names::Find(std::string path, std::string name)
{
    Ptr<Object> obj = FindInternal(path, name);
    if (obj)
    {
        return obj->GetObject<T>();
    }
    else
    {
        return nullptr;
    }
}

template <typename T>
Ptr<T>
Names::Find(Ptr<Object> context, std::string name)
{
    Ptr<Object> obj = FindInternal(context, name);
    if (obj)
    {
        return obj->GetObject<T>();
    }
    else
    {
        return nullptr;
    }
}

}

#endif /* OBJECT_NAMES_H */

