#include "color_selector.hpp"
#include "color_utils.hpp"
#include "kmeans_wrapper.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: ./heugen <image_path>\n";
    return 1;
  }

  Mat img = imread(argv[1]);
  if (img.empty()) {
    cerr << "Failed to load image: " << argv[1] << "\n";
    return 1;
  }

  resize(img, img, Size(200, 200));
  img.convertTo(img, CV_32F, 1.0 / 255.0);

  Mat lab;
  cvtColor(img, lab, COLOR_BGR2Lab);

  auto allColors = extractClusterColors(lab, 32);

  vector<Vec3f> filtered;
  for (const auto &c : allColors) {
    if (c[0] > 30.0f)
      filtered.push_back(c);
  }

  auto distinctColors = selectMostDistinctColors(filtered, 16);

  sort(distinctColors.begin(), distinctColors.end(),
       [](const Vec3f &a, const Vec3f &b) {
         return calculateSaturation(a) > calculateSaturation(b);
       });

  for (const auto &labColor : distinctColors) {
    Vec3b bgr = labToBgr(labColor);
    printf("#%02X%02X%02X\n", bgr[2], bgr[1], bgr[0]);
  }

  return 0;
}
