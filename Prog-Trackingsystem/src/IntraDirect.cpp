/*
 * IntraDirect.cpp
 *
 * function: basic implementation of IntraSystemMessaging
 *
 * author: Jannik Beyerstedt
 */

#include "IntraDirect.h"

//#include "DataFormats.h"
#include "Logger.h"

IntraDirect::IntraDirect()
    : messageList() {
  LOG_SCOPE;
}

IntraDirect::~IntraDirect() {
  LOG_SCOPE;
}

void IntraDirect::send(IntraSysMsg& msg) {
  messageList.push_back(msg);
}

Status IntraDirect::recv(IntraSysMsg& msg) {
  if (messageList.size() > 0) {
    msg = messageList[0]; // get first item
    messageList.erase(messageList.begin()); // remove item from list
    return OK;
  } else {
    return ERR;
  }
}

