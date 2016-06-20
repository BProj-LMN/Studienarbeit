/*
 * ImageProcessingMngmt.h
 *
 * function: manage all ImageProcessing entities
 *           loads sysConfig file and sets everything up
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_IMAGEPROCESSINGMNGMT_H_
#define SRC_IMAGEPROCESSINGMNGMT_H_

#include "ImageProcessing.h"
#include "CameraProperties.h"

#include <opencv2/core.hpp>

#include <string>
#include <vector>

class CameraProperties;

class ImageProcessingMngmt {
public:
  ImageProcessingMngmt(std::string configFile, IntraSystemMessaging* intMsg);
  ~ImageProcessingMngmt();

  void evaluate();

private:
  void factoryCamera(CameraProperties camProps);

private:
  std::vector<ImageProcessing*> cameras;
  IntraSystemMessaging* messaging;
};

#endif /* SRC_IMAGEPROCESSINGMNGMT_H_ */
