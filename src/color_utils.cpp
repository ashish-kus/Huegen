#include "color_utils.hpp"
#include <cmath>
#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
using namespace std;

Vec3b bgrToRgb(const Vec3b &bgrColor) {
  return Vec3b(bgrColor[2], bgrColor[1], bgrColor[0]);
}

Vec3f rgbToHsl(const Vec3b &rgbColor) {
  float r = rgbColor[0] / 255.0f;
  float g = rgbColor[1] / 255.0f;
  float b = rgbColor[2] / 255.0f;

  float max_val = max({r, g, b});
  float min_val = min({r, g, b});
  float delta = max_val - min_val;

  float h = 0, s = 0, l = (max_val + min_val) / 2.0f;

  if (delta != 0) {
    s = (l > 0.5f) ? delta / (2.0f - max_val - min_val)
                   : delta / (max_val + min_val);

    if (max_val == r) {
      h = ((g - b) / delta) + (g < b ? 6 : 0);
    } else if (max_val == g) {
      h = (b - r) / delta + 2;
    } else {
      h = (r - g) / delta + 4;
    }
    h /= 6.0f;
  }

  return Vec3f(h * 360.0f, s * 100.0f, l * 100.0f);
}

Vec3f rgbToHsv(const Vec3b &rgbColor) {
  float r = rgbColor[0] / 255.0f;
  float g = rgbColor[1] / 255.0f;
  float b = rgbColor[2] / 255.0f;

  float max_val = max({r, g, b});
  float min_val = min({r, g, b});
  float delta = max_val - min_val;

  float h = 0, s = 0, v = max_val;

  if (max_val != 0) {
    s = delta / max_val;
  }

  if (delta != 0) {
    if (max_val == r) {
      h = ((g - b) / delta) + (g < b ? 6 : 0);
    } else if (max_val == g) {
      h = (b - r) / delta + 2;
    } else {
      h = (r - g) / delta + 4;
    }
    h /= 6.0f;
  }

  return Vec3f(h * 360.0f, s * 100.0f, v * 100.0f);
}

string formatHex(const Vec3b &rgbColor) {
  stringstream ss;
  ss << "#" << hex << uppercase << setfill('0') << setw(2) << (int)rgbColor[0]
     << setw(2) << (int)rgbColor[1] << setw(2) << (int)rgbColor[2];
  return ss.str();
}

string strip(const Vec3b &rgbColor) {
  stringstream ss;
  ss << hex << uppercase << setfill('0') << setw(2) << (int)rgbColor[0]
     << setw(2) << (int)rgbColor[1] << setw(2) << (int)rgbColor[2];
  return ss.str();
}

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
