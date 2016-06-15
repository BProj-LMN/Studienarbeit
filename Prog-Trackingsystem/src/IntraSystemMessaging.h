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
//#include "Logger.h"

class IntraSystemMessaging {
public:
  virtual ~IntraSystemMessaging(){};

  virtual void send(IntraSysMsg msg) = 0;
  virtual IntraSysMsg recv() = 0;
};

#endif /* SRC_INTRASYSTEMMESSAGING_H_ */
