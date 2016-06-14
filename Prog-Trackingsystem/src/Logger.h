/*
 * Logger.h
 *
 *  Created on: 09.06.2016
 *      Author: jannik
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>

/* for Logger Class */
#define LOG_DEBUG \
  if (Logger::getLogger().getLogLevel() < DEBUG) ; \
  else Logger::getLogger().log(DEBUG)
#define LOG_WARNING \
  if (Logger::getLogger().getLogLevel() < WARNING) ; \
  else Logger::getLogger().log(WARNING)
#define LOG_ERROR \
  if (Logger::getLogger().getLogLevel() < ERROR) ; \
  else Logger::getLogger().log(ERROR)

#define LOG_SET_LEVEL_DEBUG Logger::getLogger().setLogLevel(DEBUG)
#define LOG_SET_LEVEL_WARNING Logger::getLogger().setLogLevel(WARNING)
#define LOG_SET_LEVEL_ERROR Logger::getLogger().setLogLevel(ERROR)

/* for LogScope Class */
#define LOG_SCOPE LogScope log(__FUNCTION__)

/*
 * Logger class for centralized logging.
 * for more convenient usage, use LOG_DEBUG, LOG_WARNING or LOG_ERROR
 * and pipe to this as if it’s std::err
 * use LOG_SET_LEVEL_* to set the debug level written to the logfile
 */
typedef enum {
  ERROR, WARNING, DEBUG
} log_t;

class Logger {
  std::ofstream logfile;
  std::chrono::high_resolution_clock::time_point time_start;
  log_t logLevel;

public:
  static Logger& getLogger() {
    static Logger Instance;
    return Instance;
  }
  Logger() {
    logfile.open("logfile.txt", std::ios::trunc);
    logfile << std::setprecision(3) << std::fixed;

    time_start = std::chrono::high_resolution_clock::now();
    logLevel = ERROR;
  }
  ~Logger() {
    logfile.close();
  }
  void setLogLevel(log_t logLevel) {

    this->logLevel = logLevel;
  }
  log_t getLogLevel() {
    return logLevel;
  }
  std::ofstream& log(log_t logType) {
    std::chrono::duration<double, std::milli> timediff_ms = std::chrono::high_resolution_clock::now() - time_start;
    auto time = std::chrono::system_clock::now();

    logfile << std::chrono::system_clock::to_time_t(time) << " (" << timediff_ms.count() << ") [" << logType << "] ";
    return logfile;
  }
  std::ofstream& log() {
    std::chrono::duration<double, std::milli> timediff_ms = std::chrono::high_resolution_clock::now() - time_start;
    auto time = std::chrono::system_clock::now();

    logfile << std::chrono::system_clock::to_time_t(time) << " (" << timediff_ms.count() << ") ";
    return logfile;
  }
};

/*
 * Logging the entry and exit of functions.
 * Functions will create an object of LogScope, which will be destroyed when exiting the scope of the function
 *
 * simply write LOG_SCOPE; (as a shortcut) as the first statement of your function
 */
class LogScope {
public:
  LogScope(const std::string& s)
      : logger(Logger::getLogger()), s_(s) {
    logger.log() << "entering function " << s_ << "\n";
  }
  ~LogScope() {
    logger.log() << "exiting function " << s_ << "\n";
  }

private:
  Logger& logger;
  std::string s_;
};

#endif /* SRC_LOGGER_H_ */
