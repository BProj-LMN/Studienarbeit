/*
 * ObjectDetection.h
 *
 * function: Interface for different object detection implementations
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_OBJECTDETECTION_H_
#define SRC_OBJECTDETECTION_H_

class ObjectDetection {
  cv::Mat referenceFrame;
public:
  virtual PxPosList detect(cv::Mat frame);
  virtual void setReferenceFrame(cv::Mat frame);
};

#endif /* SRC_OBJECTDETECTION_H_ */
