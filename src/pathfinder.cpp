#include <thread>
#include <iostream>
#include <mutex>
#include "tcpSocketListener.h"
#include "uartProxy.h"
#include "sensorInput.h"

// Global variable to store UART data
std::string uartData;
std::mutex uartMutex;

// Function for the main program thread
void pathfinder()
{
    while (true)
    {
        std::cout << "UART Data: " << uartData << std::endl;
    }
}

int main()
{
    // Create threads for both programs
    std::thread t1(tcpSocketListener);
    std::thread t2(uartProxy);
    std::thread t3(sensorInput);

    // Call the main program function
    pathfinder();

    // Wait for both threads to finish
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
