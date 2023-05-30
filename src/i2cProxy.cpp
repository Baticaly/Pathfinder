#include <iostream> // Added for debugging
#include <vector>
#include <mutex>
#include <fcntl.h>
#include <sys/ioctl.h> // Added for ioctl
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <chrono>
#include "i2cProxy.h"

void i2cProxy(const std::vector<int> &motorData, std::mutex &motorMutex)
{
    // Open the I2C bus (I2C-1)
    int i2cBus = open("/dev/i2c-1", O_RDWR);
    if (i2cBus == -1)
    {
        std::cerr << "Failed to open I2C bus." << std::endl;
        return;
    }

    // Set the I2C server address (Pico address)
    int i2cServerAddress = 0x10;
    if (ioctl(i2cBus, I2C_SLAVE, i2cServerAddress) < 0)
    {
        std::cerr << "Failed to acquire bus access and/or talk to I2C server." << std::endl;
        return;
    }

    while (true)
    {
        std::vector<uint8_t> data;

        motorMutex.lock(); // Lock the mutex before accessing motorData

        // Prepare data with motor values
        for (int value : motorData)
        {
            data.push_back(static_cast<uint8_t>(value));
        }

        motorMutex.unlock(); // Unlock the mutex after accessing motorData

        // Print motorData for debugging
        std::cout << "Motor Data: ";
        for (uint8_t value : data)
        {
            std::cout << static_cast<int>(value) << " ";
        }
        std::cout << std::endl;

        // Send motor data to the I2C server (Pico)
        ssize_t bytesWritten = write(i2cBus, data.data(), data.size());
        if (bytesWritten == -1)
        {
            std::cerr << "Failed to write data to I2C bus." << std::endl;
        }

        // Receive response from the I2C server (Pico) (optional)
        uint8_t response[256];
        ssize_t bytesRead = read(i2cBus, response, sizeof(response));
        if (bytesRead > 0)
        {
            std::cout << "Received response: ";
            for (ssize_t i = 0; i < bytesRead; ++i)
            {
                std::cout << static_cast<int>(response[i]) << " ";
            }
            std::cout << std::endl;
        }
    }

    // Close the I2C bus
    close(i2cBus);
}
