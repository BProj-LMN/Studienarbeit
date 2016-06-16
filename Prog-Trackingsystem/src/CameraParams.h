/*
 * Camera.h
 *
 * function: store all information of one camera
 *           save/read all parameters to/from file
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_CAMERAPARAMS_H_
#define SRC_CAMERAPARAMS_H_

#include "DataFormats.h"
//#include "Logger.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class CameraParams {
public:
  CameraParams(std::string configFile);

  Status undistort(const PxPos& src, PxPos& dst);
  Status calcObjRay(const PxPos& pos, VectRay& objectRay);

  void initGlobalMask(cv::Mat& frame);
  void addGlobalMaskToFrame(cv::Mat& frame);
  bool globalMaskSet;

private:
  Status parseConfig(std::string configFile);

  Status calcObjectRayInCameraCoordinates(const cv::Point2f& pixelPosition, cv::Point3f& objectRayCameraCoord);

  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  cv::Rect globalMask_Rect;
  cv::Point3i positionVector;
  cv::Mat rotationMatrix;

  cv::Mat globalMask;
};

#endif /* SRC_CAMERAPARAMS_H_ */
