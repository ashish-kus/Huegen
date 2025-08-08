// native_host_example.cpp
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
using json = nlohmann::json;

void send_message(const std::string &msg) {
  uint32_t len = (uint32_t)msg.size();
  std::cout.write(reinterpret_cast<const char *>(&len), 4);
  std::cout << msg;
  std::cout.flush();
}

int main() {
  std::string home = std::getenv("HOME") ? std::getenv("HOME") : ".";
  std::string path = home + "/.config/huegen/themes/colors.json";

  std::ifstream ifs(path);
  if (!ifs.is_open()) {
    send_message(R"({})");
    return 0;
  }

  json palette;
  try {
    ifs >> palette;
  } catch (...) {
    send_message(R"({})");
    return 0;
  }

  send_message(palette.dump());
  return 0;
}
