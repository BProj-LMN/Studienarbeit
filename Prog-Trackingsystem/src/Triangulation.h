/*
 * Triangulation.h
 *
 * function: utility class for triangulation of a 3D position using multiple linear equations in vector form (Object Rays / VectRay)
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_TRIANGULATION_H_
#define SRC_TRIANGULATION_H_

#include "DataFormats.h"
//#include "Logger.h"

class Triangulation {
public:
  static void calculatePosition(std::vector<IntraSysMsg>& messages, std::vector<Pos3D>& positions,  std::vector<int>& triangulationDistances);

private:
  static int triangulate(VectRay& ray1, VectRay& ray2, Pos3D& pos);
};

#endif /* SRC_TRIANGULATION_H_ */
