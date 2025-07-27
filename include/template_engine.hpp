#ifndef TEMPLATEENGINE_HPP
#define TEMPLATEENGINE_HPP

#include "color_utils.hpp"
#include <filesystem> // Add this
#include <fstream>    // Add this
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <regex>
#include <vector>

using namespace cv;
using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem; // Add this

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

bool processTemplates(const json &colorJson, const string &inputDir,
                      const string &outputDir);

string replaceColorPlaceholders(const string &content, const json &colorJson);
#endif // TEMPLATEENGINE_HPP
