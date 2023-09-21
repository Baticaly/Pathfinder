#include "imx219.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>

cv::Mat captureImage()
{
    cv::VideoCapture cap(0); // Open the default camera

    if(!cap.isOpened()) { // Check if we succeeded
        std::cerr << "Error opening camera" << std::endl;
        throw std::runtime_error("Failed to open camera");
    }

    cv::Mat frame;
    cap >> frame; // Get a new frame from camera

    if (frame.empty())
    {
        std::cerr << "ERROR capturing image" << std::endl;
        throw std::runtime_error("Failed to capture image");
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