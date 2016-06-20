/*
 * DataFormats.h
 *
 * function: unify interfaces by a central definition of data exchange formats
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_DATAFORMATS_H_
#define SRC_DATAFORMATS_H_

#include <opencv2/core.hpp>

#include <vector>
#include <exception>

typedef enum {
  ERR = -1, OK = 0
} Status;

class PxPos {
public:
  float x;
  float y;

  PxPos()
      : x(0.0), y(0.0) {
  }

  PxPos(float x_val, float y_val)
      : x(x_val), y(y_val) {
  }

  friend std::ostream& operator<<(std::ostream& os, const PxPos& obj) {
    os << "{" << "x: " << obj.x << ", y: " << obj.y << "}";
    return os;
  }
};

typedef std::vector<PxPos> PxPosList;

typedef cv::Point3i Pos3D;

class VectRay {
public:
  Pos3D pos;
  Pos3D dir;

  VectRay()
      : pos(), dir() {
  }

  VectRay(Pos3D pos_val, Pos3D dir_val)
      : pos(pos_val), dir(dir_val) {
  }

  friend std::ostream& operator<<(std::ostream& os, const VectRay& obj) {
    os << "{" << "pos: " << obj.pos << ", dir: " << obj.dir << "}";
    return os;
  }
};

typedef std::vector<VectRay> VectRayList; // TODO: some deep copy needed?

class IntraSysMsg {
public:
  int camID;
  VectRayList rayList;
  Status trackingStatus;

  IntraSysMsg() {
    camID = 9999; // default ctor with invalid ID, because not properly initialized
  }

  IntraSysMsg(int camID_val, VectRayList rayList_val, Status trackingStatus_val)
      : camID(camID_val), rayList(rayList_val), trackingStatus(trackingStatus_val) {
  }

  friend std::ostream& operator<<(std::ostream& os, const IntraSysMsg& obj) {
    // TODO: good output of the data
    os << "IntraSystMsg for camID: " << obj.camID;
    return os;
  }
};

class Error {
public:
  Error(std::string message)
      : msg(message) {
  }

  friend std::ostream& operator<<(std::ostream& os, const Error& obj) {
    os << "Error: " << obj.msg;
    return os;
  }

  std::string what() {
    return msg;
  }

private:
  std::string msg;
};

#endif /* SRC_DATAFORMATS_H_ */
