#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

const double SATURATION_THRESHOLD = 0.5;
const double LIGHTNESS_MIN = 0.3;
const double LIGHTNESS_MAX = 0.8;
const int TOP_COLORS = 8;

string bgrToHex(const Vec3b &color) {
  char hexCol[8];
  snprintf(hexCol, sizeof(hexCol), "#%02X%02X%02X", color[2], color[1],
           color[0]);
  return string(hexCol);
}

Vec3f bgrToHsl(const Vec3b &color) {
  float b = color[0] / 255.0f;
  float g = color[1] / 255.0f;
  float r = color[2] / 255.0f;

  float maxC = max({r, g, b});
  float minC = min({r, g, b});
  float delta = maxC - minC;

  float h = 0.0f;
  if (delta != 0) {
    if (maxC == r)
      h = fmod(((g - b) / delta), 6.0f);
    else if (maxC == g)
      h = ((b - r) / delta) + 2.0f;
    else
      h = ((r - g) / delta) + 4.0f;
    h *= 60.0f;
    if (h < 0)
      h += 360.0f;
  }

  float l = (maxC + minC) / 2.0f;
  float s = (delta == 0) ? 0 : (delta / (1.0f - abs(2.0f * l - 1.0f)));

  return Vec3f(h, s, l); // HSL
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
  resize(img, resized, Size(200, 200));

  Mat data;
  resized.convertTo(data, CV_32F);
  data = data.reshape(1, data.total());

  int k = 32;
  Mat labels, centers;
  kmeans(data, k, labels,
         TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3,
         KMEANS_PP_CENTERS, centers);

  vector<int> counts(k, 0);
  for (int i = 0; i < labels.rows; ++i)
    counts[labels.at<int>(i)]++;

  vector<pair<int, Vec3b>> vibrantColors;

  for (int i = 0; i < centers.rows; ++i) {
    Vec3b color;
    color[0] = (uchar)centers.at<float>(i, 0);
    color[1] = (uchar)centers.at<float>(i, 1);
    color[2] = (uchar)centers.at<float>(i, 2);

    Vec3f hsl = bgrToHsl(color);
    float saturation = hsl[1];
    float lightness = hsl[2];

    if (saturation > SATURATION_THRESHOLD && lightness > LIGHTNESS_MIN &&
        lightness < LIGHTNESS_MAX) {
      vibrantColors.push_back({counts[i], color});
    }
  }

  // Sort by dominance (pixel count)
  sort(vibrantColors.begin(), vibrantColors.end(),
       [](const pair<int, Vec3b> &a, const pair<int, Vec3b> &b) {
         return a.first > b.first;
       });

  cout << "* {\n";
  for (int i = 0; i < min((int)vibrantColors.size(), TOP_COLORS); ++i) {
    cout << "  --color" << i << ": " << bgrToHex(vibrantColors[i].second)
         << ";\n";
  }
  cout << "}\n";

  return 0;
}
