/*
 * camera based 3D tracking system -- Calibrate Camera Positions
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

#define CAM1_FILENAME "../Progs-configStore/cameraStorage1.xml"
#define CAM2_FILENAME "../Progs-configStore/cameraStorage2.xml"

#define CAMERA_CALIB_CIRCLES // show circles for calibration crosses on the wall

#include <iostream>
#include <iomanip>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace cv;

#include "Camera.h"
#include "calibrate3D.h"
#include "calibrateFrameMask.h"
#include "myGlobalConstants.h"

void printHelp() {
  std::cout << "\n" << "zugelassene Optionen: loadConfig, setFrameMask1, setFrameMask2, save, save&exit, exit" << std::endl;
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

      } else if (0 == options.compare("setFrameMask1")) {
        std::cout << "--> do calibrateFrameMask subroutine" << std::endl;
        calibrateFrameMask(&cam1);

      } else if (0 == options.compare("setFrameMask2")) {
        std::cout << "--> do calibrateFrameMask subroutine" << std::endl;
        calibrateFrameMask(&cam2);

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

      imshow("Kamera 1 mit Pattern", frame1);
      imshow("Kamera 2 mit Pattern", frame2);

      if (waitKey(30) >= 0) {
        break;
      }
    }

    destroyWindow("Kamera 1 mit Pattern");
    destroyWindow("Kamera 2 mit Pattern");
#endif


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
