/*
 * camera based 3D tracking system -- Main Tracking System
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

#define SETTINGS "../Progs-configStore/sysConfig.yml"

#define ERR_RESET         0x00
#define ERR_TRACKING_LOST 0x01
#define ERR_BIG_DISTANCE  0x02
#define DIST_ERR_CAT1     100

//#define DEBUG // show tracking image

#include <iostream>
#include <iomanip>

#include "DataFormats.h"
#include "myGlobalConstants.h"
#include "Logger.h"

#include "ImageProcessingMngmt.h"
#include "IntraSystemMessaging.h"
#include "ClusterMngmt.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/* soon not needed any more */
#include "UdpSocketServer.h"
#include "ObjDetSimple.h"
#include "triangulate.h"
/* end not needed*/

void printHelp() {
  std::cout << "\n" << "zugelassene Optionen: loadConfig, tracking, loadAndTrack, exit" << std::endl;
}

int main(int argc, const char** argv) {
  LOG_SET_LEVEL_DEBUG;

  std::string options;

  ImageProcessingMngmt a {SETTINGS};

  UdpSocketServer remoteInput(1362);
  std::string message;

  char positionData[MESSAGE_LEN];
  positionData[0] = 0xDA;
  char positionDataErrorCode = 0x00;
  int statusTracking1 = OK;
  int statusTracking2 = OK;
  float triangulationMinDistance {};

//  Camera cam1(0);
  cv::Mat frame1;
//  Camera cam2(1);
  cv::Mat frame2;

//  ObjDetSimple detect1;
  cv::Point2i pixelPos1(0, 0);
  cv::Point2f undistPos1(0.0, 0.0);
//  ObjDetSimple detect2;
  cv::Point2i pixelPos2(0, 0);
  cv::Point2f undistPos2(0.0, 0.0);

  cv::Point3f objectPos3D;

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
//        cam1.readSettings(CAM1_FILENAME);
//        cam2.readSettings(CAM2_FILENAME);

      } else if (0 == options.compare("exit")) {
        std::cout << "--> terminating ... Auf Wiedersehen" << std::endl;

        std::cout << "exit\n";

        return (0);

      } else if (0 == options.compare("tracking")) {
        std::cout << "--> do normal operation" << std::endl;
        break;

      } else if (0 == options.compare("loadAndTrack")) {
        std::cout << "--> loading config and track" << std::endl;
//        cam1.readSettings(CAM1_FILENAME);
//        cam2.readSettings(CAM2_FILENAME);
        break;

      } else {
        std::cout << "diese Eingabe ist nicht zugelassen" << std::endl;
      }

      printHelp();
      std::cin >> options;
    }

    /*
     * set reference frame for tracking
     */
    std::cout << "waiting for reference frame...\n" << std::flush;
    cv::namedWindow("reference frame 1", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("reference frame 2", cv::WINDOW_AUTOSIZE);
    for (int i = 0; i < 60; i++) {
//      cam1.get_newFrame(frame1);
//      cam2.get_newFrame(frame2);
      imshow("reference frame 1", frame1);
      imshow("reference frame 2", frame2);

      if (cv::waitKey(30) >= 0) {
        break;
      }
    }
//    detect1.setReferenceFrame(frame1);
//    detect2.setReferenceFrame(frame2);
    std::cout << "reference frame set\n\n" << std::flush;
    cv::destroyWindow("reference frame 1");
    cv::destroyWindow("reference frame 2");

    /*
     * main tracking routine
     */

#ifdef DEBUG
    namedWindow("tracking 1", WINDOW_NORMAL);
    namedWindow("tracking 2", WINDOW_NORMAL);
#else
    cv::Mat destroyimg = cv::imread("test/destroybild.jpg", 1);   // Read the file
    cv::namedWindow("zum Beenden: press ESC", cv::WINDOW_AUTOSIZE);
    imshow("zum Beenden: press ESC", destroyimg);
#endif

    while (1) {
      positionDataErrorCode = ERR_RESET;

      /*
       * evaluate remote input - remote control this software
       */
      remoteInput.evaluate();
      bool newMessage = remoteInput.get_message(message);

      if (newMessage) {
        // shut down, if requested
        if (message.compare("exit")) {
          break;
        }
      }

      /*
       * get frame and track object
       */
//      cam1.get_newFrame(frame1);
//      cam2.get_newFrame(frame2);
//
//      statusTracking1 = detect1.detectObject(frame1, pixelPos1);
//      statusTracking2 = detect2.detectObject(frame2, pixelPos2);
#ifdef DEBUG
      if (statusTracking1 != ERR) {
        circle(frame1, Point(pixelPos1.x, pixelPos1.y), 20, Scalar(255, 0, 0), 2);
        circle(frame1, Point(pixelPos1.x, pixelPos1.y), 24, Scalar(0, 0, 0), 2);
      }
      imshow("tracking 1", frame1);

      if (statusTracking2 != ERR) {
        circle(frame2, Point(pixelPos2.x, pixelPos2.y), 20, Scalar(255, 0, 0), 2);
        circle(frame2, Point(pixelPos2.x, pixelPos2.y), 24, Scalar(0, 0, 0), 2);
      }
      imshow("tracking 2", frame2);
#endif

      /*
       * undistort pixel position
       */
//      cam1.correctDistortion(pixelPos1, undistPos1); // TODO: it’s only a stub
//      cam2.correctDistortion(pixelPos2, undistPos2); // TODO: it’s only a stub
      /*
       * calculate 3D position - triangulate
       */
//      cam1.calcNewObjectRayVector(pixelPos1);
//      cam2.calcNewObjectRayVector(pixelPos2);
//
//      triangulate(cam1.positionVector, cam1.objectVector, cam2.positionVector, cam2.objectVector, objectPos3D, triangulationMinDistance);
      /*
       * send position via UDP socket
       */
      if (statusTracking1 == ERR || statusTracking2 == ERR) {
        positionDataErrorCode |= ERR_TRACKING_LOST;
      }
      if (triangulationMinDistance > DIST_ERR_CAT1) {
        positionDataErrorCode |= ERR_BIG_DISTANCE;
      }

      positionData[1] = ((int) objectPos3D.x >> 8) & 0x000000FF;
      positionData[2] = (int) objectPos3D.x & 0x000000FF;
      positionData[3] = ((int) objectPos3D.y >> 8) & 0x000000FF;
      positionData[4] = (int) objectPos3D.y & 0x000000FF;
      positionData[5] = ((int) objectPos3D.z >> 8) & 0x000000FF;
      positionData[6] = (int) objectPos3D.z & 0x000000FF;
      positionData[7] = positionDataErrorCode;

      remoteInput.sendMessage(positionData, 8);

      /*
       * Ausgabe und Abbruch
       */
      std::cout << "x " << (int) objectPos3D.x << "\ty " << (int) objectPos3D.y << "\tz " << (int) objectPos3D.z;
      std::cout << "\t\t" << "Abstand Triangulation: " << (int) triangulationMinDistance;
      std::cout << "\t\t" << "Fehlercode: ";
      printf("0x%2x", positionDataErrorCode);
      std::cout << "\n" << std::flush;

      if (cv::waitKey(1) >= 0) {
        break;
      }

    }

    /*
     * tidy everything up
     */
    std::cout << "exit\n";
    a.~ImageProcessingMngmt();

    cv::destroyWindow("tracking 1");
    cv::destroyWindow("tracking 2");
    std::cout << "\n";
    std::cout << "--> shut down program\n";
    std::cout << "windows destroyed\n";
    std::cout << "program successful terminated\n" << std::flush;

    return (0);

  } catch (cv::Exception & e) {
    std::cout << e.msg << "\n" << std::flush;
  }

}
