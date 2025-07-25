#include "color_selector.hpp"
#include "color_utils.hpp"
#include <cfloat>

using namespace std;
using namespace cv;

vector<Vec3f> selectMostDistinctColors(const vector<Vec3f> &colors, int n) {
  if (colors.empty())
    return {};

  vector<Vec3f> selected;
  vector<Vec3f> remaining = colors;

  auto maxSatIt = max_element(
      remaining.begin(), remaining.end(), [](const Vec3f &a, const Vec3f &b) {
        return calculateSaturation(a) < calculateSaturation(b);
      });

  selected.push_back(*maxSatIt);
  remaining.erase(maxSatIt);

  while (selected.size() < n && !remaining.empty()) {
    int bestIndex = 0;
    float maxMinDistance = 0.0f;

    for (int i = 0; i < remaining.size(); ++i) {
      float minDist = FLT_MAX;

      for (const auto &s : selected) {
        float dist = colorDistance(remaining[i], s);
        minDist = min(minDist, dist);
      }

      if (minDist > maxMinDistance) {
        maxMinDistance = minDist;
        bestIndex = i;
      }
    }

    selected.push_back(remaining[bestIndex]);
    remaining.erase(remaining.begin() + bestIndex);
  }

  return selected;
}
