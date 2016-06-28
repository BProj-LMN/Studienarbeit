/*
 * ComBachelorprojekt.h
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
  ComBachelorprojekt(UdpSocketServer* socketServer);
  ~ComBachelorprojekt();

  void evaluate();
  void sendData(Pos3D& position, char errorCode);
};

#endif /* SRC_BACHELORPROJEKT_H_ */
