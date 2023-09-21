#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include "tcpSocketListener.h"
#include "i2cProxy.h"
#include "sensorInput.h"
#include "imx219.h"
#include "liveVideo.h"

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
    // Camera test
    try {
        cv::Mat testImage = captureImage();

        // Get the current time
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);

        // Generate a filename with the current time
        std::ostringstream filename;
        filename << "/media/test_" << now_c << ".jpg";

        cv::imwrite(filename.str(), testImage);
        std::cout << "Camera test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Camera test failed: " << e.what() << std::endl;
        return 1;
    }

    // Start the UART proxy in a separate thread
    thread proxyThread(i2cProxy, ref(motorData), ref(motorMutex));

    // Start the TCP socket listener in a separate thread
    thread tcpThread(tcpSocketListener, ref(motorData), ref(motorMutex));

    // Start the image transmission in a separate thread
    // Replace "serverIP" and serverPort with your server's IP address and port number
    thread imageThread(transmitImage, 8081);

    // Wait for the threads to finish (optional)
    proxyThread.join();
    tcpThread.join();
    imageThread.join();

    return 0;
}
