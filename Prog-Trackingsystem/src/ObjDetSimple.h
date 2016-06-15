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

#include "DataFormats.h"
//#include "myGlobalConstants.h"
//#include "Logger.h"

#include "ObjectDetection.h"

#include <opencv2/core.hpp>

class ObjDetSimple: public ObjectDetection {
  cv::Mat refereceFrame;

public:
  virtual ~ObjDetSimple();
  void setReferenceFrame(cv::Mat frame);
  int detect(cv::Mat frame, PxPosList& pxPosition);

private:
  int getObjectPosition(cv::Mat thresImg, cv::Point2i& objectPos, cv::Rect* boundingRect);
};

#endif /* SRC_OBJDETSIMPLE_H_ */
