#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>

namespace log {

enum class Level { ERROR, INFO, DEBUG };

extern bool verboseEnabled;

// Color codes for terminal output
namespace color {
const std::string reset = "\033[0m";
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string blue = "\033[34m";
} // namespace color

void setVerbose(bool enabled);

void error(const std::string &msg);
void info(const std::string &msg);
void debug(const std::string &msg); // only shown if verbose is on

} // namespace log

#endif
