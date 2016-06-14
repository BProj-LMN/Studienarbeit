/*
 * IntraDirect.h
 *
 * function: TODO
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_INTRADIRECT_H_
#define SRC_INTRADIRECT_H_

#include "IntraSystemMessaging.h"

class IntraDirect: public IntraSystemMessaging {
  IntraSysMsg msg;
public:
  IntraDirect() {
    msg(1, nullptr);
  }
  ~IntraDirect();

  void send(IntraSysMsg msg) {
    this->msg = msg;
  }

  IntraSysMsg recv(){
    return msg;
  }
};

#endif /* SRC_INTRADIRECT_H_ */
