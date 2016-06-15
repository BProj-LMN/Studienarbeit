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
  std::cout << "Camera ctor stub\n";
}

Camera::Camera(int cameraIndex) {
  this->cameraID = cameraIndex;
  intrinsicParamsLoaded = 0;
  frameMaskSet = 0;

#ifndef TEST
  capture = cv::VideoCapture(cameraIndex);
#else
  if (0 == cameraIndex) {
    capture = cv::VideoCapture("test/frameCam1-01.png");
  } else if (1 == cameraIndex) {
    capture = cv::VideoCapture("test/frameCam2-01.png");
  }
#endif
  if (!capture.isOpened()) {
    //return -1;
  }

  // use default filename
  std::stringstream string;
  string << DEFAULT_FILENAME << cameraID;
  settingsFilename = string.str();
}

Camera::Camera(int cameraIndex, std::string settingsFile) {
  this->cameraID = cameraIndex;
  intrinsicParamsLoaded = 0;
  frameMaskSet = 0;

  capture = cv::VideoCapture(cameraIndex);
  if (!capture.isOpened()) {
    //return -1;
  }

  this->settingsFilename = settingsFile;
}

Camera::~Camera() {
  capture.release();
  std::cout << "[INFO]Camera::~Camera - released capture" << std::endl;
}

cv::VideoCapture Camera::get_capture() {
  return capture;
}

int Camera::get_cameraID() {
  return cameraID;
}

int Camera::correctDistortion(cv::Point2i src, cv::Point2f dst) {
  // TODO Task: call undistortPoints()
  //undistortPoints(src, dst, *cameraMatrix, *distCoeffs);

  dst = src; // TODO it’s only a stub

  return ERR;
}

int Camera::readSettings() {
  return readSettings(settingsFilename);
}

int Camera::readSettings(std::string settingsFile) {
  cv::FileStorage fs(settingsFile, cv::FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    fprintf(stderr, "ERROR: Camera::readSettings - opening file \n");
    return ERR;
  }

  fs["frameMaskRect"] >> frameMaskRect;
  fs["cameraMatrix"] >> cameraMatrix;
  fs["distCoeffs"] >> distCoeffs;
  fs["rvecs"] >> rvecs;
  fs["tvecs"] >> tvecs;
  fs["tvecs"] >> tvecs;
  fs["positionVector"] >> positionVector;
  fs["viewingCenter"] >> viewingCenter;
  fs["viewingRight"] >> viewingRight;

  fs.release();                                    // close Settings file
  intrinsicParamsLoaded = 1;
  if (frameMaskRect.area() > 0) {
    set_frameMask(frameMaskRect);
    frameMaskSet = 1;
  }

  setupRotationMatrix();

  return OK;
}

int Camera::saveSettings() {
  return saveSettings(settingsFilename);
}

int Camera::saveSettings(std::string settingsFile) {
  cv::FileStorage fs(settingsFile, cv::FileStorage::WRITE); // Read the settings
  if (!fs.isOpened()) {
    fprintf(stderr, "ERROR: Camera::saveSettings - opening file \n");
    return ERR;
  }

  time_t tm;
  time(&tm);
  struct tm *t2 = localtime(&tm);
  fs << "timestamp" << (int) tm;
  char buf[1024];
  strftime(buf, sizeof(buf) - 1, "%c", t2);
  fs << "datetime" << buf;

  fs << "cameraMatrix" << cameraMatrix;
  fs << "distCoeffs" << distCoeffs;
  fs << "globalMask" << frameMaskRect;
  fs << "positionVector" << positionVector;
  fs << "rotationMatrix" << rotationMatrix;
  fs << "viewingCenter" << viewingCenter;
  fs << "viewingRight" << viewingRight;

  fs.release();                                    // close Settings file
  return OK;
}

int Camera::set_frameMask(cv::Rect frameMask) {
  if (frameMask.area() > 0) {
    cv::Mat frame;
    capture >> frame;
    this->frameMask = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
    this->frameMask(frameMask) = 255;

    this->frameMaskRect = frameMask;

    frameMaskSet = 1;
  } else {
    std::cout << "[ERROR] Camera::set_frameMask - no valid frameMask given in" << std::endl;
  }

  return OK;
}

int Camera::get_newFrame(cv::Mat& frame) {
#ifdef TEST
  if (capture.get(cv::CAP_PROP_POS_FRAMES) >= capture.get(cv::CAP_PROP_FRAME_COUNT)) {
    capture.set(cv::CAP_PROP_POS_FRAMES, 0);
  }
#endif

  capture >> frame;

  cvtColor(frame, frame, CV_BGR2GRAY);

  //vector<Mat> channels;
  //split(frame, channels);
  //frame = channels[0];

  if (frameMaskSet) {
    frame = frame & frameMask;
  }

  return OK;
}

int Camera::get_rawFrame(cv::Mat& frame) {
#ifdef TEST
  if (capture.get(cv::CAP_PROP_POS_FRAMES) >= capture.get(cv::CAP_PROP_FRAME_COUNT)) {
    capture.set(cv::CAP_PROP_POS_FRAMES, 0);
  }
#endif

  capture >> frame;

  return OK;
}

