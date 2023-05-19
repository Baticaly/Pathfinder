#include <iostream>
#include <thread>
#include "tcpSocketListener.h"
#include "uartProxy.h"

int main()
{
    // Create threads for both programs
    std::thread t1(tcpSocketListener);
    std::thread t2(uartProxy);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    return 0;
}
