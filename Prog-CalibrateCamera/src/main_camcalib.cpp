/*
 * camera based 3D tracking system -- Camera Calibration (distortion)
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

#define CAM1_FILENAME "../Progs-configStore/cameraStorage1.xml"
#define CAM2_FILENAME "../Progs-configStore/cameraStorage2.xml"

#include <iostream>
#include <iomanip>


#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace cv;

#include "Camera.h"
#include "calibrateCamera.h"
#include "myGlobalConstants.h"

void printHelp() {
  std::cout << "\n" << "zugelassene Optionen: loadConfig, calibrateCamera, save, save&exit, exit" << std::endl;
}

int main(int argc, const char** argv) {
  std::string options;

  Camera cam1(0);
  Mat frame1;
  Camera cam2(1);
  Mat frame2;

  try {

    /*
     * start directly to one mode by program argument or get input by user
     */
    if (argc > 1) {
      options = argv[1];
    } else {
      std::cout << "Guten Tag, hier ist das Tracking-System. Was wollen Sie?" << std::endl;
      printHelp();
      std::cin >> options;
    }

    while (1) {
      if (0 == options.compare("loadConfig")) {
        std::cout << "--> do loadConfig subroutine" << std::endl;
        cam1.readSettings(CAM1_FILENAME);
        cam2.readSettings(CAM2_FILENAME);

      } else if (0 == options.compare("calibrateCamera")) {
        std::cout << "--> do calibrateCamera subroutine" << std::endl;
        calibrateCameras(&cam1, &cam2);

      } else if (0 == options.compare("save")) {
        std::cout << "--> save camera object parameters" << std::endl;
        cam1.saveSettings(CAM1_FILENAME);
        cam2.saveSettings(CAM2_FILENAME);

      } else if (0 == options.compare("exit")) {
        std::cout << "--> terminating ... Auf Wiedersehen" << std::endl;
        return (0);

      } else if (0 == options.compare("save&exit")) {
        std::cout << "--> saving and terminating ... Auf Wiedersehen" << std::endl;
        cam1.saveSettings(CAM1_FILENAME);
        cam2.saveSettings(CAM2_FILENAME);
        return (0);

      } else {
        std::cout << "diese Eingabe ist nicht zugelassen" << std::endl;
      }

      printHelp();
      std::cin >> options;
    }


    /*
     * main tracking routine
     */

    while (1) {

      if (waitKey(1) >= 0) {
        break;
      }

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
