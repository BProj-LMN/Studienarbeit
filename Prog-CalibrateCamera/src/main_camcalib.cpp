/*
 * camera based 3D tracking system -- Camera Calibration (distortion)
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

#include <iostream>
#include <iomanip>

#include "Camera.h"
#include "calibrateCamera.h"
#include "myGlobalConstants.h"

void printHelp() {
  std::cout << "\n" << "valid options: loadConfig, calibrateCamera, save, exit" << std::endl;
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
      cameraID = 0; // TODO string to int
      if (!(cameraID >= 0 && cameraID < 10)) {
        std::cout << "Sorry, camera ID is not valid --> terminating\n" << std::flush;
        return (0);
      }
      path = argv[2];

      if (argc > 3) {
        options = argv[3];
      }

    } else {
      std::cout << "Hi, here’s the camera calibration.\n";

      std::cout << "Please give camera ID (beginning at 0):\n" << std::flush;
      std::cin >> cameraID;

      std::cout << "Please give path to camera settings file:\n" << std::flush;
      std::cin >> path;

      printHelp();
      std::cin >> options;
    }

    Camera cam(cameraID);
    Mat frame;

    while (1) {
      if (0 == options.compare("loadConfig")) {
        std::cout << "--> do loadConfig subroutine" << std::endl;
        cam.readSettings(path);

      } else if (0 == options.compare("calibrateCamera")) {
        std::cout << "--> do calibrateCamera subroutine" << std::endl;
        calibrateCameras(&cam);

      } else if (0 == options.compare("save")) {
        std::cout << "--> save camera object parameters" << std::endl;
        cam.saveSettings(path);

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
