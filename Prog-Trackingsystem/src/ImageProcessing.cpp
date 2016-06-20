/*
 * ImageProcessing.cpp
 *
 * function: all image processing for one camera
 *
 * author: Jannik Beyerstedt
 */

#include "ImageProcessing.h"

#include "DataFormats.h"
#include "Logger.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

//#define DEBUG
#define REFERENCE_FRAME_DELAY 60

ImageProcessing::ImageProcessing(int camID, ImageSource* src, CameraParams* cam, ObjectDetection* objDet, IntraSystemMessaging* intMsg)
    : cameraID(camID), capture(src), camParams(cam), objectDet(objDet), internalCom(intMsg) {
  LOG_SCOPE;

  /*
   * set reference frame for tracking
   */
  cv::Mat frame;

  std::cout << "camera " << camID << ": waiting for reference frame...\n" << std::flush;
  cv::namedWindow("reference frame", cv::WINDOW_AUTOSIZE);
  for (int i = 0; i < REFERENCE_FRAME_DELAY; i++) {
    *capture >> frame;

    imshow("reference frame", frame);
    if (cv::waitKey(1) >= 0) {
      break;
    }
  }
  objectDet->setReferenceFrame(frame);
  std::cout << "camera " << camID << ": reference frame set\n" << std::flush;
  cv::destroyWindow("reference frame");

  /*
   * initialize global frame mask
   */
  camParams->initGlobalMask(frame);
}

ImageProcessing::~ImageProcessing() {
  LOG_SCOPE;

  delete capture;
  delete camParams;
  delete objectDet;
  // do not delete messaging, because pointer is held by main!!!
}

void ImageProcessing::evaluate() {
  cv::Mat frame;
  Status status = OK;
  PxPosList pxPositions;
  VectRayList objectRayList;

  /*
   * image processing chain
   */
  *capture >> frame;

  camParams->addGlobalMaskToFrame(frame);

  status = objectDet->detect(frame, pxPositions);

#ifdef DEBUG
  PxPos pixelPos = pxPositions[0];
  if (status != ERR) {
    circle(frame, cv::Point(pixelPos.x, pixelPos.y), 20, cv::Scalar(255, 0, 0), 2);
    circle(frame, cv::Point(pixelPos.x, pixelPos.y), 24, cv::Scalar(0, 0, 0), 2);
  }
  imshow("tracking", frame);
#endif

  for (PxPos pixel : pxPositions) {
    PxPos undistorted;
    VectRay objectRay;

    camParams->undistort(pixel, undistorted);
    camParams->calcObjRay(undistorted, objectRay);

    objectRayList.push_back(objectRay);
  }

  /*
   * send data
   */
  IntraSysMsg message{cameraID, objectRayList, status};
  internalCom->send(message);
}
