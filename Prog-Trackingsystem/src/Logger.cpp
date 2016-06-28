/*
 * Logger.cpp
 *
 * function: Logger Singleton for logging with different log levels to a file
 *
 * author: Jannik Beyerstedt
 */

#include "Logger.h"

#include <iomanip>

Logger::Logger() {
  logfile.open("logfile.txt", std::ios::trunc);
  logfile << std::setprecision(3) << std::fixed;

  time_start = std::chrono::high_resolution_clock::now();
  logLevel = ERROR;
}

Logger::~Logger() {
  logfile.close();
}

void Logger::setLogLevel(log_t logLevel) {
  this->logLevel = logLevel;
}

log_t Logger::getLogLevel() {
  return logLevel;
}

std::ofstream& Logger::log(log_t logType) {
  std::chrono::duration<double, std::milli> timediff_ms = std::chrono::high_resolution_clock::now() - time_start;
  auto time = std::chrono::system_clock::now();

  logfile << std::chrono::system_clock::to_time_t(time) << " (" << timediff_ms.count() << ") [" << logType << "] ";
  return logfile;
}

std::ofstream& Logger::log() {
  std::chrono::duration<double, std::milli> timediff_ms = std::chrono::high_resolution_clock::now() - time_start;
  auto time = std::chrono::system_clock::now();

  logfile << std::chrono::system_clock::to_time_t(time) << " (" << timediff_ms.count() << ") ";
  return logfile;
}

LogScope::LogScope(const std::string& s)
    : logger(Logger::getLogger()), s_(s) {
  if (!(Logger::getLogger().getLogLevel() < DEBUG)) {
    logger.log(DEBUG) << "entering function " << s_ << "\n";
  }
}

LogScope::~LogScope() {
  if (!(Logger::getLogger().getLogLevel() < DEBUG)) {
    logger.log(DEBUG) << "exiting function " << s_ << "\n";
  }
}
