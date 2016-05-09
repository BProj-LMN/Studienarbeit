/*
 * calibrateCamera.h
 *
 * function: calibrate distCoeffs of camera
 *
 * author: Jannik Beyerstedt
 * modified from: http://docs.opencv.org/2.4/doc/tutorials/calib3d/camera_calibration/camera_calibration.html
 */

#ifndef SRC_CALIBRATECAMERA_H_
#define SRC_CALIBRATECAMERA_H_

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "calibrateCamera_Helpers.h"
#include "calibrateCamera_Settings.h"

#include "myGlobalConstants.h"

enum {
  DETECTION = 0, CAPTURING = 1, CALIBRATED = 2
};

int executeDistCalib(std::string settingsFile, Camera* cam);

int calibrateCameras(Camera* cam) {
  std::cout << "\n\nHello, this is the distortion correction subroutine" << std::endl;
  int returnValue = ERR;

  returnValue = executeDistCalib("calibrateCamera.xml", cam);
  if (ERR == returnValue) {
    return ERR;
  }
  cam->intrinsicParamsLoaded = 1;

  return OK;
}

int executeDistCalib(std::string settingsFile, Camera* cam) {
  DistCalibSettings s;
  FileStorage fs(settingsFile, FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    std::cout << "Could not open the configuration file: \"" << settingsFile << "\"" << std::endl;
    return -1;
  }
  // hard code some settings
  s.inputType = DistCalibSettings::CAMERA;
  VideoCapture cap = cam->get_capture();
  s.inputCapture = cap;
  fs["Settings"] >> s;
  fs.release();                                         // close Settings file

  if (!s.goodInput) {
    std::cout << "Invalid input detected. Application stopping. " << std::endl;
    return ERR;
  }

  std::vector<std::vector<Point2f> > imagePoints;
  Mat* cameraMatrix = &cam->cameraMatrix;
  Mat* distCoeffs = &cam->distCoeffs;

  Size imageSize;
  int mode = DETECTION;
  clock_t prevTimestamp = 0;
  const Scalar RED(0, 0, 255), GREEN(0, 255, 0);
  const char ESC_KEY = 27;

  while (1) {
    Mat view;
    bool blinkOutput = false;

    view = s.nextImage();
    if (view.empty()) {
      return ERR;
    }

    //-----  If no more image, or got enough, then stop calibration and show result -------------
    if (mode == CAPTURING && imagePoints.size() >= (unsigned) s.nrFrames) {
      if (runCalibration(s, imageSize, *cameraMatrix, *distCoeffs, imagePoints)) {
        mode = CALIBRATED;
      } else {
        mode = DETECTION;
      }
    }

    imageSize = view.size();  // Format input image.
    if (s.flipVertical) {
      flip(view, view, 0);
    }

    std::vector<Point2f> pointBuf;

    bool found;
    switch (s.calibrationPattern) { // Find feature points on the input format
      case DistCalibSettings::CHESSBOARD:
        found = findChessboardCorners(view, s.boardSize, pointBuf,
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
        break;
      case DistCalibSettings::CIRCLES_GRID:
        found = findCirclesGrid(view, s.boardSize, pointBuf);
        break;
      case DistCalibSettings::ASYMMETRIC_CIRCLES_GRID:
        found = findCirclesGrid(view, s.boardSize, pointBuf, CALIB_CB_ASYMMETRIC_GRID);
        break;
      default:
        found = false;
        break;
    }

    if (found) {               // If done with success,
      // improve the found corners' coordinate accuracy for chessboard
      if (s.calibrationPattern == DistCalibSettings::CHESSBOARD) {
        Mat viewGray;
        cvtColor(view, viewGray, COLOR_BGR2GRAY);
        cornerSubPix(viewGray, pointBuf, Size(11, 11), Size(-1, -1), TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
      }

      if (mode == CAPTURING &&  // For camera only take new samples after delay time
      (!s.inputCapture.isOpened() || clock() - prevTimestamp > s.delay * 1e-3 * CLOCKS_PER_SEC)) {
        imagePoints.push_back(pointBuf);
        prevTimestamp = clock();
        blinkOutput = s.inputCapture.isOpened();
      }

      // Draw the corners.
      drawChessboardCorners(view, s.boardSize, Mat(pointBuf), found);
    }

    //----------------------------- Output Text ------------------------------------------------
    std::string msg = (mode == CAPTURING) ? "100/100" : mode == CALIBRATED ? "Calibrated, press 'esc' for next" : "Press 'g' to start";
    int baseLine = 0;
    Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
    Point textOrigin(view.cols - 2 * textSize.width - 10, view.rows - 2 * baseLine - 10);

    if (mode == CAPTURING) {
      if (s.showUndistorsed)
        msg = format("%d/%d Undist", (int) imagePoints.size(), s.nrFrames);
      else
        msg = format("%d/%d", (int) imagePoints.size(), s.nrFrames);
    }

    putText(view, msg, textOrigin, 1, 1, mode == CALIBRATED ? GREEN : RED);

    if (blinkOutput) {
      bitwise_not(view, view);
    }

    //------------------------- Video capture  output  undistorted ------------------------------
    if (mode == CALIBRATED && s.showUndistorsed) {
      Mat temp = view.clone();
      undistort(temp, view, *cameraMatrix, *distCoeffs);
    }

    //------------------------------ Show image and check for input commands -------------------
    imshow("distortion calibration", view);
    char key = (char) waitKey(s.inputCapture.isOpened() ? 50 : s.delay);
    if (key == ESC_KEY) {
      destroyWindow("distortion calibration");
      break;
    }
    if (key == 'u' && mode == CALIBRATED) {
      s.showUndistorsed = !s.showUndistorsed;
    }
    if (s.inputCapture.isOpened() && key == 'g') {
      mode = CAPTURING;
      imagePoints.clear();
    }
  }

  return OK;
}

#endif /* SRC_CALIBRATECAMERA_H_ */

