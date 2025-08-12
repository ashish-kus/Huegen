#include "color_selector.hpp"
#include "color_utils.hpp"
#include "kmeans_wrapper.hpp"
#include "template_engine.hpp"
#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using namespace cv;
using json = nlohmann::json;
int
main(int argc, char** argv)
{
  if (argc < 2)
  {
    cerr << "Usage: ./heugen <image_path>\n";
    return 1;
  }
  Mat img = imread(argv[1]);
  if (img.empty())
  {
    cerr << "Failed to load image: " << argv[1] << "\n";
    return 1;
  }
  resize(img, img, Size(200, 200));
  img.convertTo(img, CV_32F, 1.0 / 255.0);
  Mat lab;
  cvtColor(img, lab, COLOR_BGR2Lab);
  auto allColors = extractClusterColors(lab, 32);
  vector<Vec3f> filtered;
  for (const auto& c : allColors)
  {
    if (c[0] > 30.0f)
      filtered.push_back(c);
  }
  auto distinctColors = selectMostDistinctColors(filtered, 16);
  sort(distinctColors.begin(), distinctColors.end(),
       [](const Vec3f& a, const Vec3f& b) {
         return calculateSaturation(a) > calculateSaturation(b);
       });

  nlohmann::json colorJson = colorsToJson(distinctColors);

  std::string templateDir =
    std::string(std::getenv("HOME")) + "/.config/huegen/templates/";
  std::string outputDir =
    std::string(std::getenv("HOME")) + "/.config/huegen/themes/";

  bool success           = processTemplates(colorJson, templateDir, outputDir);
  std::string outputFile = outputDir + "colors.json";
  std::ofstream file(outputFile);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not open file " << outputDir << "\n";
    return 1;
  }

  file << colorJson.dump(4); // Pretty-print with 4-space indentation
  file.close();

  if (success)
  {
    cout << "Template processing completed successfully!" << endl;
  }
  else
  {
    cout << "Template processing failed!" << endl;
    return 1;
  }
  return 0;
}
