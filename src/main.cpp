#include "color_selector.hpp"
#include "color_utils.hpp"
#include "cxxopts.hpp"
#include "kmeans_wrapper.hpp"
#include "template_engine.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

// namespace fs = std::filesystem;
// using namespace std;
// using namespace cv;
using json = nlohmann::json;

struct AppConfig {
  std::string imagePath;
  std::string outputPath;
  std::string templatePath;
  bool verbose;
};

AppConfig parseArguments(int argc, char *argv[]) {
  cxxopts::Options options(
      "huegen",
      "A C++ tool that extracts visually distinct color palettes from images");

  options.add_options()("h,help", "Prints help.")(
      "i,image", "Path to the image file", cxxopts::value<std::string>())(
      "o,output", "Output folder path",
      cxxopts::value<std::string>()->default_value(
          std::string(std::getenv("HOME")) + "/.config/huegen/themes/"))(
      "t,template", "Template folder path",
      cxxopts::value<std::string>()->default_value(
          std::string(std::getenv("HOME")) + "/.config/huegen/templates/"))(
      "v,verbose", "Enable verbose logging",
      cxxopts::value<bool>()->default_value("false"));

  auto result = options.parse(argc, argv);

  if (result.count("help") || argc == 1) {
    std::cout << options.help() << std::endl;
    exit(0); // exit here, not return wrong type
  }

  AppConfig config;
  config.imagePath = result["image"].as<std::string>();
  config.outputPath = result["output"].as<std::string>();
  config.templatePath = result["template"].as<std::string>();
  config.verbose = result["verbose"].as<bool>();

  return config;
}

void ensureDirectories(const AppConfig &config) {
  try {
    if (!fs::exists(config.outputPath)) {
      fs::create_directories(config.outputPath);
    }
    if (!fs::exists(config.templatePath)) {
      fs::create_directories(config.templatePath);
    }
  } catch (const fs::filesystem_error &e) {
    throw std::runtime_error(std::string("Filesystem error: ") + e.what());
  }
}

// processes Image and extract colors
std::vector<cv::Vec3f> processImage(const std::string &imagePath) {
  cv::Mat img = cv::imread(imagePath);
  if (img.empty()) {
    throw std::runtime_error("Failed to load image: " + imagePath);
  }
  cv::resize(img, img, cv::Size(200, 200));
  img.convertTo(img, CV_32F, 1.0 / 255.0);
  cv::Mat lab;
  cv::cvtColor(img, lab, cv::COLOR_BGR2Lab);
  auto allColors = extractClusterColors(lab, 32);
  std::vector<cv::Vec3f> filtered;
  for (const auto &c : allColors) {
    if (c[0] > 30.0f)
      filtered.push_back(c);
  }
  return filtered;
}

int main(int argc, char *argv[]) {
  try {
    auto config = parseArguments(argc, argv);
    ensureDirectories(config);
    std::vector<cv::Vec3f> filtered = processImage(config.imagePath);

    auto distinctColors = selectMostDistinctColors(filtered, 16);

    sort(distinctColors.begin(), distinctColors.end(),
         [](const cv::Vec3f &a, const Vec3f &b) {
           return calculateSaturation(a) > calculateSaturation(b);
         });

    nlohmann::json colorJson = colorsToJson(distinctColors);

    bool success =
        processTemplates(colorJson, config.templatePath, config.outputPath);
    if (success) {
      std::cout << "Template processing completed successfully!" << std::endl;
    } else {
      std::cout << "Template processing failed!" << std::endl;
      return 1;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error" << e.what() << std::endl;
    return 1;
  }
  return 0;
}
