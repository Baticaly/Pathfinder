#include <iostream> // Added for debugging
#include <vector>
#include <mutex>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include "uartProxy.h"

void uartProxy(const std::vector<int> &motorData, std::mutex &motorMutex)
{
    // Open the serial port (ttyAMA1)
    int serialPort = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY);
    if (serialPort == -1)
    {
        std::cerr << "Failed to open serial port." << std::endl;
        return;
    }

    // Configure the serial port
    struct termios options;
    tcgetattr(serialPort, &options);
    cfsetispeed(&options, B9600); // Set baud rate to 9600 (adjust as needed)
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8; // 8 data bits
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tcsetattr(serialPort, TCSANOW, &options);

    while (true)
    {
        std::string jsonMsg;

        motorMutex.lock(); // Lock the mutex before accessing motorData

        // Prepare JSON message with motor values
        jsonMsg = "{";
        for (size_t i = 0; i < motorData.size(); ++i)
        {
            jsonMsg += "\"motor" + std::to_string(i + 1) + "\":" + std::to_string(motorData[i]);
            if (i != motorData.size() - 1)
                jsonMsg += ",";
        }
        jsonMsg += "}";

        motorMutex.unlock(); // Unlock the mutex after accessing motorData

        // Print motorData for debugging
        std::cout << "Motor Data: " << jsonMsg << std::endl;

        // Send JSON message over serial
        ssize_t bytesWritten = write(serialPort, jsonMsg.c_str(), jsonMsg.length());
        if (bytesWritten == -1)
        {
            std::cerr << "Failed to write data to serial port." << std::endl;
        }
    }

    // Close the serial port
    close(serialPort);
}
