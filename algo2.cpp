#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

// Minimum thresholds
const double COLOR_DISTANCE_THRESHOLD = 30.0;
const int MIN_SATURATION = 30; // to avoid gray
const int MIN_BRIGHTNESS = 50; // to avoid dark

// Convert BGR to HEX string
string bgrToHex(const Vec3b &color) {
  char hexCol[8];
  snprintf(hexCol, sizeof(hexCol), "#%02X%02X%02X", color[2], color[1],
           color[0]);
  return string(hexCol);
}

// Euclidean color distance
double colorDistance(const Vec3b &c1, const Vec3b &c2) {
  return sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) +
              pow(c1[2] - c2[2], 2));
}

// Score vibrancy using HSV (Saturation)
float computeVibrancy(const Vec3b &bgr) {
  Mat3b bgrMat(bgr);
  Mat3b hsv;
  cvtColor(bgrMat, hsv, COLOR_BGR2HSV);
  int s = hsv(0, 0)[1];
  int v = hsv(0, 0)[2];
  return (s >= MIN_SATURATION && v >= MIN_BRIGHTNESS) ? s : 0;
}

// Darken a color
Vec3b darkenColor(const Vec3b &color, float factor = 0.7) {
  Vec3b darker;
  for (int i = 0; i < 3; ++i)
    darker[i] = max(int(color[i] * factor), 0);
  return darker;
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

  // Resize image for faster processing
  Mat resized;
  resize(img, resized, Size(200, 200));

  // Collect vibrant pixels
  vector<pair<float, Vec3b>> vibrantColors;
  for (int y = 0; y < resized.rows; ++y) {
    for (int x = 0; x < resized.cols; ++x) {
      Vec3b color = resized.at<Vec3b>(y, x);
      float vibrancy = computeVibrancy(color);
      if (vibrancy > 0) {
        vibrantColors.emplace_back(vibrancy, color);
      }
    }
  }

  // Sort by vibrancy (descending)
  sort(vibrantColors.begin(), vibrantColors.end(),
       [](const auto &a, const auto &b) { return a.first > b.first; });

  // Extract 8 distinct vibrant colors
  vector<Vec3b> distinctColors;
  for (const auto &[vib, color] : vibrantColors) {
    bool isDistinct = true;
    for (const Vec3b &dc : distinctColors) {
      if (colorDistance(dc, color) < COLOR_DISTANCE_THRESHOLD) {
        isDistinct = false;
        break;
      }
    }
    if (isDistinct) {
      distinctColors.push_back(color);
    }
    if (distinctColors.size() == 8)
      break;
  }

  if (distinctColors.size() < 8) {
    cerr << "Warning: Only " << distinctColors.size()
         << " vibrant colors found.\n";
  }

  // Print CSS-style output
  cout << "* {\n";

  // Reversed base vibrant colors
  for (int i = 0; i < distinctColors.size(); ++i) {
    int rev = distinctColors.size() - 1 - i;
    cout << "  --color" << i << ": " << bgrToHex(distinctColors[rev]) << ";\n";
  }

  // Darker versions
  for (int i = 0; i < distinctColors.size(); ++i) {
    int rev = distinctColors.size() - 1 - i;
    Vec3b dark = darkenColor(distinctColors[rev]);
    cout << "  --color" << (i + 8) << ": " << bgrToHex(dark) << ";\n";
  }

  cout << "}\n";

  return 0;
}
