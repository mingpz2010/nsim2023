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

#ifndef RNG_SEED_MANAGER_H
#define RNG_SEED_MANAGER_H

#include <stdint.h>


namespace nsim2023
{

/**
 * Manage the seed number and run number of the underlying
 * random number generator, and automatic assignment of stream numbers.
 */
class RngSeedManager
{
  public:
    /**
     * This sets the global initial seed which will be used all
     * subsequently instantiated RandomVariableStream objects.
     */
    static void SetSeed(uint32_t seed);

    static uint32_t GetSeed();

    /**
     * Set the run number of simulation.
     */
    static void SetRun(uint64_t run);

    /**
     * Get the current run number.
     */
    static uint64_t GetRun();

    /**
     * Get the next automatically assigned stream index.
     */
    static uint64_t GetNextStreamIndex();
};

/** Alias for compatibility. */
typedef RngSeedManager SeedManager;

}

#endif /* RNG_SEED_MANAGER_H */

