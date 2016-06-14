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

#include <opencv2/core.hpp>

#include "Camera.h"

class ObjDetSimple {
  Camera* cam;
  cv::Mat refereceFrame;

public:
  ObjDetSimple(Camera* cam);
  virtual ~ObjDetSimple();

  int setReferenceFrame(cv::Mat frame);
  int detectObject(cv::Mat frame, cv::Point2i& pixelPosition);

private:
  int getObjectPosition(cv::Mat thresImg, cv::Point2i& objectPos, cv::Rect* boundingRect);
};

#endif /* SRC_OBJDETSIMPLE_H_ */
