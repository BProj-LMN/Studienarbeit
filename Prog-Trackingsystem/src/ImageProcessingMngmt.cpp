/*
 * ImageProcessingMngmt.cpp
 *
 * function: manage all ImageProcessing entities
 *           loads sysConfig file and sets everything up
 *
 * author: Jannik Beyerstedt
 */

#include "ImageProcessingMngmt.h"

//#include "DataFormats.h"
//#include "myGlobalConstants.h"
#include "Logger.h"

#include "ObjDetSimple.h"

#include <iostream>

ImageProcessingMngmt::ImageProcessingMngmt(std::string configFile) {
  LOG_SCOPE;

  cv::FileStorage fs(configFile, cv::FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    LOG_ERROR << "ImageProcessingMngmt::ctor - settings file could not be openend\n";
    std::cout << "ImageProcessingMngmt::ctor - settings file could not be openend\n";
    // TODO: exit / throw exception
  }

  std::string objDet;
  std::vector<CameraProperties> cameras;

  fs["objectDetectionUsed"] >> objDet;
  fs["cameras"] >> cameras;

  for (CameraProperties c : cameras) {
    std::cout << c << "\n";

    parseConfigAndFactory(c);
  }

  // TODO: setup reference frame for each camera

}

ImageProcessingMngmt::~ImageProcessingMngmt() {
  LOG_SCOPE;

  for (ImageProcessing* cam : cameras) {
    delete cam;
  }

}

void ImageProcessingMngmt::evaluate() {

  for (ImageProcessing* cam : cameras) {
    cam->evaluate();
  }

  std::cout << "\n";
}

void ImageProcessingMngmt::parseConfigAndFactory(CameraProperties camProps) {
  cv::VideoCapture* cap = new cv::VideoCapture{camProps.videoSrc};
  if (!cap->isOpened()) {
    LOG_ERROR << "ImageProcessingMngmt::parseConfigAndFactory - error opening VideoCapture\n";
    std::cout << "ImageProcessingMngmt::parseConfigAndFactory - error opening VideoCapture\n";
    std::cout << "videoSrc=" << camProps.videoSrc << "\n";
    return;
  }

  Camera* cam = new Camera{camProps.configFile};
  ObjectDetection* objDet = new ObjDetSimple{}; // TODO switch between different object detections

  ImageProcessing* camera = new ImageProcessing{camProps.cameraID, cap, cam, objDet};
  cameras.push_back(camera);

  std::cout << "ImageProcessingMngmt - camera " << camProps.cameraID << " created\n\n";
}

