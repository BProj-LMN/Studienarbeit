/*
 * Logger.h
 *
 * function: Logger Singleton for logging with different log levels to a file
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include <iostream>
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
public:
  static Logger& getLogger() {
    static Logger Instance;
    return Instance;
  }

  Logger();
  ~Logger();

  void setLogLevel(log_t logLevel);
  log_t getLogLevel();
  std::ofstream& log(log_t logType);
  std::ofstream& log();

private:
  std::ofstream logfile;
  std::chrono::high_resolution_clock::time_point time_start;
  log_t logLevel;
};

/*
 * Logging the entry and exit of functions.
 * Functions will create an object of LogScope, which will be destroyed when exiting the scope of the function
 *
 * simply write LOG_SCOPE; (as a shortcut) as the first statement of your function
 */
class LogScope {
public:
  LogScope(const std::string& s);
  ~LogScope();

private:
  Logger& logger;
  std::string s_;
};

#endif /* SRC_LOGGER_H_ */
