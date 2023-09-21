#include "i2cProxy.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

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
        close(i2cBus); // Close the I2C bus before returning
        return;
    }

    // Define default motor values
    std::vector<uint8_t> defaultMotorData(8, 0); // 8 motor values, all set to 0

    while (true)
    {
        std::vector<uint8_t> data;

        {
            std::lock_guard<std::mutex> lock(motorMutex); // Use lock_guard for automatic unlocking

            // Prepare data with motor values
            if (motorData.empty()) {
                data = defaultMotorData;
            } else {
                for (int value : motorData) {
                    data.push_back(static_cast<uint8_t>(value));
                }
            }
        }

        // Send motor data to the I2C server (Pico)
        ssize_t bytesWritten = write(i2cBus, data.data(), data.size());
        if (bytesWritten == -1)
        {
            std::cerr << "Failed to write data to I2C bus." << std::endl;
        }

        // Receive response from the I2C server (Pico) (optional)
        // uint8_t response[256];
        // ssize_t bytesRead = read(i2cBus, response, sizeof(response));
        // if (bytesRead > 0)
        // {
        //     std::cout << "Received response: ";
        //     for (ssize_t i = 0; i < bytesRead; ++i)
        //     {
        //         std::cout << static_cast<int>(response[i]) << " ";
        //     }
        //     std::cout << std::endl;
        // }
    }

    // Close the I2C bus
    close(i2cBus);
}