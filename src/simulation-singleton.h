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

#ifndef SIMULATION_SINGLETON_H
#define SIMULATION_SINGLETON_H


namespace nsim2023
{

/**
 * This singleton class template ensures that the type
 * for which we want a singleton has a lifetime bounded
 * by the simulation run lifetime. That it, the underlying
 * type will be automatically deleted upon a call
 * to Simulator::Destroy.
 *
 * For a singleton with a lifetime bounded by the process,
 * not the simulation run, see Singleton.
 */
template <typename T>
class SimulationSingleton
{
  public:
    // Delete default constructor, copy constructor and assignment operator to avoid misuse
    SimulationSingleton<T>() = delete;
    SimulationSingleton<T>(const SimulationSingleton<T>&) = delete;
    SimulationSingleton<T>& operator=(const SimulationSingleton<T>&) = delete;

    /**
     * Get a pointer to the singleton instance.
     *
     * This instance will be automatically deleted when the
     * simulation is destroyed by a call to Simulator::Destroy.
     */
    static T* Get();

  private:
    /**
     * Get the singleton object, creating a new one if it doesn't exist yet.
     */
    static T** GetObject();

    /** Delete the static instance. */
    static void DeleteObject();
};

}

/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/

#include "simulator.h"

namespace nsim2023
{

template <typename T>
T*
SimulationSingleton<T>::Get()
{
    T** ppobject = GetObject();
    return *ppobject;
}

template <typename T>
T**
SimulationSingleton<T>::GetObject()
{
    static T* pobject = nullptr;
    if (pobject == nullptr)
    {
        pobject = new T();
        Simulator::ScheduleDestroy(&SimulationSingleton<T>::DeleteObject);
    }
    return &pobject;
}

template <typename T>
void
SimulationSingleton<T>::DeleteObject()
{
    T** ppobject = GetObject();
    delete (*ppobject);
    *ppobject = nullptr;
}

}

#endif /* SIMULATION_SINGLETON_H */

