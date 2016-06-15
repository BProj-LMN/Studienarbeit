/*
 * CameraProperties.h
 *
 * function: class holding the config data for each camera in sysConfig. Needed to read data from opencv persistance (FileStorage)
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_CAMERAPROPERTIES_H_
#define SRC_CAMERAPROPERTIES_H_

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

#endif /* SRC_CAMERAPROPERTIES_H_ */
