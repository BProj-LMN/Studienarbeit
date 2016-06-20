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
#include "triangulate.h"

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

  int triangulationMinDistance{0};
  Pos3D position;
  char errorCode{0x00};

  // TODO: receive from IntraSystemMessaging
  // foreach camera, if IntraSysMsg.trackingStatus == ERR
  // errorCode |= ERR_TRACKING_LOST;

  // TODO: triangulate
  if (triangulationMinDistance > DIST_ERR_CAT1) {
    errorCode |= ERR_BIG_DISTANCE;
  }

  externalCom->sendData(position, errorCode);

  /*
   * console output
   */
  std::cout << "x " << position.x << "\ty " << position.y << "\tz " << position.z;
  std::cout << "\t\t" << "distance rays: " << triangulationMinDistance;
  std::cout << "\t\t" << "error code: ";
  fprintf(stdout, "0x%2x", errorCode);
  std::cout << "\n";
}
