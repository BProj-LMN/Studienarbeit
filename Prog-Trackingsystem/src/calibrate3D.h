/*
 * calibrate3D.h
 *
 * function: calibrate position of cameras in world coordinates
 *
 * author: Daniel Friedrich
 */
/*
 Zettelwerte 1 - 15 X Y Z (X1 Y1 Z1 X2 Y2 Z2 X3 Y3 Z3 X.....)
 x y  z   x y   z   x y   z   x y  z   x y   z   x y   z   x y  z
 0 37 248 0 151 232 0 287 226 0 63 155 0 170 171 0 254 149 0 27 78
 x y   z  x y   z  x  y z   x   y z   x   y z  x   y z  x   y z   x   y z
 0 153 37 0 308 46 68 0 193 177 0 121 371 0 80 100 0 28 209 0 250 344 0 212

 Klötzchenwerte 1 - 5 X Y Z (X1 Y1 Z1 X2 Y2 Z2 X3 Y3 Z3 X.....)
 x   y   z   x   y   z   x   y   z  x   y   z   x   y   z
 605 233 175 465 112 168 364 112 81 225 233 115 134 112 131
 */

#ifndef SRC_CALIBRATE3D_H_
#define SRC_CALIBRATE3D_H_

#define PUNKTE 20
#define MY_FILENAME "calibrate3D-"

#include <fstream>
#include <string>

#include <opencv2/calib3d/calib3d.hpp>
using namespace cv;

#include "Camera.h"
#include "calibrate3D_Helpers.h"
#include "myGlobalConstants.h"

int PixelX, PixelY;
bool MauscallbackBekommen = 0;

void myMouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
  if (event == EVENT_LBUTTONDOWN) {
    cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    MauscallbackBekommen = 1;
    PixelX = x;
    PixelY = y;
  }
}

void calibrate3Deinzeln(Camera* cam, Mat Pixelmatrix, Mat& PunktematrixXYZ) {
  Mat Kalliframe;
  int KamNr = cam->get_cameraID();
  VideoCapture cap = cam->get_capture();
  namedWindow("Kallibild", 1);
  for (int i = 0; i < PUNKTE; i++) { //für die Eingabe der Kalibrierpixel aus dem Bild per Klicken
    cout << "Bitte Punkt " << i + 1 << " im Bild anklicken" << endl;
    MauscallbackBekommen = 0;
    setMouseCallback("Kallibild", myMouseCallBackFunc, NULL);
    while (0 == MauscallbackBekommen) {

      cam->get_newFrame(Kalliframe);
      imshow("Kallibild", Kalliframe);
      if (waitKey(30) >= 0)
        break;
    }
    Pixelmatrix.at<int>(i, 0) = PixelX;
    Pixelmatrix.at<int>(i, 1) = PixelY;
  }
  speichern(KamNr, Pixelmatrix, PunktematrixXYZ);
  cout << "Kamera" << KamNr << "done" << endl;
  destroyWindow("Kallibild");
}

void calibrate3D(Camera* cam1, Camera* cam2) {
  if (cam1->intrinsicParamsLoaded == 1 && cam2->intrinsicParamsLoaded == 1) {
    char Antwortvariable = 0;
    Mat PunktematrixXYZ = Mat::zeros(PUNKTE, 3, CV_32F); //rows, cols, type
    Mat Pixelmatrix1 = Mat::zeros(PUNKTE, 2, CV_32S); //rows, cols, type - 4 für unsigned int 32 bit (CV_32S)
    Mat Pixelmatrix2 = Mat::zeros(PUNKTE, 2, CV_32S); //rows, cols, type - 4 für unsigned int 32 bit (CV_32S)

    cout << "Wollen Sie neue Kalibrierungspunkte eingeben oder abgespeicherte verwendnen? (n - neu; g - gespeichert)"
         << endl;
    cin >> Antwortvariable;
    if ('g' == Antwortvariable) { //gespeicherte Werte verwenden
      lesen(cam1, Pixelmatrix1, PunktematrixXYZ);
      lesen(cam2, Pixelmatrix2, PunktematrixXYZ);
      cout << "PunktematrixXYZ: " << endl << PunktematrixXYZ << endl;
      cout << "Pixelmatrix1: " << endl << Pixelmatrix1 << endl;
      cout << "Pixelmatrix2: " << endl << Pixelmatrix2 << endl;
      cout << "Wollen Sie auch Weltkoordinaten einlesen oder neue eintippen? (n - neu; g - gespeichert)" << endl;
      cin >> Antwortvariable;
      if ('n' == Antwortvariable) {
        calibrate3Deinzeln(cam1, Pixelmatrix1, PunktematrixXYZ);
        calibrate3Deinzeln(cam2, Pixelmatrix2, PunktematrixXYZ);
      }
    } else {

      for (int i = 0; i < PUNKTE; i++) { //für die Eingabe der Kalibrierpunkte
        cout << "Punkt " << i + 1 << "; Koordinate X: " << endl;
        cin >> PunktematrixXYZ.at<int>(i, 0);
        cout << "Punkt " << i + 1 << "; Koordinate Y: " << endl;
        cin >> PunktematrixXYZ.at<int>(i, 1);
        cout << "Punkt " << i + 1 << "; Koordinate Z: " << endl;
        cin >> PunktematrixXYZ.at<int>(i, 2);
      }

      for (int i = 0; i < PUNKTE; i++) { //für die Ausgabe/Bestaetigung der Eingabe
        cout << "Punkt " << i + 1 << "; Koordinate X = " << PunktematrixXYZ.at<int>(i, 0) << endl;
        cout << "Punkt " << i + 1 << "; Koordinate Y = " << PunktematrixXYZ.at<int>(i, 1) << endl;
        cout << "Punkt " << i + 1 << "; Koordinate Z = " << PunktematrixXYZ.at<int>(i, 2) << endl;
      }
      calibrate3Deinzeln(cam1, Pixelmatrix1, PunktematrixXYZ);
      calibrate3Deinzeln(cam2, Pixelmatrix2, PunktematrixXYZ);
    }
    gleichungRechnenEinzeln(cam1, Pixelmatrix1, PunktematrixXYZ);
    gleichungRechnenEinzeln(cam2, Pixelmatrix2, PunktematrixXYZ);
    cout << "Kalibrierung beendet" << endl;
  } else {
    cout
        << "Kalibrierung konnte nicht abgeschlossen werden \nBitte erst Funktion \"calibrateCamera\" oder \"loadConfig\" aufrufen"
        << endl;
  }
}

#endif /* SRC_CALIBRATE3D_H_ */
