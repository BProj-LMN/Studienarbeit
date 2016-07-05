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
    imgIsNotSet = false;
  }

  cv::cvtColor(img, image, CV_BGR2GRAY);

  return *this;
}

