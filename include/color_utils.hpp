#ifndef COLOR_UTILS_HPP
#define COLOR_UTILS_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

cv::Vec3b labToBgr(cv::Vec3f labColor);
cv::Vec3b bgrToRgb(const cv::Vec3b &bgrColor);
cv::Vec3f rgbToHsl(const cv::Vec3b &rgbColor);
cv::Vec3f rgbToHsv(const cv::Vec3b &rgbColor);
std::string formatHex(const cv::Vec3b &rgbColor);
float calculateSaturation(const cv::Vec3f &labColor);
float colorDistance(const cv::Vec3f &a, const cv::Vec3f &b);

#endif
