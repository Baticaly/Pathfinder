#include "imx219.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>

cv::Mat captureImage()
{
    cv::Mat frame;
    try {
        cv::VideoCapture cap(0); // Open the default camera

        if(!cap.isOpened()) { // Check if we succeeded
            std::cerr << "Error opening camera" << std::endl;
            throw std::runtime_error("Failed to open camera");
        }

        cap >> frame; // Get a new frame from camera

        if (frame.empty())
        {
            std::cerr << "ERROR capturing image" << std::endl;
            throw std::runtime_error("Failed to capture image");
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
        // Handle the error or recover here
    }

    return frame;
}

std::vector<unsigned char> convertToJpeg(const cv::Mat& image)
{
    // Convert the image to JPEG format
    std::vector<unsigned char> jpegData;
    cv::imencode(".jpg", image, jpegData);

    return jpegData;
}