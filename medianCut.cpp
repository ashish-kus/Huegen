#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

struct ColorBox {
  vector<Vec3b> colors;

  // Longest dimension (R, G, B)
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
      return 2; // R
    else if (gRange >= rRange && gRange >= bRange)
      return 1; // G
    else
      return 0; // B
  }

  // Average color of the box
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

// Convert BGR to hex
string bgrToHex(const Vec3b &color) {
  char hexCol[8];
  snprintf(hexCol, sizeof(hexCol), "#%02X%02X%02X", color[2], color[1],
           color[0]);
  return string(hexCol);
}

// Median Cut algorithm
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
    cerr << "Usage: ./extractor <image_path>\n";
    return 1;
  }

  string imagePath = argv[1];
  Mat img = imread(imagePath, IMREAD_COLOR);
  if (img.empty()) {
    cerr << "Error: Could not open or find the image.\n";
    return 1;
  }

  Mat resized;
  resize(img, resized, Size(200, 200)); // Resize for speed

  vector<Vec3b> pixels;
  for (int y = 0; y < resized.rows; ++y) {
    for (int x = 0; x < resized.cols; ++x) {
      pixels.push_back(resized.at<Vec3b>(y, x));
    }
  }

  vector<Vec3b> palette;
  int numColors = 64;             // You want 16 colors
  int maxDepth = log2(numColors); // log2(16) = 4

  medianCut(pixels, 0, maxDepth, palette);

  cout << "* {\n";
  for (int i = 0; i < palette.size(); ++i) {
    cout << "  --color" << i << ": " << bgrToHex(palette[i]) << ";\n";
  }
  cout << "}\n";

  return 0;
}
