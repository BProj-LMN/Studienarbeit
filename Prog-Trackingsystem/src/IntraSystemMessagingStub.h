/*
 * IntraSystemMessagingStub.h
 *
 * function: stub interface for testing interaction with IntraSystemMessaging
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_INTRASYSTEMMESSAGINGSTUB_H_
#define SRC_INTRASYSTEMMESSAGINGSTUB_H_

#include "IntraSystemMessaging.h"

class IntraSystemMessagingStub: public IntraSystemMessaging {

  IntraSysMsg message;

public:
  void send(IntraSysMsg& msg) {
    message = msg;
  }

  IntraSysMsg getMessage() {
    return message;
  }

  Status recv(IntraSysMsg& msg) {
    return ERR;
  }

  void setMessage(IntraSysMsg msg) {
    message = msg;
  }
};

#endif /* SRC_INTRASYSTEMMESSAGINGSTUB_H_ */
