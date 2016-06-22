/*
 * camera based 3D tracking system -- Main Tracking System
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

//#define REMOTE_ONLY  // switch on to disable possibility to exit application by key press locally

#define SETTINGS "../Progs-configStore/sysConfig.yml"
//#define SETTINGS "../Progs-configStore/sysConfig-stub.yml"

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

  try {
    std::cout << "--- tracking system application start ---\n\n" << std::flush;

    IntraSystemMessaging* messaging = new IntraDirect {};

    ImageProcessingMngmt imgProcManagement {SETTINGS, messaging};
    ClusterMngmt clusterManagement {SETTINGS, messaging};

#ifndef REMOTE_ONLY
    // display some image, so that an keypress can be detected
    cv::Mat uiPlaceholderIcon = cv::imread("cog-icon.jpg", 1);
    cv::namedWindow("press ESC to exit application", cv::WINDOW_AUTOSIZE);
    imshow("press ESC to exit application", uiPlaceholderIcon);
#endif

    std::cout << "--- setup ok, doing tracking loop ---\n\n" << std::flush;
    LOG_ERROR << "setup done\n";

    while (1) {
      /*
       * calculations
       */
      imgProcManagement.evaluate();
      clusterManagement.evaluate();

#ifndef REMOTE_ONLY
      // evaluate key events from window to terminate the application
      if (cv::waitKey(1) == 27) { // ESC key is number 27
        break;
      }
#endif

      std::cout << std::flush;
    }

    /*
     * tidy everything up
     */
    std::cout << "--> shut down program\n";
    LOG_ERROR << "normal program shutdown\n";
    return (0);

  } catch (cv::Exception& e) {
    std::cout << "---- opencv exception caught ----\n";
    LOG_ERROR << "catch cv::Exception\n";
    std::cout << e.msg << "\n" << std::flush;

  } catch (Error& e) {
    std::cout << "---- Error caught ----\n";
    LOG_ERROR << "catch Error\n";
    std::cout << e << "\n" << std::flush;;
  }

}
