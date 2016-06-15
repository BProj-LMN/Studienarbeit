/*
 * ObjectDetection.h
 *
 * function: Interface for different object detection implementations
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_OBJECTDETECTION_H_
#define SRC_OBJECTDETECTION_H_

#include "DataFormats.h"
//#include "Logger.h"

class ObjectDetection {
protected:
  cv::Mat referenceFrame;

public:
  virtual ~ObjectDetection() {
  }

  virtual ReturnStatus detect(cv::Mat frame, PxPosList& position) = 0;
  virtual void setReferenceFrame(cv::Mat frame) = 0;
};

#endif /* SRC_OBJECTDETECTION_H_ */
