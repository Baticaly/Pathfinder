#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "uartProxy.h"

/*
Name:   uart2
Info:   Enable uart 2 on GPIOs 0-3
Load:   dtoverlay=uart2,<param>
Params: ctsrts                  Enable CTS/RTS on GPIOs 2-3 (default off)

Name:   uart4
Info:   Enable uart 4 on GPIOs 8-11
Load:   dtoverlay=uart4,<param>
Params: ctsrts                  Enable CTS/RTS on GPIOs 10-11 (default off)
*/

void uartProxy()
{
    // Open the serial port (ttyAMA1)
    int serialPort = open("/dev/ttyAMA1", O_RDWR | O_NOCTTY | O_NDELAY);
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

    // Placeholder data to send
    std::string data = "Hello Pico!";

    // Send data continuously
    while (true)
    {
        ssize_t bytesWritten = write(serialPort, data.c_str(), data.length());
        if (bytesWritten == -1)
        {
            std::cerr << "Failed to write data to serial port." << std::endl;
            break;
        }
        usleep(1000000); // Sleep for 1 second before sending the next batch
    }

    // Close the serial port
    close(serialPort);
}
