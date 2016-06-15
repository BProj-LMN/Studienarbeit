/*
 * ImageProcessing.h
 *
 * function: all image processing for one camera
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_IMAGEPROCESSING_H_
#define SRC_IMAGEPROCESSING_H_

#include "Camera.h"
#include "ObjectDetection.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>

class ImageProcessing {
public:
  ImageProcessing(int cameraID, cv::VideoCapture* src, Camera* camera, ObjectDetection* objDetection);
  ~ImageProcessing();

  void evaluate();

private:
  int camID;
  cv::VideoCapture* cap;
  Camera* cam;
  ObjectDetection* objDet;
};

#endif /* SRC_IMAGEPROCESSING_H_ */