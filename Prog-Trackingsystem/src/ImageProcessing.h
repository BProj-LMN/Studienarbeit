/*
 * ImageProcessing.h
 *
 * function: all image processing for one camera
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_IMAGEPROCESSING_H_
#define SRC_IMAGEPROCESSING_H_

#include "CameraParams.h"
#include "ObjectDetection.h"
#include "IntraSystemMessaging.h"
#include "ImageSource.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class ImageProcessing {
public:
  ImageProcessing(int camID, ImageSource* src, CameraParams* cam, ObjectDetection* objDet, IntraSystemMessaging* intMsg);
  ~ImageProcessing();

  void evaluate();

private:
  int cameraID;
  ImageSource* capture;
  CameraParams* camParams;
  ObjectDetection* objectDet;
  IntraSystemMessaging* internalCom;
};

#endif /* SRC_IMAGEPROCESSING_H_ */
