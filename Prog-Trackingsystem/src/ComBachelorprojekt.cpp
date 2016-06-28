/*
 * ComBachelorprojekt.cpp
 *
 * function: UDP messaging protocol used in the Bachelorprojekt WS 2015/16
 *
 * author: Jannik Beyerstedt
 */

#include "ComBachelorprojekt.h"

//#include "DataFormats.h"
#include "Logger.h"

ComBachelorprojekt::ComBachelorprojekt(UdpSocketServer* socketServer) {
  LOG_SCOPE;

  socket = socketServer;
}

ComBachelorprojekt::~ComBachelorprojekt() {
  LOG_SCOPE;

  delete socket;
}

void ComBachelorprojekt::evaluate() {
  /*
   * evaluate remote input - remote control this software
   */
  std::string message;
  socket->evaluate();

  bool newMessage = socket->get_message(message);

  if (newMessage) {
    // shut down, if requested
    if (message.compare("exit")) {
      throw Error("remote exit command received"); // throw exception to abort/ exit application
    }
  }
}

void ComBachelorprojekt::sendData(Pos3D& position, char errorCode) {
  char positionData[MESSAGE_LEN];

  positionData[0] = 0xDA;
  positionData[1] = char(position.x >> 8);
  positionData[2] = char(position.x);
  positionData[3] = char(position.y >> 8);
  positionData[4] = char(position.y);
  positionData[5] = char(position.z >> 8);
  positionData[6] = char(position.z);
  positionData[7] = errorCode;

  socket->sendMessage(positionData, 8);
}
