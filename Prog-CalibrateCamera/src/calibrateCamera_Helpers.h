/*
 * calibrateCameraHelpers.h
 *
 * function: classes and functions for calibrateCamera module
 *
 * author: Jannik Beyerstedt
 * modified from: http://docs.opencv.org/2.4/doc/tutorials/calib3d/camera_calibration/camera_calibration.html
 */

#ifndef SRC_CALIBRATECAMERA_HELPERS_H_
#define SRC_CALIBRATECAMERA_HELPERS_H_

#include "DataFormats.h"
//#include "Logger.h"

#include "calibrateCamera_Settings.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

static void read(const FileNode& node, DistCalibSettings& x, const DistCalibSettings& default_value =
                     DistCalibSettings()) {
  if (node.empty()) {
    x = default_value;
  } else {
    x.read(node);
  }
}

static double computeReprojectionErrors(const std::vector<std::vector<Point3f> >& objectPoints,
                                        const std::vector<std::vector<Point2f> >& imagePoints, const std::vector<Mat>& rvecs,
                                        const std::vector<Mat>& tvecs, const Mat& cameraMatrix, const Mat& distCoeffs,
                                        std::vector<float>& perViewErrors) {
  std::vector<Point2f> imagePoints2;
  int i, totalPoints = 0;
  double totalErr = 0, err;
  perViewErrors.resize(objectPoints.size());

  for (i = 0; i < (int) objectPoints.size(); ++i) {
    projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
    err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);

    int n = (int) objectPoints[i].size();
    perViewErrors[i] = (float) std::sqrt(err * err / n);
    totalErr += err * err;
    totalPoints += n;
  }

  return std::sqrt(totalErr / totalPoints);
}

static void calcBoardCornerPositions(Size boardSize, float squareSize, std::vector<Point3f>& corners,
                                     DistCalibSettings::Pattern patternType /*= Settings::CHESSBOARD*/) {
  corners.clear();

  switch (patternType) {
    case DistCalibSettings::CHESSBOARD:
    case DistCalibSettings::CIRCLES_GRID:
      for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width; ++j) {
          corners.push_back(Point3f(float(j * squareSize), float(i * squareSize), 0));
        }
      }
      break;

    case DistCalibSettings::ASYMMETRIC_CIRCLES_GRID:
      for (int i = 0; i < boardSize.height; i++) {
        for (int j = 0; j < boardSize.width; j++) {
          corners.push_back(Point3f(float((2 * j + i % 2) * squareSize), float(i * squareSize), 0));
        }
      }
      break;
    default:
      break;
  }
}

bool runCalibration(DistCalibSettings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                    std::vector<std::vector<Point2f> > imagePoints) {
  std::vector<Mat> rvecs, tvecs;
  std::vector<float> reprojErrs;
  double totalAvgErr = 0;

  // --> compute
  cameraMatrix = Mat::eye(3, 3, CV_64F);
  if (s.flag & CV_CALIB_FIX_ASPECT_RATIO) {
    cameraMatrix.at<double>(0, 0) = 1.0;
  }

  distCoeffs = Mat::zeros(8, 1, CV_64F);

  std::vector<std::vector<Point3f> > objectPoints(1);
  calcBoardCornerPositions(s.boardSize, s.squareSize, objectPoints[0], s.calibrationPattern);

  objectPoints.resize(imagePoints.size(), objectPoints[0]);

  //Find intrinsic and extrinsic camera parameters
  double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs,
                               s.flag | CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5);

  std::cout << "Re-projection error reported by calibrateCamera: " << rms << std::endl;

  bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

  totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints, rvecs, tvecs, cameraMatrix, distCoeffs,
                                          reprojErrs);
  // <-- end compute

  std::cout << (ok ? "Calibration succeeded" : "Calibration failed") << ". avg re projection error = " << totalAvgErr
       << std::endl;

  return ok;
}

#endif /* SRC_CALIBRATECAMERA_HELPERS_H_ */