int Camera::setupRotationMatrix() {
  /*
   * calculates euler rotation angles
   * and calculate rotation matrix for all rotations at once
   */

  // some variables
  cv::Mat Xaxis = (cv::Mat_<float>(3, 1) << 1, 0, 0);
  cv::Mat Yaxis = (cv::Mat_<float>(3, 1) << 0, 1, 0);
  cv::Mat Euler1, Euler2, Euler3;

  cv::Point3i r = viewingCenter - positionVector;
  cv::Point3i s = viewingRight - positionVector;
  cv::Mat r_vec = (cv::Mat_<float>(3, 1) << r.x, r.y, r.z);
  cv::Mat s_vec = (cv::Mat_<float>(3, 1) << s.x, s.y, s.z);

  // calculate euler angles
  float w1 = -atan2(r.y, r.x);
  float w2 = -asin(r.z / sqrt(r.x * r.x + r.y * r.y + r.z * r.z));

  euler1(w1, Euler1);
  euler2(w2, Euler2);

  cv::Mat Xaxis_new = Euler2 * Euler1 * Xaxis;
  cv::Mat Yaxis_new = Euler2 * Euler1 * Yaxis;
  cv::Mat plane_reference = Xaxis_new.cross(Yaxis_new);
  cv::Mat plane_camera = s_vec.cross(r_vec);
  float w3 = -acos(plane_reference.dot(plane_camera) / (norm(plane_reference) * norm(plane_camera)));
  euler3(w3, Euler3);

  std::cout << "Camera::setupRotationMatrix of camera " << cameraID << "\n";
  std::cout << "w1 " << std::fixed << std::setprecision(5) << std::setw(8) << w1 << "\n";
  std::cout << "w2 " << std::fixed << std::setprecision(5) << std::setw(8) << w2 << "\n";
  std::cout << "w3 " << std::fixed << std::setprecision(5) << std::setw(8) << w3 << "\n";
  std::cout << "\n" << std::flush;

  // calculate euler rotation matrix
  this->rotationMatrix = Euler3 * Euler2 * Euler1;

  return OK;
}

int Camera::calcNewObjectRayVector(cv::Point2f pixelPosition) {
  /*
   * calculates objectRay for use in triangulation out of a sensor pixelPosition
   *
   * pixelPosition is float instead of int, because there are subpixel values after undistort
   */

  // calculate object ray in world coordinates from pixel position
  cv::Point3f ray;
  if (ERR == calcObjectRayInCameraCoordinates(pixelPosition, ray)) {
    return ERR;
  }
  cv::Mat objectRayCameraCoord = (cv::Mat_<float>(3, 1) << ray.x, ray.y, ray.z);

  // transform vector to world coordinates by multiplying rotationMatrix in front of the vector
  cv::Mat objectRay_Mat = rotationMatrix * objectRayCameraCoord;

  this->objectVector.x = objectRay_Mat.at<float>(0, 0);
  this->objectVector.y = objectRay_Mat.at<float>(1, 0);
  this->objectVector.z = objectRay_Mat.at<float>(2, 0);

  return OK;
}

int Camera::calcObjectRayInCameraCoordinates(cv::Point2f pixelPosition, cv::Point3f& objectRay) {
  /*
   * calculates object ray from pixel value on the sensor. Vector is in camera coordinate system
   *
   * pixelPosition.x <=> u , pixelPosition.y <=> v
   */

  float c_x = cameraMatrix.at<double>(0, 2);
  float c_y = cameraMatrix.at<double>(1, 2);
  float f = cameraMatrix.at<double>(0, 0);

  objectRay.y = pixelPosition.x - c_x; // u --> Y
  objectRay.z = pixelPosition.y - c_y; // v --> Z
  objectRay.x = f;                     // w --> X

  return OK;
}

/*
 * euler rotations for the following scheme
 * rotate Z, then Y', then X''
 *
 * correspondance Camera Coordinate System to World Coordinate System:
 * u,v,w --> X,Y,Z or more precisely X'',Y'',Z''
 * X --> w, Y --> u, Z --> v
 * ==> X-Axis is camera viewing axis !
 * ==> with this correspondance, camera is already upside down
 */
void Camera::euler1(float angle, cv::Mat& matrix) {
  /* first euler rotation
   * Matrix = [ [ cos(alpha)  sin(alpha)  0];
   [-sin(alpha)  cos(alpha)  0];
   [ 0           0           1] ];
   */
  double m[3][3] = { { cos(angle), sin(angle), 0 }, { -sin(angle), cos(angle), 0 }, { 0, 0, 1 } };
  cv::Mat result = cv::Mat(3, 3, CV_32F, m);
  result.copyTo(matrix);
}
void Camera::euler2(float angle, cv::Mat& matrix) {
  /* second euler rotation
   * Matrix = [ [ cos(alpha)  0          -sin(alpha)];
   [ 0           1           0];
   [ sin(alpha)  0           cos(alpha)] ];
   */
  double m[3][3] = { { cos(angle), 0, -sin(angle) }, { 0, 1, 0 }, { sin(angle), 0, cos(angle) } };
  cv::Mat result = cv::Mat(3, 3, CV_32F, m);
  result.copyTo(matrix);
}
void Camera::euler3(float angle, cv::Mat& matrix) {
  /* third euler rotation
   * Matrix = [ [ 1           0           0];
   [ 0           cos(alpha)  sin(alpha)];
   [ 0          -sin(alpha)  cos(alpha)] ];
   */
  double m[3][3] = { { 1, 0, 0 }, { 0, cos(angle), sin(angle) }, { 0, -sin(angle), cos(angle) } };
  cv::Mat result = cv::Mat(3, 3, CV_32F, m);
  result.copyTo(matrix);
}
float Camera::norm(cv::Mat column_vector) {
  float result = 0;

  if (column_vector.rows > 1 && column_vector.cols == 1) {
    for (int i = 0; i < column_vector.rows; i++) {
      result += column_vector.at<float>(i, 0) * column_vector.at<float>(i, 0);
    }
    result = sqrt(result);
  } else {
    fprintf(stderr, "ERROR: in Camera::vecNorm - no column vector given \n");
  }

  return result;
}

