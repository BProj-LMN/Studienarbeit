/*
 * calibrateCamera_Settings.h
 *
 *  Created on: 15.11.2015
 *      Author: jannik
 */

#ifndef SRC_CALIBRATECAMERA_SETTINGS_H_
#define SRC_CALIBRATECAMERA_SETTINGS_H_

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class DistCalibSettings {
public:
  DistCalibSettings() :
      goodInput(false) {
  }
  enum Pattern {
    NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID
  };
  enum InputType {
    INVALID, CAMERA
  };

  //void write(FileStorage& fs) const;    // Write serialization for this class
  void read(const FileNode& node);      // Read serialization for this class
  void interprate();
  Mat nextImage();

public:
  Size boardSize;               // The size of the board -> Number of items by width and height
  Pattern calibrationPattern;   // One of the Chessboard, circles, or asymmetric circle pattern
  float squareSize;             // The size of a square in your defined unit (point, millimeter,etc).
  int nrFrames;                 // The number of frames to use from the input for calibration
  float aspectRatio;            // The aspect ratio
  int delay;                    // In case of a video input
  bool bwritePoints;            // Write detected feature points
  bool bwriteExtrinsics;        // Write extrinsic parameters
  bool calibZeroTangentDist;    // Assume zero tangential distortion
  bool calibFixPrincipalPoint;  // Fix the principal point at the center
  bool flipVertical;            // Flip the captured images around the horizontal axis
  std::string outputFileName;   // The name of the file where to write
  bool showUndistorsed;         // Show undistorted images after calibration
  std::string input;            // The input ->

  VideoCapture inputCapture;
  InputType inputType;
  bool goodInput;
  int flag;

private:
  std::string patternToUse;

};

#endif /* SRC_CALIBRATECAMERA_SETTINGS_H_ */
