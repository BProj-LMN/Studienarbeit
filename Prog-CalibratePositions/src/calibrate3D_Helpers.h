/*
 * calibrate3D.h
 *
 * function: calibrate position of cameras in world coordinates
 *
 * author: Daniel Friedrich
 */

#ifndef SRC_CALIBRATE3D_HELPERS_H_
#define SRC_CALIBRATE3D_HELPERS_H_

#define PUNKTE 20
#define MY_FILENAME "calibrate3D-"

#include <fstream>
#include <string>

#include <opencv2/calib3d/calib3d.hpp>
using namespace cv;

#include "Camera.h"
#include "myGlobalConstants.h"

void gleichungRechnenEinzeln(Camera* cam, Mat Pixelmatrix, Mat PunktematrixXYZ) {
  std::vector<Point3f> Weltvektor;
  std::vector<Point2f> Pixelvektor;

  Weltvektor.resize(PUNKTE);
  Pixelvektor.resize(PUNKTE);

  for (int i = 0; i < PUNKTE; i++) {
    Weltvektor[i] = cv::Point3f(PunktematrixXYZ.at<int>(i, 0), PunktematrixXYZ.at<int>(i, 1),
                                PunktematrixXYZ.at<int>(i, 2));
    Pixelvektor[i] = cv::Point2f(Pixelmatrix.at<int>(i, 0), Pixelmatrix.at<int>(i, 1));
  }

  std::cout << "\n gleichungRechnenEinzeln\n";
  std::cout << "Weltvektor" << "\n" << Weltvektor << "\n";
  std::cout << "Pixelvektor" << "\n" << Pixelvektor << "\n";

  solvePnP(Weltvektor, Pixelvektor, cam->cameraMatrix, cam->distCoeffs, cam->rvecs, cam->tvecs, false,
           SOLVEPNP_ITERATIVE);

  std::cout << "tvec" << "\n" << cam->tvecs << "\n";
  std::cout << "rvec" << "\n" << cam->rvecs << "\n" << std::flush;
}

int speichern(int KamNr, Mat Pixelmatrix, Mat& PunktematrixXYZ) {
  std::stringstream pfad;
  pfad << MY_FILENAME << KamNr << ".xml";
  std::string settingsFilename = pfad.str();

  FileStorage fs(settingsFilename, FileStorage::WRITE); // Read the settings
  if (!fs.isOpened()) {
    std::cout << "Could not open the configuration file: \"" << settingsFilename << "\"" << std::endl;
    return -1;
  }

  time_t tm;
  time(&tm);
  struct tm *t2 = localtime(&tm);
  fs << "timestamp" << (int) tm;
  char buf[1024];
  strftime(buf, sizeof(buf) - 1, "%c", t2);
  fs << "datetime" << buf;

  fs << "PunktematrixXYZ" << PunktematrixXYZ;
  fs << "Pixelmatrix" << Pixelmatrix;

  fs.release();                                    // close Settings file

  std::cout << PunktematrixXYZ << "\n";
  std::cout << Pixelmatrix << "\n" << std::flush;

  return 1;
}

int lesen(Camera* cam, Mat Pixelmatrix, Mat& PunktematrixXYZ) {
  int KamNr = cam->get_cameraID();
  std::stringstream pfad;
  pfad << MY_FILENAME << KamNr << ".xml";
  std::string settingsFilename = pfad.str();

  FileStorage fs(settingsFilename, FileStorage::READ); // Read the settings
  if (!fs.isOpened()) {
    std::cout << "Could not open the configuration file: \"" << settingsFilename << "\"" << std::endl;
    return -1;
  }

  fs["PunktematrixXYZ"] >> PunktematrixXYZ;
  fs["Pixelmatrix"] >> Pixelmatrix;

  fs.release();                                    // close Settings file

  return 1;
}

#endif /* SRC_CALIBRATE3D_HELPERS_H_ */
