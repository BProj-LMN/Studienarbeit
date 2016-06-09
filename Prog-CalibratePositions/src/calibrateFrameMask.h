/*
 * frameMask.h
 *
 * function: set the camera frameMask
 *
 * author: Daniel Friedrich
 */

#include "Camera.h"
using namespace cv;

void frameMaskMouseCallback(int event, int x, int y, int flags, void* userdata) {
  if (event == EVENT_LBUTTONDOWN) {
    //cout << "position (" << x << ", " << y << ")" << endl;
    MauscallbackBekommen = 1;
    PixelX = x;
    PixelY = y;
  }
}

void calibrateFrameMask(Camera* cam) {
  Mat Pixelgesammelt = Mat::zeros(4, 2, CV_32S);
  Mat frame;
  VideoCapture cap = cam->get_capture();
  namedWindow("frameMask calibration", 1);
  for (int i = 0; i < 4; i++) { //für die Eingabe der Pixel aus dem Bild per Klicken
    std::cout << "Bitte Punkt " << i + 1 << " im Bild anklicken (1: links, 2: rechts, 3: oben, 4: unten)" << std::endl;
    MauscallbackBekommen = 0;
    setMouseCallback("frameMask calibration", frameMaskMouseCallback, NULL);
    while (0 == MauscallbackBekommen) {
      cam->get_newFrame(frame);
      imshow("frameMask calibration", frame);
      if (waitKey(30) >= 0) {
        break;
      }
    }
    Pixelgesammelt.at<int>(i, 0) = PixelX;
    Pixelgesammelt.at<int>(i, 1) = PixelY;
  }

  Rect frameMask = Rect(Pixelgesammelt.at<int>(0, 0), Pixelgesammelt.at<int>(2, 1),
                        (Pixelgesammelt.at<int>(1, 0) - Pixelgesammelt.at<int>(0, 0)),
                        (Pixelgesammelt.at<int>(3, 1) - Pixelgesammelt.at<int>(2, 1)));

  std::cout << "your frameMask is: " << frameMask << std::endl;
  cam->set_frameMask(frameMask);

  destroyWindow("frameMask calibration");

  std::cout << "frameMask for Camera " << cam->get_cameraID() << " set" << std::endl;
}
