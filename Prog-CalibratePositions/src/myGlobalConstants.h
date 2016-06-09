/*
 * globalConstants.h
 *
 * function: define global enums and functions, error handling, ...
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_MYGLOBALCONSTANTS_H_
#define SRC_MYGLOBALCONSTANTS_H_

enum {
  ERR = -1, OK = 0
};

#ifndef _WIN32
enum {
  FALSE = 0, TRUE = 1
};
#endif

/*
 * error scheme
 *
 * fprintf(stderr, "ERROR: in {functionName} - {custom message, error codes, etc.} \n");
 */

#endif /* SRC_MYGLOBALCONSTANTS_H_ */
