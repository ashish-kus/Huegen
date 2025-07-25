#include "color_utils.hpp"
#include <cmath>

using namespace cv;

Vec3b labToBgr(Vec3f labColor) {
  Mat lab(1, 1, CV_32FC3, Scalar(labColor[0], labColor[1], labColor[2]));
  Mat bgr;
  cvtColor(lab, bgr, COLOR_Lab2BGR);
  bgr.convertTo(bgr, CV_8UC3, 255.0);
  return bgr.at<Vec3b>(0, 0);
}

float calculateSaturation(const Vec3f &labColor) {
  return std::sqrt(labColor[1] * labColor[1] + labColor[2] * labColor[2]);
}

float colorDistance(const Vec3f &a, const Vec3f &b) {
  return std::sqrt((a[0] - b[0]) * (a[0] - b[0]) +
                   (a[1] - b[1]) * (a[1] - b[1]) +
                   (a[2] - b[2]) * (a[2] - b[2]));
}
