#include <fstream>
#include <iostream>
#include <string>
#include <mutex>

// Global variable for UART data (extern declaration)
extern std::string uartData;
extern std::mutex uartMutex;

// Function for the UART listener thread
void sensorInput()
{
    std::ifstream serial("/dev/ttyAMA2");

    if (!serial)
    {
        std::cerr << "Failed to open serial port." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(serial, line))
    {
        // Store UART data in the global variable
        std::lock_guard<std::mutex> lock(uartMutex);
        uartData = line;
        std::cout << "UART Data: " << uartData << std::endl;
    }
}
