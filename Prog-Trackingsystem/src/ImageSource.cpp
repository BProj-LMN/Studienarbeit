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
  if (this->get(cv::CAP_PROP_POS_FRAMES) >= this->get(cv::CAP_PROP_FRAME_COUNT)) {
    this->set(cv::CAP_PROP_POS_FRAMES, 0);
  }
  static_cast<cv::VideoCapture>(*this).operator>>(image);

  cv::cvtColor(image, image, CV_BGR2GRAY);

  return *this;
}

