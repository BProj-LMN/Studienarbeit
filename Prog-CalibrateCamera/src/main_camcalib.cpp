/*
 * camera based 3D tracking system -- Camera Calibration (distortion)
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

#include "DataFormats.h"
//#include "Logger.h"

#include "calibrateCamera.h"
#include "CameraParamsSetup.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

void printHelp() {
  std::cout << "\n" << "valid options: calibrateCamera, save, exit" << std::endl;
}

int main(int argc, const char** argv) {
  std::string options;
  std::string path;
  int cameraID;

  try {

    /*
     * start directly to one mode by program argument or get input by user
     */
    if (argc > 1) {
      // split camera ID followed by a whitespace and the path
      std::string id = argv[1];
      std::stringstream temp(id);
      temp >> cameraID;
      if (!(cameraID >= 0 && cameraID < 10)) {
        std::cout << "Sorry, camera ID is not valid --> terminating\n" << std::flush;
        return (0);
      }
      path = argv[2];

      if (argc > 3) {
        options = argv[3];
      } else {
        options = "calibrateCamera";
      }

    } else {
      std::cout << "Hi, here’s the camera calibration.\n";

      std::cout << "Please give opencv camera ID (beginning at 0):\n" << std::flush;
      std::cin >> cameraID;

      std::cout << "Please give path to camera settings file:\n" << std::flush;
      std::cin >> path;

      printHelp();
      std::cin >> options;
    }

    /*
     * construct CameraParamsExt and VideoSource objects
     */
    CameraParamsSetup cam{};
    cam.readConfig(path);
    cv::VideoCapture* cap = new cv::VideoCapture(cameraID);
    Mat frame;

    while (1) {
      if (0 == options.compare("calibrateCamera")) {
        std::cout << "--> do calibrateCamera subroutine" << std::endl;
        calibrateCamera(&cam, cap);

      } else if (0 == options.compare("save")) {
        std::cout << "--> save camera object parameters" << std::endl;
        cam.saveConfig(path);

      } else if (0 == options.compare("exit")) {
        std::cout << "--> terminating ..." << std::endl;
        return (0);

      } else {
        std::cout << "input not valid" << std::endl;
      }

      printHelp();
      std::cin >> options;
    }


    /*
     * tidy everything up
     */
    std::cout << "\n";
    std::cout << "--> shut down program\n";
    std::cout << "program successful terminated\n" << std::flush;

    return (0);

  } catch (cv::Exception & e) {
    std::cout << e.msg << "\n" << std::flush;
  }

}
