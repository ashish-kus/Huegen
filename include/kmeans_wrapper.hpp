#ifndef KMEANS_WRAPPER_HPP
#define KMEANS_WRAPPER_HPP

#include <opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Vec3f> extractClusterColors(const cv::Mat &lab, int k = 32);

#endif
