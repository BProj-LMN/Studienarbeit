/*
 * Camera.h
 *
 * function: store all information of one camera
 *           save/read all parameters to/from file
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#include "DataFormats.h"
//#include "Logger.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class Camera {
public:
  Camera(std::string configFile);

  ReturnStatus undistort(const PxPos& src, PxPos& dst);
  ReturnStatus calcObjRay(const PxPos& pixelPos, VectRay& objectRay);

  void initGlobalMask(cv::Mat& frame);
  void addGlobalMaskToFrame(cv::Mat& frame);
  bool globalMaskSet;

private:
  ReturnStatus parseConfig(std::string configFile);

  ReturnStatus calcObjectRayInCameraCoordinates(const cv::Point2f& pixelPosition, cv::Point3f& objectRayCameraCoord);

  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  cv::Rect globalMask_Rect;
  cv::Point3i positionVector;
  cv::Mat rotationMatrix;

  cv::Mat globalMask;
};

#endif /* SRC_CAMERA_H_ */
