#include "kmeans_wrapper.hpp"
#include <map>

using namespace std;
using namespace cv;

vector<Vec3f> extractClusterColors(const Mat &lab, int k) {
  Mat samples = lab.reshape(1, lab.rows * lab.cols);
  Mat labels, centers;

  kmeans(samples, k, labels,
         TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3,
         KMEANS_PP_CENTERS, centers);

  vector<Vec3f> allColors;

  auto vec3fCompare = [](const Vec3f &a, const Vec3f &b) {
    if (a[0] != b[0])
      return a[0] < b[0];
    if (a[1] != b[1])
      return a[1] < b[1];
    return a[2] < b[2];
  };

  for (int cluster = 0; cluster < k; ++cluster) {
    map<Vec3f, int, decltype(vec3fCompare)> colorCount(vec3fCompare);

    for (int i = 0; i < labels.rows; ++i) {
      if (labels.at<int>(i, 0) == cluster) {
        int row = i / lab.cols;
        int col = i % lab.cols;
        Vec3f labColor = lab.at<Vec3f>(row, col);

        labColor[0] = round(labColor[0]);
        labColor[1] = round(labColor[1] * 2) / 2.0f;
        labColor[2] = round(labColor[2] * 2) / 2.0f;

        colorCount[labColor]++;
      }
    }

    Vec3f mostFrequent;
    int maxCount = 0;
    for (const auto &pair : colorCount) {
      if (pair.second > maxCount) {
        maxCount = pair.second;
        mostFrequent = pair.first;
      }
    }

    if (maxCount > 0) {
      allColors.push_back(mostFrequent);
    }
  }

  return allColors;
}
