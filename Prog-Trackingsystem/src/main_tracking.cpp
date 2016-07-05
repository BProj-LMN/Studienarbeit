/*
 * camera based 3D tracking system -- Main Tracking System
 * Studienarbeit SS 2016 - HAW Hamburg - Jannik Beyerstedt
 *
 * based on:
 * Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung
 * authors: Jannik Beyerstedt, Daniel Friedrich
 */

#define REMOTE_ONLY  // switch on to disable possibility to exit application by key press locally
//#define TESTS // switch unit and other function tests on

#ifndef TESTS
#define SETTINGS "../Progs-configStore/sysConfig-stub.yml"
#else
#define SETTINGS "/test/sysConfig-test.yml"
#endif

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
#include <chrono>

#ifdef TESTS
#include "TestCases.h"
#endif

int main(int argc, char* argv[]) {
#ifdef TESTS
  return boost::unit_test::unit_test_main(init_unit_test, argc, argv);
#endif

  LOG_SET_LEVEL_DEBUG;

  try {
    std::cout << "--- tracking system application start ---\n\n" << std::flush;

    IntraSystemMessaging* messaging = new IntraDirect{};

    ImageProcessingMngmt imgProcManagement{SETTINGS, messaging};
    ClusterMngmt clusterManagement{SETTINGS, messaging};

#ifndef REMOTE_ONLY
    // display some image, so that an keypress can be detected
    cv::Mat uiPlaceholderIcon = cv::imread("cog-icon.jpg", 1);
    cv::namedWindow("press ESC to exit application", cv::WINDOW_AUTOSIZE);
    imshow("press ESC to exit application", uiPlaceholderIcon);
#endif

    std::cout << "--- setup ok, doing tracking loop ---\n\n" << std::flush;
    LOG_ERROR << "setup done\n";

    /* TIME MEASUREMENT START */
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int i = 1; i < 1000; i++) {
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

    /* TIME MEASUREMENT END */
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    std::cout << "duration: " << fp_ms.count() << " ms\n";

    /*
     * tidy everything up
     */
    std::cout << "--> shut down program\n";
    LOG_ERROR << "normal program shutdown\n";
    return EXIT_SUCCESS;

  } catch (cv::Exception& e) {
    std::cerr << "---- opencv exception caught ----\n";
    LOG_ERROR << "catch cv::Exception\n";
    std::cerr << e.msg << "\n" << std::flush;
    return EXIT_FAILURE;

  } catch (Error& e) {
    std::cerr << "---- Error caught ----\n";
    LOG_ERROR << "catch Error\n";
    std::cerr << e << "\n" << std::flush;
    return EXIT_FAILURE;
  }

}
