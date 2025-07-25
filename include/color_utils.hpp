#ifndef COLOR_UTILS_HPP
#define COLOR_UTILS_HPP

#include <opencv2/opencv.hpp>

cv::Vec3b labToBgr(cv::Vec3f labColor);
float calculateSaturation(const cv::Vec3f &labColor);
float colorDistance(const cv::Vec3f &a, const cv::Vec3f &b);

#endif
