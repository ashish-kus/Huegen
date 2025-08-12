#include "logger.hpp"

namespace log {
bool verboseEnabled = false;

void setVerbose(bool enabled) { verboseEnabled = enabled; }

void error(const std::string &msg) {
  std::cerr << color::red << "[ERROR] " << msg << color::reset << std::endl;
}

void info(const std::string &msg) {
  std::cout << color::green << "[INFO] " << msg << color::reset << std::endl;
}

void debug(const std::string &msg) {
  if (verboseEnabled) {
    std::cout << color::blue << "[DEBUG] " << msg << color::reset << std::endl;
  }
}
} // namespace log
