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

#include "double.h"
#include "nstime.h"
#include "simulator.h"
#include "nsim-string.h"
#include "log.h"

#include <iostream>

using namespace nsim2023;

/** Simple model object to illustrate event handling. */
class MyModel
{
  public:
    /* Start model execution by scheduling a HandleEvent. */
    void Start();

  private:
    void HandleEvent(double eventValue);
};

void
MyModel::Start()
{
    Simulator::Schedule(Seconds(10.0), &MyModel::HandleEvent, this, Simulator::Now().GetSeconds());
}

void
MyModel::HandleEvent(double value)
{
    std::cout << "Member method received event at " << Simulator::Now().GetSeconds()
              << "s started at " << value << "s" << std::endl;
}

void
ExampleFunction(MyModel* model)
{
    std::cout << "ExampleFunction received event at " << Simulator::Now().GetSeconds() << "s"
              << std::endl;
    model->Start();
}

/** Simple function event handler; the corresponding event is cancelled. */
void
CancelledEvent()
{
    std::cout << "I should never be called... " << std::endl;
}

int main(int argc, char* argv[])
{
    // LogComponentEnableAll(LOG_ALL);

    MyModel model;
    Simulator::Schedule(Seconds(10.0), &ExampleFunction, &model);

    EventId id = Simulator::Schedule(Seconds(30.0), &CancelledEvent);
    Simulator::Cancel(id);

    Simulator::Schedule(Seconds(25.0), []() {
        std::cout << "Code within a lambda expression at time " << Simulator::Now().As(Time::S)
                        << std::endl;
    });

    Simulator::Run();

    Simulator::Destroy();

    return 0;
}

