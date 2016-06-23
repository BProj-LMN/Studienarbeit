/*
 * ClusterMngmt.cpp
 *
 * function: combine data of all cameras to calculate a 3D position
 *
 * author: Jannik Beyerstedt
 */

#include "ClusterMngmt.h"

#include "ComBachelorprojekt.h"
#include "Logger.h"

#include "UdpSocketServer.h"
#include "Triangulation.h"

ClusterMngmt::ClusterMngmt(std::string configFile, IntraSystemMessaging* intMsg) {
  LOG_SCOPE;

  internalCom = intMsg;

  cv::FileStorage fs(configFile, cv::FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    LOG_ERROR << "ClusterMngmt::ctor - sysConfig settings file could not be opened\n";
    throw Error("sysConfig settings file could not be opened");
  }

  std::string comIntUsed;
  int udpPort;

  fs["comInterfaceUsed"] >> comIntUsed;
  fs["cameras"] >> cameras;
  fs["port"] >> udpPort;
  fs.release();

  UdpSocketServer* socket = new UdpSocketServer(udpPort);
  externalCom = new ComBachelorprojekt(socket); // TODO switch between different com interfaces
}

ClusterMngmt::~ClusterMngmt() {
  LOG_SCOPE;

  delete externalCom;
  // do not delete messaging, because pointer is held by main!!!
}

void ClusterMngmt::evaluate() {
  externalCom->evaluate();

  Pos3D position{};
  char errorCode{0x00};
  std::vector<Pos3D> positions{};
  std::vector<int> triangulationDistances{};
  std::vector<IntraSysMsg> messages{};

  // receive from IntraSystemMessaging
  IntraSysMsg receivedMessage{};
  while (internalCom->recv(receivedMessage) != ERR) {
    messages.push_back(receivedMessage);
  }

  // TODO: assure, that exactly one message from each camera is received!
  // TODO: assure, that the time stamps of the messages are in a certain range.

  // TODO-enh: detect missing cameras

  // set error "tracking lost", if at least one camera has lost tracking
  for (IntraSysMsg m : messages) {
    if (m.trackingStatus == ERR) {
      errorCode |= ERR_TRACKING_LOST;
    }
  }

  // calculate position
  Triangulation::calculatePosition(messages, positions, triangulationDistances);

  // set error "big object ray distance", if at least one triangulation has detected a big distance
  for (int distance : triangulationDistances) {
    if (distance > DIST_ERR_CAT1) {
      errorCode |= ERR_BIG_DISTANCE;
    }
  }

  externalCom->sendData(position, errorCode);

  /*
   * console output
   */
  std::cout << "x " << position.x << "\ty " << position.y << "\tz " << position.z;
  std::cout << "\t\t" << "error code: ";
  fprintf(stdout, "0x%2x", errorCode);
  std::cout << "\n";
}
