/*
 * ObjDetSimple.cpp
 *
 * function: detect object in an image
 *
 * author: Jannik Beyerstedt
 */

#include "ObjDetSimple.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "myGlobalConstants.h"

const static int SENSITIVITY_VALUE = 35;  // threshold for threshold()
const static int BLUR_SIZE = 10;  // for absdiff()

ObjDetSimple::ObjDetSimple(Camera* cam) {
  this->cam = cam;

}

ObjDetSimple::~ObjDetSimple() {

}

int ObjDetSimple::setReferenceFrame(cv::Mat frame) {
  this->refereceFrame = frame;

  return OK;
}
/*
 * input:  grayscale frame
 * output: Point2i with detected position
 */
int ObjDetSimple::detectObject(cv::Mat frame, cv::Point2i& pixelPosition) {
  cv::Mat diffImage, thresholdImage;
  cv::Rect objectBounding = cv::Rect(0, 0, 0, 0);

  // subtract background and create binary mask
  absdiff(refereceFrame, frame, diffImage); // output: grayscale
  threshold(diffImage, thresholdImage, SENSITIVITY_VALUE, 255, cv::THRESH_BINARY); // output: binary
#ifdef SHOW_THESHOLD
  if (cam->get_cameraID() == 1) {
    imshow("cam2 threshold 1", thresholdImage);
  } else {
    imshow("cam1 threshold 1", thresholdImage);
  }
#endif

  // blur and threshold again to get rid of noise
  blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE)); // output: grayscale
  threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, cv::THRESH_BINARY); // output: binary
#ifdef SHOW_THESHOLD
  if (cam->get_cameraID() == 1) {
    imshow("cam2 threshold 2", thresholdImage);
  } else {
    imshow("cam1 threshold 2", thresholdImage);
  }
#endif

  int error = getObjectPosition(thresholdImage, pixelPosition, &objectBounding);

  if (0 == error) {
    return OK;
  } else {
    return ERR;
  }

}

int ObjDetSimple::getObjectPosition(cv::Mat thresImg, cv::Point2i& objectPos, cv::Rect* boundingRectange) {

  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  findContours(thresImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); // retrieves external contours

  // TODO: merge the two nearby contours ! - with erode and dilate !?

  // if contours vector is empty, we have found no objects
  if (contours.size() == 0) {
    return ERR;
  } else {
    // the largest contour is found at the end of the contours vector
    // we will simply assume that the biggest contour is the object we are looking for.
    std::vector<std::vector<cv::Point> > largestContourVec;
    largestContourVec.push_back(contours.at(contours.size() - 1));

    // make a bounding rectangle around the largest contour then find its centroid
    // this will be the object's final estimated position.
    *boundingRectange = boundingRect(largestContourVec.at(0));

    int xpos = boundingRectange->x + boundingRectange->width / 2;
    int ypos = boundingRectange->y + boundingRectange->height / 2;

    objectPos.x = xpos, objectPos.y = ypos;

    return OK;
  }

}
