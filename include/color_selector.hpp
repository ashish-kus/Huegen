#ifndef COLOR_SELECTOR_HPP
#define COLOR_SELECTOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Vec3f>
selectMostDistinctColors(const std::vector<cv::Vec3f> &colors, int n);

#endif
