#ifndef IMX219_H
#define IMX219_H

#include <opencv2/core/core.hpp>
#include <vector>

cv::Mat captureImage();
std::vector<unsigned char> convertToJpeg(const cv::Mat& image);

#endif // IMX219_H