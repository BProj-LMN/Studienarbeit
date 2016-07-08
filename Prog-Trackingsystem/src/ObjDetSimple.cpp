/*
 * ObjDetSimple.cpp
 *
 * function: detect object in an image
 *
 * author: Jannik Beyerstedt
 */

#include "ObjDetSimple.h"

#include "DataFormats.h"
#include "Logger.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const static int SENSITIVITY_VALUE = 35;  // threshold for threshold()
const static int BLUR_SIZE = 10;  // for absdiff()

void ObjDetSimple::setReferenceFrame(cv::Mat frame) {
  this->referenceFrame = frame;
}
/*
 * input:  grayscale frame
 * output: Point2i with detected position
 */
Status ObjDetSimple::detect(cv::Mat frame, PxPosList& positions) {
  cv::Mat diffImage, thresholdImage;
  cv::Rect objectBounding = cv::Rect(0, 0, 0, 0);
  cv::Point2i pixelPosition;

  // subtract background and create binary mask
  absdiff(referenceFrame, frame, diffImage); // output: grayscale
  threshold(diffImage, thresholdImage, SENSITIVITY_VALUE, 255, cv::THRESH_BINARY); // output: binary
#ifdef SHOW_THESHOLD
  imshow("cam threshold 1", thresholdImage);
#endif

  // blur and threshold again to get rid of noise
  blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE)); // output: grayscale
  threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, cv::THRESH_BINARY); // output: binary
#ifdef SHOW_THESHOLD
  imshow("cam threshold 2", thresholdImage);
#endif

  int error = getObjectPosition(thresholdImage, pixelPosition, &objectBounding);

  if (0 == error) {
    PxPos convert{float(pixelPosition.x), float(pixelPosition.y)};
    positions.push_back(convert);

    return OK;
  } else {
    return ERR;
  }

}

Status ObjDetSimple::getObjectPosition(cv::Mat& binaryImg, cv::Point2i& objectPos, cv::Rect* boundingRectange) {

  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  findContours(binaryImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); // retrieves external contours

  // TODO-enh: merge the two nearby contours ! - with erode and dilate !?

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
