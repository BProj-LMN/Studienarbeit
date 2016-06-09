/*
 * objectDetection.h
 *
 * function: detect object in an image
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_OBJECTDETECTION_H_
#define SRC_OBJECTDETECTION_H_

//#define SHOW_THESHOLD // for debugging the object detection

#include <opencv2/core.hpp>
using namespace cv;

#include "Camera.h"

class ObjectDetection {
  Camera* cam;
  Mat refereceFrame;

public:
  ObjectDetection(Camera* cam);
  virtual ~ObjectDetection();
  
  int setReferenceFrame(Mat frame);
  int detectObject(Mat frame, Point2i& pixelPosition);

private:
  int getObjectPosition(Mat thresImg, Point2i&  objectPos, Rect* boundingRect);
};

#endif /* SRC_OBJECTDETECTION_H_ */
