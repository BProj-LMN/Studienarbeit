/*
 * ImageSource.cpp
 *
 * function: wrap cv::VideoCapture for one channel b/w images, manage images form file system
 *
 * author: Jannik Beyerstedt
 */

#include "ImageSource.h"

#include <opencv2/imgproc.hpp>

ImageSource& ImageSource::operator>>(cv::Mat& image) {
  if (imgIsNotSet) {
    static_cast<cv::VideoCapture>(*this).operator>>(img);
    cv::cvtColor(img, img, CV_BGR2GRAY);
    imgIsNotSet = false;
  }

  image = img;

  return *this;
}

