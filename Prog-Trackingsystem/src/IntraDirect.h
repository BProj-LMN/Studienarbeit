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
  std::vector<IntraSysMsg> messageList;

public:
  IntraDirect();
  ~IntraDirect();

  void send(IntraSysMsg& msg);
  Status recv(IntraSysMsg& msg);
};

#endif /* SRC_INTRADIRECT_H_ */
