/*
 * camera based 3D tracking system -- Main Tracking System
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

//#define SETTINGS "../Progs-configStore/sysConfig.yml"
#define SETTINGS "../Progs-configStore/sysConfig-stub.yml"

#include "DataFormats.h"
#include "Logger.h"

#include "ImageProcessingMngmt.h"
#include "IntraSystemMessaging.h"
#include "ClusterMngmt.h"

#include "IntraDirect.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <iomanip>

int main(int argc, const char** argv) {
  LOG_SET_LEVEL_DEBUG;

  IntraSystemMessaging* messaging = new IntraDirect {};

  ImageProcessingMngmt imgProcManagement {SETTINGS, messaging};
  ClusterMngmt clusterManagement {SETTINGS, messaging};

//    cv::Mat destroyimg = cv::imread("test/destroybild.jpg", 1);   // Read the file
//    cv::namedWindow("zum Beenden: press ESC", cv::WINDOW_AUTOSIZE);
//    imshow("zum Beenden: press ESC", destroyimg);

  try {
    while (1) {
      /*
       * calculations
       */
      imgProcManagement.evaluate();
      clusterManagement.evaluate();

      /*
       * live output to std::cout / console
       */
//      std::cout << "x " << (int) objectPos3D.x << "\ty " << (int) objectPos3D.y << "\tz " << (int) objectPos3D.z;
//      std::cout << "\t\t" << "Abstand Triangulation: " << (int) triangulationMinDistance;
//      std::cout << "\t\t" << "Fehlercode: ";
//      printf("0x%2x", positionDataErrorCode);
      std::cout << "\n" << std::flush;

      // TODO: evaluate some window to terminate the application
      if (cv::waitKey(1) >= 0) {
        break;
      }

    }

    /*
     * tidy everything up
     */
    std::cout << "exit\n";
    std::cout << "\n";
    std::cout << "--> shut down program\n";

    return (0);

  } catch (cv::Exception & e) {
    std::cout << e.msg << "\n" << std::flush;
  }

}
