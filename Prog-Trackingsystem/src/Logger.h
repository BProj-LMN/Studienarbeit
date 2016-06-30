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
  if (Logger::getLogger().getLogLevel() < debug) ; \
  else Logger::getLogger().log(debug)
#define LOG_WARNING \
  if (Logger::getLogger().getLogLevel() < warning) ; \
  else Logger::getLogger().log(warning)
#define LOG_ERROR \
  if (Logger::getLogger().getLogLevel() < error) ; \
  else Logger::getLogger().log(error)

#define LOG_SET_LEVEL_DEBUG Logger::getLogger().setLogLevel(debug)
#define LOG_SET_LEVEL_WARNING Logger::getLogger().setLogLevel(warning)
#define LOG_SET_LEVEL_ERROR Logger::getLogger().setLogLevel(error)

/* for LogScope Class */
#define LOG_SCOPE LogScope log(__FUNCTION__)

/*
 * Logger class for centralized logging.
 * for more convenient usage, use LOG_DEBUG, LOG_WARNING or LOG_ERROR
 * and pipe to this as if it’s std::err
 * use LOG_SET_LEVEL_* to set the debug level written to the logfile
 */
typedef enum {
  error, warning, debug  // can't use ERROR on windows!
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
