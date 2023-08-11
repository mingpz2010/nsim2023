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

#include "rng-seed-manager.h"

#include "attribute-helper.h"
#include "config.h"
#include "global-value.h"
#include "log.h"
#include "uinteger.h"


namespace nsim2023
{

NS_LOG_COMPONENT_DEFINE("RngSeedManager");

/**
 * The next random number generator stream number to use
 * for automatic assignment.
 */
static uint64_t g_nextStreamIndex = 0;

static nsim2023::GlobalValue g_rngSeed("RngSeed",
                                  "The global seed of all rng streams",
                                  nsim2023::UintegerValue(1),
                                  nsim2023::MakeUintegerChecker<uint32_t>());

/**
 * The random number generator substream index.  This is used to generate
 * new PRNG sequences for all streams (random variables) in such a manner
 * that the streams remain uncorrelated.  Incrementing this variable can
 * be used for independent replications.
 */
static nsim2023::GlobalValue g_rngRun("RngRun",
                                 "The substream index used for all streams",
                                 nsim2023::UintegerValue(1),
                                 nsim2023::MakeUintegerChecker<uint64_t>());

uint32_t RngSeedManager::GetSeed()
{
    NS_LOG_FUNCTION_NOARGS();
    UintegerValue seedValue;
    g_rngSeed.GetValue(seedValue);
    return static_cast<uint32_t>(seedValue.Get());
}

void RngSeedManager::SetSeed(uint32_t seed)
{
    NS_LOG_FUNCTION(seed);
    Config::SetGlobal("RngSeed", UintegerValue(seed));
}

void
RngSeedManager::SetRun(uint64_t run)
{
    NS_LOG_FUNCTION(run);
    Config::SetGlobal("RngRun", UintegerValue(run));
}

uint64_t
RngSeedManager::GetRun()
{
    NS_LOG_FUNCTION_NOARGS();
    UintegerValue value;
    g_rngRun.GetValue(value);
    uint64_t run = value.Get();
    return run;
}

uint64_t
RngSeedManager::GetNextStreamIndex()
{
    NS_LOG_FUNCTION_NOARGS();
    uint64_t next = g_nextStreamIndex;
    g_nextStreamIndex++;
    return next;
}

}

