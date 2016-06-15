/*
 * IntraDirect.h
 *
 * function: basic implementation of IntraSystemMessaging
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_INTRADIRECT_H_
#define SRC_INTRADIRECT_H_

#include "IntraSystemMessaging.h"

//#include "DataFormats.h"
//#include "Logger.h"

class IntraDirect: public IntraSystemMessaging {
  IntraSysMsg msg;
public:
  IntraDirect()
      : msg() {
  }
  virtual ~IntraDirect() {}

  void send(IntraSysMsg msg) {
    this->msg = msg;
    // TODO: save in vector / queue
  }

  IntraSysMsg recv() {
    return msg;
    // TODO: return from vector / queue
  }
};

#endif /* SRC_INTRADIRECT_H_ */
