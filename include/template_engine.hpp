#ifndef TEMPLATEENGINE_HPP
#define TEMPLATEENGINE_HPP

#include "color_utils.hpp"
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;
using json = nlohmann::json;

/**
 * Convert vector of LAB colors to JSON object with multiple color format
 * representations
 */
json colorsToJson(const vector<Vec3f> &labColors);

/**
 * Convert vector of LAB colors to JSON string with multiple color format
 * representations
 */
string colorsToJsonString(const vector<Vec3f> &labColors, int indent = 2);

#endif // TEMPLATEENGINE_HPP
