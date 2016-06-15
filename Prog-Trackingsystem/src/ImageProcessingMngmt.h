/*
 * ImageProcessingMngmt.h
 *
 * function: manage all ImageProcessing entities
 *           loads sysConfig file and sets everything up
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_IMAGEPROCESSINGMNGMT_H_
#define SRC_IMAGEPROCESSINGMNGMT_H_

#include "ImageProcessing.h"

#include <opencv2/core.hpp>

#include <string>
#include <vector>

class CameraProperties;

class ImageProcessingMngmt {
public:
  ImageProcessingMngmt(std::string configFile);
  ~ImageProcessingMngmt();

  void evaluate();

private:
  void parseConfigAndFactory(CameraProperties camProps);

private:
  std::vector<ImageProcessing*> cameras;
};

class CameraProperties {
public:
  int cameraID;
  std::string videoSrc;
  std::string configFile;

public:
  CameraProperties(int id, std::string src, std::string config)
      : cameraID(id), videoSrc(src), configFile(config) {
  }

  friend std::ostream& operator<<(std::ostream& os, const CameraProperties& obj) {
    os << "{" << "cameraID: " << obj.cameraID << ", videoSrc: " << obj.videoSrc << ", configFile: " << obj.configFile << "}";
    return os;
  }

  // methods needed for opencv persistence
  CameraProperties() {
  }
  void write(cv::FileStorage& fs) const {
    fs << "{" << "cameraID" << cameraID << "videoSrc" << videoSrc << "configFile" << configFile << "}";
  }
  void read(const cv::FileNode& node) {
    cameraID = (int) node["cameraID"];
    videoSrc = (std::string) node["videoSrc"];
    configFile = (std::string) node["configFile"];
  }
};

// these two functions are needed for opencv persistence.
static void write(cv::FileStorage& fs, const std::string&, const CameraProperties& x) {
  x.write(fs);
}
static void read(const cv::FileNode& node, CameraProperties& x, const CameraProperties& default_value = CameraProperties()) {
  if (node.empty()) {
    x = default_value;
  } else {
    x.read(node);
  }
}

#endif /* SRC_IMAGEPROCESSINGMNGMT_H_ */
