#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

struct ColorBox {
  vector<Vec3b> colors;

  int getLongestColorDimension() {
    int rMin = 255, rMax = 0, gMin = 255, gMax = 0, bMin = 255, bMax = 0;
    for (const auto &c : colors) {
      rMin = min(rMin, (int)c[2]);
      rMax = max(rMax, (int)c[2]);
      gMin = min(gMin, (int)c[1]);
      gMax = max(gMax, (int)c[1]);
      bMin = min(bMin, (int)c[0]);
      bMax = max(bMax, (int)c[0]);
    }

    int rRange = rMax - rMin;
    int gRange = gMax - gMin;
    int bRange = bMax - bMin;

    if (rRange >= gRange && rRange >= bRange)
      return 2;
    else if (gRange >= rRange && gRange >= bRange)
      return 1;
    else
      return 0;
  }

  Vec3b getAverageColor() {
    Vec3i sum(0, 0, 0);
    for (const auto &c : colors) {
      sum[0] += c[0];
      sum[1] += c[1];
      sum[2] += c[2];
    }
    int count = (int)colors.size();
    return Vec3b(sum[0] / count, sum[1] / count, sum[2] / count);
  }
};

string bgrToHex(const Vec3b &color) {
  char hexCol[8];
  snprintf(hexCol, sizeof(hexCol), "#%02X%02X%02X", color[2], color[1],
           color[0]);
  return string(hexCol);
}

void medianCut(vector<Vec3b> &pixels, int depth, int maxDepth,
               vector<Vec3b> &palette) {
  if (pixels.empty())
    return;

  ColorBox box;
  box.colors = pixels;

  if (depth == maxDepth) {
    palette.push_back(box.getAverageColor());
    return;
  }

  int dim = box.getLongestColorDimension();

  sort(pixels.begin(), pixels.end(),
       [dim](const Vec3b &a, const Vec3b &b) { return a[dim] < b[dim]; });

  size_t medianIndex = pixels.size() / 2;

  vector<Vec3b> left(pixels.begin(), pixels.begin() + medianIndex);
  vector<Vec3b> right(pixels.begin() + medianIndex, pixels.end());

  medianCut(left, depth + 1, maxDepth, palette);
  medianCut(right, depth + 1, maxDepth, palette);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: ./extract_vibrant <image_path>\n";
    return 1;
  }

  string imagePath = argv[1];
  Mat img = imread(imagePath, IMREAD_COLOR);
  if (img.empty()) {
    cerr << "Error: Could not open or find the image.\n";
    return 1;
  }

  Mat resized;
  resize(img, resized, Size(200, 200));

  vector<Vec3b> pixels;
  for (int y = 0; y < resized.rows; ++y) {
    for (int x = 0; x < resized.cols; ++x) {
      pixels.push_back(resized.at<Vec3b>(y, x));
    }
  }

  vector<Vec3b> rawPalette;
  int numColors = 64; // Start with more to allow filtering
  int maxDepth = log2(numColors);
  medianCut(pixels, 0, maxDepth, rawPalette);

  // Filter: keep only vibrant and bright colors (HSV)
  vector<Vec3b> filteredPalette;
  for (const auto &color : rawPalette) {
    Mat bgr(1, 1, CV_8UC3, color);
    Mat hsv;
    cvtColor(bgr, hsv, COLOR_BGR2HSV);
    Vec3b hsvVal = hsv.at<Vec3b>(0, 0);
    int s = hsvVal[1];
    int v = hsvVal[2];
    if (s > 100 && v > 120) {
      filteredPalette.push_back(color);
    }
  }

  // Sort by vibrance (Saturation + Brightness)
  sort(filteredPalette.begin(), filteredPalette.end(),
       [](const Vec3b &a, const Vec3b &b) {
         Mat bgrA(1, 1, CV_8UC3, a), bgrB(1, 1, CV_8UC3, b);
         Mat hsvA, hsvB;
         cvtColor(bgrA, hsvA, COLOR_BGR2HSV);
         cvtColor(bgrB, hsvB, COLOR_BGR2HSV);
         Vec3b va = hsvA.at<Vec3b>(0, 0), vb = hsvB.at<Vec3b>(0, 0);
         return (va[1] + va[2]) > (vb[1] + vb[2]);
       });

  // Limit to top 16
  if (filteredPalette.size() > 16) {
    filteredPalette.resize(16);
  }

  // Output
  cout << "* {\n";
  for (int i = 0; i < filteredPalette.size(); ++i) {
    cout << "  --color" << i << ": " << bgrToHex(filteredPalette[i]) << ";\n";
  }
  cout << "}\n";

  return 0;
}
