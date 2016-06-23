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
#include "Logger.h"

class IntraDirect: public IntraSystemMessaging {
  std::vector<IntraSysMsg> messageList;

public:
  IntraDirect()
      : messageList() {
    LOG_SCOPE;
  }
  virtual ~IntraDirect() {
    LOG_SCOPE;
  }

  void send(IntraSysMsg& msg) {
    messageList.push_back(msg);
  }

  Status recv(IntraSysMsg& msg) {
    if (messageList.size() > 0) {
      msg = messageList[0]; // get first item
      messageList.erase(messageList.begin()); // remove item from list
      return OK;
    } else {
      return ERR;
    }
  }
};

#endif /* SRC_INTRADIRECT_H_ */
