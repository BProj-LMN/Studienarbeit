/*
 * Camera.cpp
 *
 * function: store all information of one camera
 *           save/read all parameters to/from file
 *
 * author: Jannik Beyerstedt
 */

#include "Camera.h"

//#include "DataFormats.h"
#include "myGlobalConstants.h"
//#include "Logger.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <time.h>
#include <iostream>
#include <iomanip>

Camera::Camera(std::string configFile) {
  globalMaskSet = 0;
  parseConfig(configFile);
}

int Camera::undistort(PxPos src, PxPos dst) {
  // TODO Task: call undistortPoints()
  //undistortPoints(src, dst, *cameraMatrix, *distCoeffs);

  dst = src; // TODO it’s only a stub

  return ERR;
}

int Camera::parseConfig(std::string configFile) {
  cv::FileStorage fs(configFile, cv::FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    fprintf(stderr, "ERROR: Camera::readSettings - opening file \n");
    return ERR;
  }

  fs["cameraMatrix"] >> cameraMatrix;
  fs["distCoeffs"] >> distCoeffs;
  fs["globalMask"] >> globalMask_Rect;
  fs["positionVector"] >> positionVector;
  fs["rotationMatrix"] >> rotationMatrix;
  fs.release();                                    // close Settings file

  return OK;
}

void Camera::initGlobalMask(cv::Mat& frame) {
  if (globalMask_Rect.area() > 0) {
    this->globalMask = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
    this->globalMask(globalMask_Rect) = 255;
    globalMaskSet = 1;
  } else {
    std::cout << "[ERROR] Camera::initGlobalMask - area of globalMask_Rect is 0" << std::endl;
  }
}

void Camera::addGlobalMaskToFrame(cv::Mat& frame) {
  if (globalMaskSet) {
    frame = frame & globalMask;
  }
}

/*
 * calculates objectRay for use in triangulation from a sensor pixelPosition
 */
int Camera::calcObjRay(PxPos pixelPos, VectRay objectRay) {
  cv::Point2f pixelPosition{pixelPos.x, pixelPos.y};
  Pos3D direction;

  // calculate object ray in world coordinates from pixel position
  cv::Point3f ray;
  if (ERR == calcObjectRayInCameraCoordinates(pixelPosition, ray)) {
    return ERR;
  }
  cv::Mat objectRayCameraCoord = (cv::Mat_<float>(3, 1) << ray.x, ray.y, ray.z);

  // transform vector to world coordinates by multiplying rotationMatrix in front of the vector
  cv::Mat objectRay_Mat = rotationMatrix * objectRayCameraCoord;

  direction.x = objectRay_Mat.at<float>(0, 0);
  direction.y = objectRay_Mat.at<float>(1, 0);
  direction.z = objectRay_Mat.at<float>(2, 0);

  objectRay.dir = direction;
  objectRay.pos = positionVector;
  return OK;
}

/*
 * calculates object ray from pixel value on the sensor. Vector is in camera coordinate system
 *
 * pixelPosition.x <=> u , pixelPosition.y <=> v
 */
int Camera::calcObjectRayInCameraCoordinates(cv::Point2f pixelPosition, cv::Point3f& objectRay) {
  float c_x = cameraMatrix.at<double>(0, 2);
  float c_y = cameraMatrix.at<double>(1, 2);
  float f = cameraMatrix.at<double>(0, 0);

  objectRay.y = pixelPosition.x - c_x; // u --> Y
  objectRay.z = pixelPosition.y - c_y; // v --> Z
  objectRay.x = f;                     // w --> X

  return OK;
}

