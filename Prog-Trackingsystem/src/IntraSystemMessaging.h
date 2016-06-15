/*
 * IntraSystemMessaging.h
 *
 * function: Interface for different communication protocols
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_INTRASYSTEMMESSAGING_H_
#define SRC_INTRASYSTEMMESSAGING_H_

#include "DataFormats.h"
//#include "myGlobalConstants.h"
//#include "Logger.h"

class IntraSystemMessaging {
public:
  virtual void send(IntraSysMsg msg);
  virtual IntraSysMsg recv();
};

#endif /* SRC_INTRASYSTEMMESSAGING_H_ */
