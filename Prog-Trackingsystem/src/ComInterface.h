/*
 * ComInterface.h
 *
 * function: Interface for different protocols sending the position data
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_COMINTERFACE_H_
#define SRC_COMINTERFACE_H_

#include "DataFormats.h"
//#include "Logger.h"

#include "UdpSocketServer.h"

class ComInterface {
protected:
  UdpSocketServer* socket;

public:
  //ComInterface(UdpSocketServer* socket);
  virtual ~ComInterface() {
  }

  virtual void evaluate() = 0;
  virtual void sendData(Pos3D& position, char errorCode) = 0; // TODO: extend to multiple positions?
};

#endif /* SRC_COMINTERFACE_H_ */
