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
#include <opencv2/imgproc.hpp>

class ImageSource: public cv::VideoCapture {
public:
  using cv::VideoCapture::VideoCapture;

  virtual ImageSource& operator>>(cv::Mat& image) {
    if (this->get(cv::CAP_PROP_POS_FRAMES) >= this->get(cv::CAP_PROP_FRAME_COUNT)) {
      this->set(cv::CAP_PROP_POS_FRAMES, 0);
    }
    static_cast<cv::VideoCapture>(*this).operator>>(image);

    cv::cvtColor(image, image, CV_BGR2GRAY);

    return *this;
  }
};

#endif /* SRC_IMAGESOURCE_H_ */
