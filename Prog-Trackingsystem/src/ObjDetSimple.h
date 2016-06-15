/*
 * ObjDetSimple.h
 *
 * function: detect object in an image
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_OBJDETSIMPLE_H_
#define SRC_OBJDETSIMPLE_H_

//#define SHOW_THESHOLD // for debugging the object detection

#include "ObjectDetection.h"

#include <opencv2/core.hpp>

class ObjDetSimple: public ObjectDetection {
  cv::Mat referenceFrame;

public:
  virtual ~ObjDetSimple();
  void setReferenceFrame(cv::Mat frame);
  ReturnStatus detect(cv::Mat frame, PxPosList& pxPosition);

private:
  ReturnStatus getObjectPosition(cv::Mat thresImg, cv::Point2i& objectPos, cv::Rect* boundingRect);
};

#endif /* SRC_OBJDETSIMPLE_H_ */
