/*
 * ImageSource.h
 *
 * function: wrap cv::VideoCapture for one channel b/w images, manage images form file system
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_IMAGESOURCE_H_
#define SRC_IMAGESOURCE_H_

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class ImageSource: public cv::VideoCapture {
public:
  using cv::VideoCapture::VideoCapture;

  ImageSource& operator>>(cv::Mat& image);
};

#endif /* SRC_IMAGESOURCE_H_ */
