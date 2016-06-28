/*
 * CameraParamsSetup.h
 *
 * function: store all information of one camera
 *           save/read all parameters to/from file
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_CAMERAPARAMSSETUP_H_
#define SRC_CAMERAPARAMSSETUP_H_

#include "DataFormats.h"
//#include "Logger.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class CameraParamsSetup {
public:
  CameraParamsSetup();

  void readConfig(std::string configFile);
  void saveConfig(std::string configFile);
  Status setupRotationMatrix();

  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;

private:
  cv::Rect globalMask_Rect;

  cv::Point3i viewingCenter;  // central image point (p) - will be saved and loaded
  cv::Point3i viewingRight;   // second image point  (t) - will be saved and loaded
  cv::Point3i positionVector; // position            (o) - will be saved and loaded
  cv::Mat rotationMatrix;

  cv::Mat globalMask;

  void euler1(float angle, cv::Mat& matrix);
  void euler2(float angle, cv::Mat& matrix);
  void euler3(float angle, cv::Mat& matrix);
  float norm(cv::Mat column_vector);
};

#endif /* SRC_CAMERAPARAMSSETUP_H_ */
