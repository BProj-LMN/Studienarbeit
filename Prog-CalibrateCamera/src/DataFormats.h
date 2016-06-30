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

  PxPos();
  PxPos(float x_val, float y_val);

  friend std::ostream& operator<<(std::ostream& os, const PxPos& obj);
};

typedef std::vector<PxPos> PxPosList;

typedef cv::Point3i Pos3D;

class VectRay {
public:
  Pos3D pos;
  Pos3D dir;

  VectRay();
  VectRay(Pos3D pos_val, Pos3D dir_val);

  friend std::ostream& operator<<(std::ostream& os, const VectRay& obj);
  bool operator==(const VectRay& other) const;
};

typedef std::vector<VectRay> VectRayList;

class IntraSysMsg {
public:
  int camID;
  VectRayList rayList;
  Status trackingStatus;

  IntraSysMsg();
  IntraSysMsg(int camID_val, VectRayList rayList_val, Status trackingStatus_val);

  friend std::ostream& operator<<(std::ostream& os, const IntraSysMsg& obj);
};

class Error {
public:
  Error(std::string message);

  friend std::ostream& operator<<(std::ostream& os, const Error& obj);
  std::string what();

private:
  std::string msg;
};

#endif /* SRC_DATAFORMATS_H_ */
