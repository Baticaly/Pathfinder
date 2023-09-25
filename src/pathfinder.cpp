#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include "tcpSocketListener.h"
#include "i2cProxy.h"
#include "sensorInput.h"
using namespace std;

// Global variable to store UART data
string uartData;
mutex uartMutex;

// Shared variable to store motor values
vector<int> motorData;
mutex motorMutex; // Mutex for protecting motorData

// Global variable to store the received JSON data
std::string pathUpdate;
std::mutex pathUpdateMutex;

int main()
{
    // Start the UART proxy in a separate thread
    thread proxyThread(i2cProxy, ref(motorData), ref(motorMutex));

    // Start the TCP socket listener in a separate thread
    thread tcpThread(tcpSocketListener, ref(motorData), ref(motorMutex));

    // Wait for the threads to finish (optional)
    proxyThread.join();
    tcpThread.join();

    return 0;
}
