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

#include "Camera.h"

class ObjectDetection {
  Camera* cam;
  cv::Mat refereceFrame;

public:
  ObjectDetection(Camera* cam);
  virtual ~ObjectDetection();

  int setReferenceFrame(cv::Mat frame);
  int detectObject(cv::Mat frame, cv::Point2i& pixelPosition);

private:
  int getObjectPosition(cv::Mat thresImg, cv::Point2i& objectPos, cv::Rect* boundingRect);
};

#endif /* SRC_OBJECTDETECTION_H_ */
