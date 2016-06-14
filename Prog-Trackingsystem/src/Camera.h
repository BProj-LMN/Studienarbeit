/*
 * camera.h
 *
 * function: store all information of one camera
 *           save/read all parameters to/from file
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#define TEST  // for testing the system without cameras

#define DEFAULT_FILENAME "cameraStorage-"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class Camera {
  cv::VideoCapture capture;
  int cameraID;
  std::string settingsFilename;
  cv::Rect frameMaskRect;
  cv::Mat frameMask;
  bool frameMaskSet;

  cv::Point3i viewingCenter;  // central image point (p) - will be saved and loaded
  cv::Point3i viewingRight;   // second image point  (t) - will be saved and loaded
  cv::Point3f cameraRotation; // 3 euler rotations   (w) - will be computed
  cv::Mat rotationMatrix;     // 3x3 float !!!

public:
  cv::Point3i positionVector; // position            (o) - will be saved and loaded
  cv::Point3i objectVector;   // from calcNewObjectRayVector

  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  cv::Mat rvecs;
  cv::Mat tvecs;
  bool intrinsicParamsLoaded;

public:
  Camera(int cameraIndex);
  Camera(int cameraIndex, std::string settingsFile);
  virtual ~Camera();

  cv::VideoCapture get_capture();
  int correctDistortion(cv::Point2i src, cv::Point2f dst);

  int readSettings(std::string settingsFile); // read from temporarily filename
  int readSettings();                         // read from filename from constructor
  int saveSettings(std::string settingsFile); // save to temporarily filename
  int saveSettings();                         // save to filename from constructor
  int get_cameraID();
  int set_frameMask(cv::Rect frameMask);
  int get_newFrame(cv::Mat& frame);
  int get_rawFrame(cv::Mat& frame);           // get original 3 channel frame, without mask

  // vector from Camera to object in world coordinates,
  // be sure to reload settings after changing intrinsic parameters
  int calcNewObjectRayVector(cv::Point2f pixelPosition);

private:
  int setupRotationMatrix(); // call after changing camera position information - or reload setting
  int calcObjectRayInCameraCoordinates(cv::Point2f pixelPosition, cv::Point3f& objectRayCameraCoord);

  void euler1(float angle, cv::Mat& matrix);
  void euler2(float angle, cv::Mat& matrix);
  void euler3(float angle, cv::Mat& matrix);
  float norm(cv::Mat column_vector);
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

#endif /* SRC_CAMERA_H_ */
