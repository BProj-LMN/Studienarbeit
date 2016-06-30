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
    factoryCamera(c, objDetUsed);
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

void ImageProcessingMngmt::factoryCamera(CameraProperties camProps, std::string objDetUsed) {
  std::cout << "ImageProcessingMngmt - create " << camProps << "\n";

  ImageSource* cap;
  std::string videoSrc = camProps.videoSrc;

#ifndef _WIN32
  try {                   // try to convert to int (device ID)
    int devideID = std::stoi(videoSrc);
    std::cout << "device ID detected\n";

    cap = new ImageSource{devideID};
  } catch(std::invalid_argument e) {  // else leave it as string (video / image file)
    cap = new ImageSource{videoSrc};
  }
#else
  // ugly windows approach assuming less that 100 cameras AND filenames with more that 2 characters
  // because minGW claims to do c++11, but does not have std::stoi implemented
  if (videoSrc.length() < 3) {
    int devideID = atoi(videoSrc.c_str());

    cap = new ImageSource{devideID};
  } else {
    cap = new ImageSource{videoSrc};
  }
#endif
  if (!cap->isOpened()) {
    LOG_ERROR << "ImageProcessingMngmt::factoryCamerar - error opening VideoCapture - videoSrc=" << camProps.videoSrc << "\n";
    std::cout << "videoSrc=" << camProps.videoSrc << "\n";
    throw Error("ImageProcessingMngmt::factoryCamera - error opening VideoCapture\n");
  }

  CameraParams* cam = new CameraParams{camProps.configFile};
  ObjectDetection* objDet;
  if ("ObjDetSimple" == objDetUsed) {
    objDet = new ObjDetSimple{};
  } else {
    throw Error("invalid objectDetectionUsed in sysConfig file!\n if it’s a valid class name, it must be registered manually in ImageProcessingMngmt.cpp!");
  }

  ImageProcessing* camera = new ImageProcessing{camProps.cameraID, cap, cam, objDet, messaging};
  cameras.push_back(camera);

  std::cout << "ImageProcessingMngmt - camera " << camProps.cameraID << " created\n\n";
}

