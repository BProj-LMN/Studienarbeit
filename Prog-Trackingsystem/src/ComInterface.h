/*
 * ComInterface.h
 *
 * function: Interface for different protocols sending the position data
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_COMINTERFACE_H_
#define SRC_COMINTERFACE_H_

class ComInterface {
  // TODO: connection with udp socket server

public:
  virtual void evaluate();
  virtual void sendData();
};

#endif /* SRC_COMINTERFACE_H_ */
