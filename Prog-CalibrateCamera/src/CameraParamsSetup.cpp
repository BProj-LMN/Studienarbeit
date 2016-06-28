/*
 * CameraParamsSetup.cpp
 *
 * function: store all information of one camera
 *           save/read all parameters to/from file
 *
 * author: Jannik Beyerstedt
 */

#include "CameraParamsSetup.h"

#include "Logger.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <time.h>
#include <iostream>
#include <iomanip>

CameraParamsSetup::CameraParamsSetup() {
  LOG_SCOPE;
}

void CameraParamsSetup::readConfig(std::string configFile) {
  cv::FileStorage fs(configFile, cv::FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    LOG_ERROR << "CameraParamsSetup::readConfig - settings file could not be opened\n";
    throw Error("camConfig settings file could not be opened");
  }

  fs["cameraMatrix"] >> cameraMatrix;
  fs["distCoeffs"] >> distCoeffs;
  fs["globalMask"] >> globalMask_Rect;
  fs["positionVector"] >> positionVector;
  fs["rotationMatrix"] >> rotationMatrix;
  fs["viewingCenter"] >> viewingCenter;
  fs["viewingRight"] >> viewingRight;
  fs.release();                                    // close Settings file
}

void CameraParamsSetup::saveConfig(std::string configFile) {
  cv::FileStorage fs(configFile, cv::FileStorage::WRITE); // Read the settings
  if (!fs.isOpened()) {
    LOG_ERROR << "CameraParamsSetup::saveConfig - settings file could not be opened\n";
    throw Error("camConfig settings file could not be opened");
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
  fs << "globalMask" << globalMask_Rect;
  fs << "positionVector" << positionVector;
  fs << "rotationMatrix" << rotationMatrix;
  fs << "viewingCenter" << viewingCenter;
  fs << "viewingRight" << viewingRight;
  fs.release();                                    // close Settings file
}

Status CameraParamsSetup::setupRotationMatrix() {
  /*
   * calculates euler rotation angles
   * and calculate rotation matrix for all rotations at once
   */

  // some variables
  cv::Mat Xaxis = (cv::Mat_<float>(3, 1) << 1, 0, 0);
  cv::Mat Yaxis = (cv::Mat_<float>(3, 1) << 0, 1, 0);
  cv::Mat Euler1, Euler2, Euler3;

  cv::Point3f r = viewingCenter - positionVector;
  cv::Point3f s = viewingRight - positionVector;
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

  std::cout << "Camera::setupRotationMatrix\n";
  std::cout << "w1 " << std::fixed << std::setprecision(5) << std::setw(8) << w1 << "\n";
  std::cout << "w2 " << std::fixed << std::setprecision(5) << std::setw(8) << w2 << "\n";
  std::cout << "w3 " << std::fixed << std::setprecision(5) << std::setw(8) << w3 << "\n";
  std::cout << "\n" << std::flush;

  // calculate euler rotation matrix
  this->rotationMatrix = Euler3 * Euler2 * Euler1;

  return Status::OK;
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
void CameraParamsSetup::euler1(float angle, cv::Mat& matrix) {
  /* first euler rotation
   * Matrix = [ [ cos(alpha)  sin(alpha)  0];
   [-sin(alpha)  cos(alpha)  0];
   [ 0           0           1] ];
   */
  double m[3][3] = {{cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1}};
  cv::Mat result = cv::Mat(3, 3, CV_32F, m);
  result.copyTo(matrix);
}
void CameraParamsSetup::euler2(float angle, cv::Mat& matrix) {
  /* second euler rotation
   * Matrix = [ [ cos(alpha)  0          -sin(alpha)];
   [ 0           1           0];
   [ sin(alpha)  0           cos(alpha)] ];
   */
  double m[3][3] = {{cos(angle), 0, -sin(angle)}, {0, 1, 0}, {sin(angle), 0, cos(angle)}};
  cv::Mat result = cv::Mat(3, 3, CV_32F, m);
  result.copyTo(matrix);
}
void CameraParamsSetup::euler3(float angle, cv::Mat& matrix) {
  /* third euler rotation
   * Matrix = [ [ 1           0           0];
   [ 0           cos(alpha)  sin(alpha)];
   [ 0          -sin(alpha)  cos(alpha)] ];
   */
  double m[3][3] = {{1, 0, 0}, {0, cos(angle), sin(angle)}, {0, -sin(angle), cos(angle)}};
  cv::Mat result = cv::Mat(3, 3, CV_32F, m);
  result.copyTo(matrix);
}
float CameraParamsSetup::norm(cv::Mat column_vector) {
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
