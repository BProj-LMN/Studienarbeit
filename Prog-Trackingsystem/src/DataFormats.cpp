/*
 * DataFormats.cpp
 *
 * function: unify interfaces by a central definition of data exchange formats
 *
 * author: Jannik Beyerstedt
 */

#include "DataFormats.h"

PxPos::PxPos()
    : x(0.0), y(0.0) {
}

PxPos::PxPos(float x_val, float y_val)
    : x(x_val), y(y_val) {
}

std::ostream& operator<<(std::ostream& os, const PxPos& obj) {
  os << "{" << "x: " << obj.x << ", y: " << obj.y << "}";
  return os;
}

VectRay::VectRay()
    : pos(), dir() {
}

VectRay::VectRay(Pos3D pos_val, Pos3D dir_val)
    : pos(pos_val), dir(dir_val) {
}

std::ostream& operator<<(std::ostream& os, const VectRay& obj) {
  os << "{" << "pos: " << obj.pos << ", dir: " << obj.dir << "}";
  return os;
}

bool VectRay::operator==(const VectRay& other) const {
  if (pos == other.pos && dir == other.dir) {
    return true;
  } else {
    return false;
  }
}

IntraSysMsg::IntraSysMsg() {
  camID = 9999; // default ctor with invalid ID, because not properly initialized
}

IntraSysMsg::IntraSysMsg(int camID_val, VectRayList rayList_val, Status trackingStatus_val)
    : camID(camID_val), rayList(rayList_val), trackingStatus(trackingStatus_val) {
}

std::ostream& operator<<(std::ostream& os, const IntraSysMsg& obj) {
  // TODO: good output of the data
  os << "IntraSystMsg for camID: " << obj.camID;
  return os;
}

Error::Error(std::string message)
    : msg(message) {
}

std::ostream& operator<<(std::ostream& os, const Error& obj) {
  os << "Error: " << obj.msg;
  return os;
}

std::string Error::what() {
  return msg;
}

