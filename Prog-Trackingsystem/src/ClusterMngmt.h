/*
 * ClusterMngmt.h
 *
 * function: combine data of all cameras to calculate a 3D position
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_CLUSTERMNGMT_H_
#define SRC_CLUSTERMNGMT_H_

#include "IntraSystemMessaging.h"
#include "CameraProperties.h"

class ClusterMngmt {
public:
  ClusterMngmt(std::string configFile, IntraSystemMessaging* messagingSystem);
  virtual ~ClusterMngmt();

  void evaluate();

private:
  IntraSystemMessaging* messaging;
};

#endif /* SRC_CLUSTERMNGMT_H_ */
