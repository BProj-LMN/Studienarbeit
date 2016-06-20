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
#include "Logger.h"

#include "ObjDetSimple.h"
#include "IntraDirect.h"

#include <iostream>

ImageProcessingMngmt::ImageProcessingMngmt(std::string configFile, IntraSystemMessaging* messagingSystem) {
  LOG_SCOPE;

  messaging = messagingSystem;

  cv::FileStorage fs(configFile, cv::FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    LOG_ERROR << "ImageProcessingMngmt::ctor - sysConfig settings file could not be opened\n";
    throw Error("sysConfig settings file could not be opened");
  }

  std::string objDetUsed;
  std::vector<CameraProperties> cameras;

  fs["objectDetectionUsed"] >> objDetUsed;
  fs["cameras"] >> cameras;
  fs.release();

  for (CameraProperties c : cameras) {
    factoryCamera(c);
  }
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
}

void ImageProcessingMngmt::factoryCamera(CameraProperties camProps) {
  std::cout << "ImageProcessingMngmt - create " << camProps << "\n";

  ImageSource* cap = new ImageSource{camProps.videoSrc};
  if (!cap->isOpened()) {
    LOG_ERROR << "ImageProcessingMngmt::factoryCamerar - error opening VideoCapture - videoSrc=" << camProps.videoSrc << "\n";
    std::cout << "videoSrc=" << camProps.videoSrc << "\n";
    throw Error("ImageProcessingMngmt::factoryCamera - error opening VideoCapture\n");
  }

  CameraParams* cam = new CameraParams{camProps.configFile};
  ObjectDetection* objDet = new ObjDetSimple{}; // TODO switch between different object detections

  ImageProcessing* camera = new ImageProcessing{camProps.cameraID, cap, cam, objDet, messaging};
  cameras.push_back(camera);

  std::cout << "ImageProcessingMngmt - camera " << camProps.cameraID << " created\n\n";
}

