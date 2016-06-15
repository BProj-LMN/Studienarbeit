/*
 * Bachelorprojekt.h
 *
 * function: UDP messaging protocol used in the Bachelorprojekt WS 2015/16
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_BACHELORPROJEKT_H_
#define SRC_BACHELORPROJEKT_H_

#include "ComInterface.h"

#include "DataFormats.h"
//#include "Logger.h"

#define ERR_TRACKING_LOST 0x01
#define ERR_BIG_DISTANCE  0x02
#define DIST_ERR_CAT1     100

class ComBachelorprojekt: public ComInterface {
public:
  ComBachelorprojekt(UdpSocketServer* socketServer) {
    socket = socketServer;
  }

  ~ComBachelorprojekt() {
    delete socket;
  }

  void evaluate() {
    /*
     * evaluate remote input - remote control this software
     */
    std::string message;
    socket->evaluate();

    bool newMessage = socket->get_message(message);

    if (newMessage) {
      // shut down, if requested
      if (message.compare("exit")) {
        // TODO: throw exception or something to quit application
      }
    }

  }
  void sendData(Pos3D& position, char errorCode) {
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
};

#endif /* SRC_BACHELORPROJEKT_H_ */
