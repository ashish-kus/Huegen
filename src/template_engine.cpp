#include "template_engine.hpp"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace cv;
using json = nlohmann::json;

json colorsToJson(const vector<Vec3f> &labColors) {
  json result = json::object();

  for (size_t i = 0; i < labColors.size(); ++i) {
    const Vec3f &labColor = labColors[i];
    // Convert LAB to BGR then to RGB
    Vec3b bgrColor = labToBgr(labColor);
    Vec3b rgbColor = bgrToRgb(bgrColor);
    // Calculate other color formats
    Vec3f hslColor = rgbToHsl(rgbColor);
    Vec3f hsvColor = rgbToHsv(rgbColor);
    string colorKey = "color" + to_string(i);
    result[colorKey] = {
        {"hex", formatHex(rgbColor)},
        {"rgb", "rgb(" + to_string(rgbColor[0]) + ", " +
                    to_string(rgbColor[1]) + ", " + to_string(rgbColor[2]) +
                    ")"},
        {"rgba", "rgba(" + to_string(rgbColor[0]) + ", " +
                     to_string(rgbColor[1]) + ", " + to_string(rgbColor[2]) +
                     ", 1.0)"},
        {"hsl", "hsl(" + to_string((int)round(hslColor[0])) + ", " +
                    to_string((int)round(hslColor[1])) + "%, " +
                    to_string((int)round(hslColor[2])) + "%)"},
        {"hsv", "hsv(" + to_string((int)round(hsvColor[0])) + ", " +
                    to_string((int)round(hsvColor[1])) + "%, " +
                    to_string((int)round(hsvColor[2])) + "%)"},
        {"lab", "lab(" + to_string((int)round(labColor[0])) + ", " +
                    to_string((int)round(labColor[1])) + ", " +
                    to_string((int)round(labColor[2])) + ")"}};
  }
  return result;
}

string colorsToJsonString(const vector<Vec3f> &labColors, int indent) {
  json colorJson = colorsToJson(labColors);
  return colorJson.dump(indent);
}
