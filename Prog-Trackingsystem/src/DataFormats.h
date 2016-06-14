/*
 * DataFormats.h
 *
 *  Created on: 14.06.2016
 *      Author: jannik
 */

#ifndef SRC_DATAFORMATS_H_
#define SRC_DATAFORMATS_H_

#include <vector>

#include <opencv2/core.hpp>

class PxPos {
public:
  float x;
  float y;

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

  VectRay(Pos3D pos_val, Pos3D dir_val)
      : pos(pos_val), dir(dir_val) {
  }

  friend std::ostream& operator<<(std::ostream& os, const VectRay& obj) {
    os << "{" << "pos: " << obj.pos << ", dir: " << obj.dir << "}";
    return os;
  }
};

typedef std::vector<VectRay> VectRayList;

class IntraSysMsg {
public:
  int camID;
  VectRayList rayList;

  IntraSysMsg(int camID_val, VectRayList rayList_val)
      : camID(camID_val), rayList(rayList_val) {
  }

  friend std::ostream& operator<<(std::ostream& os, const IntraSysMsg& obj) {
    // TODO: good output of the data
    os << "IntraSystMsg for camID: " << obj.camID;
    return os;
  }
};

#endif /* SRC_DATAFORMATS_H_ */
