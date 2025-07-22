#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <set>

using namespace cv;
using namespace std;

// Minimum color distance threshold (tweak as needed)
const double COLOR_DISTANCE_THRESHOLD = 30.0;

// Convert BGR color to hex string
string bgrToHex(const Vec3b &color) {
  char hexCol[8];
  snprintf(hexCol, sizeof(hexCol), "#%02X%02X%02X", color[2], color[1],
           color[0]); // BGR to RGB
  return string(hexCol);
}

// Euclidean distance in RGB
double colorDistance(const Vec3b &c1, const Vec3b &c2) {
  return sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) +
              pow(c1[2] - c2[2], 2));
}

// Brighten a color slightly
Vec3b brightenColor(const Vec3b &color, float factor = 1.3) {
  Vec3b brighter;
  for (int i = 0; i < 3; ++i)
    brighter[i] = min(int(color[i] * factor), 255);
  return brighter;
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
  resize(img, resized, Size(200, 200)); // Resize for performance

  // Prepare data for k-means
  Mat data;
  resized.convertTo(data, CV_32F);
  data = data.reshape(1, data.total());

  int k = 32; // Number of clusters (more than needed to allow filtering)
  Mat labels, centers;
  kmeans(data, k, labels,
         TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3,
         KMEANS_PP_CENTERS, centers);

  // Filter distinct colors
  vector<Vec3b> distinctColors;
  for (int i = 0; i < centers.rows; ++i) {
    Vec3b color;
    color[0] = (uchar)centers.at<float>(i, 0);
    color[1] = (uchar)centers.at<float>(i, 1);
    color[2] = (uchar)centers.at<float>(i, 2);

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
         << " distinct colors found.\n";
  }

  // Print CSS/terminal color variables
  cout << "* {\n";
  for (int i = 0; i < distinctColors.size(); ++i) {
    cout << "  --color" << i << ": " << bgrToHex(distinctColors[i]) << ";\n";
  }

  // Generate bright variants
  for (int i = 0; i < distinctColors.size(); ++i) {
    Vec3b bright = brightenColor(distinctColors[i]);
    cout << "  --color" << (i + 8) << ": " << bgrToHex(bright) << ";\n";
  }
  cout << "}\n";

  return 0;
}
