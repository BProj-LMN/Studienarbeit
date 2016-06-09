/*
 * camera based 3D tracking system -- Calibrate Camera Positions
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

//#define CAMERA_CALIB_CIRCLES // show circles for calibration crosses on the wall

#include <iostream>
#include <iomanip>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

#include "Camera.h"
#include "calibrate3D.h"
#include "calibrateFrameMask.h"
#include "myGlobalConstants.h"

void printHelp() {
  std::cout << "\n" << "valid options: loadConfig, setFrameMask, save, exit, check" << std::endl;
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
      std::cout << "Hi, here’s the 3D camera position calibration.\n";

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

      } else if (0 == options.compare("setFrameMask")) {
        std::cout << "--> do calibrateFrameMask subroutine" << std::endl;
        calibrateFrameMask(&cam);

      } else if (0 == options.compare("save")) {
        std::cout << "--> save camera object parameters" << std::endl;
        cam.saveSettings(path);

      } else if (0 == options.compare("exit")) {
        std::cout << "--> terminating ..." << std::endl;
        return (0);

      } else if (0 == options.compare("check")) {
        std::cout << "--> showing old camera calibration markers" << std::endl;
        break;

      } else {
        std::cout << "input not valid" << std::endl;
      }

      printHelp();
      std::cin >> options;
    }

    /*
     #ifdef CAMERA_CALIB_CIRCLES
     std::cout << "Overlay zur Kamerakalibrierung wird angezeigt...\nBeenden mit ESC\n" << std::endl;

     while (1) {
     namedWindow("Kamera 1 mit Overlay", WINDOW_AUTOSIZE);
     namedWindow("Kamera 2 mit Overlay", WINDOW_AUTOSIZE);

     cam1.get_rawFrame(frame1);
     cam2.get_rawFrame(frame2);

     circle(frame1, Point(frame1.cols / 2, frame1.rows / 2), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(frame2.cols / 2, frame2.rows / 2), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(frame1.cols / 2, frame1.rows), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(frame2.cols / 2, frame2.rows), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(frame1.cols, frame1.rows / 2), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(frame2.cols, frame2.rows / 2), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(frame1.cols, frame1.rows), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(frame2.cols, frame2.rows), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(0, frame1.rows), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(0, frame2.rows), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(frame1.cols, 0), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(frame2.cols, 0), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(0, frame1.rows / 2), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(0, frame2.rows / 2), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(frame1.cols / 2, 0), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(frame2.cols / 2, 0), 10, Scalar(0, 255, 0), 1);
     circle(frame1, Point(0, 0), 10, Scalar(0, 255, 0), 1);
     circle(frame2, Point(0, 0), 10, Scalar(0, 255, 0), 1);

     imshow("Kamera 1 mit Pattern", frame);

     if (waitKey(30) >= 0) {
     break;
     }
     }

     destroyWindow("Kamera 1 mit Pattern");
     #endif
     */

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
