#include "template_engine.hpp"
#include <algorithm>
#include <filesystem> // Add this
#include <fstream>    // Add this
#include <iomanip>
#include <regex> // Add this
#include <sstream>

using namespace std;
using namespace cv;
using json = nlohmann::json;
namespace fs = std::filesystem; // Add this

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
        {"strip", strip(rgbColor)},
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

bool processTemplates(const json &colorJson, const string &inputDir,
                      const string &outputDir) {
  try {
    // Create output directory if it doesn't exist
    if (!fs::exists(outputDir)) {
      fs::create_directories(outputDir);
    }

    // Find all .tlp files in input directory
    for (const auto &entry : fs::directory_iterator(inputDir)) {
      if (entry.path().extension() == ".tlp") {
        string inputPath = entry.path().string();
        string filename = entry.path().stem().string(); // Remove .tlp extension
        string outputPath = outputDir + "/" + filename;

        // Read template file
        ifstream inputFile(inputPath);
        if (!inputFile.is_open()) {
          cerr << "Error: Could not open template file: " << inputPath << endl;
          continue;
        }

        string content((istreambuf_iterator<char>(inputFile)),
                       istreambuf_iterator<char>());
        inputFile.close();

        // Replace color placeholders
        string processedContent = replaceColorPlaceholders(content, colorJson);

        // Write output file
        ofstream outputFile(outputPath);
        if (!outputFile.is_open()) {
          cerr << "Error: Could not create output file: " << outputPath << endl;
          continue;
        }

        outputFile << processedContent;
        outputFile.close();

        cout << "Processed: " << filename << endl;
      }
    }

    return true;

  } catch (const exception &e) {
    cerr << "Error processing templates: " << e.what() << endl;
    return false;
  }
}

string replaceColorPlaceholders(const string &content, const json &colorJson) {
  string result = content;

  // Create regex pattern to match {colorN.property} placeholders
  regex colorPattern(R"(\{(color\d+)\.([^}]+)\})");
  sregex_iterator iter(content.begin(), content.end(), colorPattern);
  sregex_iterator end;

  // Process matches in reverse order to avoid position shifts
  vector<pair<size_t, pair<size_t, string>>> replacements;

  for (auto it = iter; it != end; ++it) {
    smatch match = *it;
    string colorKey = match[1].str(); // e.g., "color0"
    string property = match[2].str(); // e.g., "hex"

    // Check if the color and property exist in JSON
    if (colorJson.contains(colorKey) &&
        colorJson[colorKey].contains(property)) {
      string replacement;

      // Handle different property types
      if (colorJson[colorKey][property].is_string()) {
        replacement = colorJson[colorKey][property].get<string>();
      } else if (colorJson[colorKey][property].is_number()) {
        replacement = to_string(colorJson[colorKey][property].get<double>());
      } else if (colorJson[colorKey][property].is_object()) {
        // For nested objects like rgb_values.r
        replacement = colorJson[colorKey][property].dump();
      }

      //      replacements.push_back({match.position(), {match.length(),
      //      replacement}});
      replacements.push_back(
          {static_cast<size_t>(match.position()),
           {static_cast<size_t>(match.length()), replacement}});
    } else {
      cerr << "Warning: Color placeholder not found: {" << colorKey << "."
           << property << "}" << endl;
    }
  }

  // Apply replacements in reverse order
  sort(replacements.rbegin(), replacements.rend());
  for (const auto &replacement : replacements) {
    size_t pos = replacement.first;
    size_t len = replacement.second.first;
    string newValue = replacement.second.second;

    result.replace(pos, len, newValue);
  }

  return result;
}
