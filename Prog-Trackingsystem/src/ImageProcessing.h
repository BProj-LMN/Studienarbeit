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
#include "IntraSystemMessaging.h"
#include "ImageSource.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>

class ImageProcessing {
public:
  ImageProcessing(int cameraID, ImageSource* src, Camera* camera, ObjectDetection* objDetection, IntraSystemMessaging* msgQueue);
  ~ImageProcessing();

  void evaluate();

private:
  int camID;
  ImageSource* cap;
  Camera* cam;
  ObjectDetection* objDet;
  IntraSystemMessaging* messaging;
};

#endif /* SRC_IMAGEPROCESSING_H_ */
