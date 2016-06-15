/*
 * ImageProcessing.cpp
 *
 * function: all image processing for one camera
 *
 * author: Jannik Beyerstedt
 */

#include "ImageProcessing.h"

//#include "DataFormats.h"
//#include "myGlobalConstants.h"
//#include "Logger.h"

ImageProcessing::ImageProcessing(int cameraID, cv::VideoCapture* src, Camera* camera, ObjectDetection* objDetection)
    : camID(cameraID), cap(src), cam(camera), objDet(objDetection) {
  std::cout << "ImageProcessing ctor \n";
}

ImageProcessing::~ImageProcessing() {
  std::cout << "ImageProcessing " << camID << " dtor\n";

  delete cap;
  delete cam;
  delete objDet;
}

void ImageProcessing::evaluate() {
  std::cout << "ImageProcessing evaluate \n";

  // TODO: image processing chain
}
