/*
 * ImageProcessing.cpp
 *
 * function: all image processing for one camera
 *
 * author: Jannik Beyerstedt
 */

#include "ImageProcessing.h"

#include "DataFormats.h"
//#include "Logger.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//#define DEBUG
#define REFERENCE_FRAME_DELAY 60

ImageProcessing::ImageProcessing(int cameraID, ImageSource* src, Camera* camera, ObjectDetection* objDetection)
    : camID(cameraID), cap(src), cam(camera), objDet(objDetection) {
  std::cout << "ImageProcessing::ctor start\n";

  /*
   * set reference frame for tracking
   */
  cv::Mat frame;

  std::cout << "camera " << cameraID << ": waiting for reference frame...\n" << std::flush;
  cv::namedWindow("reference frame", cv::WINDOW_AUTOSIZE);
  for (int i = 0; i < REFERENCE_FRAME_DELAY; i++) {
    *cap >> frame;

    imshow("reference frame", frame);
    if (cv::waitKey(30) >= 0) {
      break;
    }
  }
  objDet->setReferenceFrame(frame);
  std::cout << "camera " << cameraID << " :reference frame set\n" << std::flush;
  cv::destroyWindow("reference frame");

  /*
   * initialize global frame mask
   */
  cam->initGlobalMask(frame);

}

ImageProcessing::~ImageProcessing() {
  std::cout << "ImageProcessing " << camID << " dtor\n";

  delete cap;
  delete cam;
  delete objDet;
}

void ImageProcessing::evaluate() {
  std::cout << "ImageProcessing " << camID << " evaluate \n";
  cv::Mat frame;
  int status = OK;
  PxPosList pxPositions;
  VectRayList positionsRays;

  /*
   * image processing chain
   */
  *cap >> frame;

  cam->addGlobalMaskToFrame(frame);

  status = objDet->detect(frame, pxPositions);

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

    cam->undistort(pixel, undistorted);
    cam->calcObjRay(undistorted, objectRay);

    positionsRays.push_back(objectRay);
  }

  // TODO: send positions rays to intra system messaging

}
