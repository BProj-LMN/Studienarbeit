/*
 * TestCases.h
 *
 * function: all unit and integration tests using the boost test library
 *
 * author: Jannik Beyerstedt
 */

#ifndef SRC_TESTCASES_H_
#define SRC_TESTCASES_H_

#define BOOST_TEST_MODULE Trackingsystem Tests
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/included/unit_test.hpp> // header file

#include "CameraParams.h"
#include "ObjDetSimple.h"
#include "IntraSystemMessagingStub.h"

#define PX_TOLERANCE 3

BOOST_AUTO_TEST_CASE(test01a_CameraParams_calcObjRay) {
  CameraParams cam{"test/camConfig-test1.yml"};

  PxPos foo{100, 200};
  VectRay ray{};

  cam.calcObjRay(foo, ray);

  // pos and dir are "only" integer values!
  BOOST_TEST(ray.pos.x == 8950);
  BOOST_TEST(ray.pos.y == 3510);
  BOOST_TEST(ray.pos.z == 2900);

  BOOST_TEST(ray.dir.x == -850);
  BOOST_TEST(ray.dir.y == 219);
  BOOST_TEST(ray.dir.z == -39);
}

BOOST_AUTO_TEST_CASE(test01b_CameraParams_calcObjRay) {
  CameraParams cam{"test/camConfig-test2.yml"};

  PxPos foo{100, 200};
  VectRay ray{};

  cam.calcObjRay(foo, ray);

  // pos and dir are "only" integer values!
  BOOST_TEST(ray.pos.x == 8955);
  BOOST_TEST(ray.pos.y == 1100);
  BOOST_TEST(ray.pos.z == 2900);

  BOOST_TEST(ray.dir.x == -509);
  BOOST_TEST(ray.dir.y == 539);
  BOOST_TEST(ray.dir.z == -904);
}

BOOST_AUTO_TEST_CASE(test02a_ObjectDetection) {
  ObjectDetection* test = new ObjDetSimple();
  ImageSource capRef{"test/camFrame-test1-01.png"};
  ImageSource capDet{"test/camFrame-test1-02.png"};
  PxPosList position{};

  cv::Mat* frameRef = new cv::Mat{};
  cv::Mat frameDet;
  capRef >> *frameRef;
  capDet >> frameDet;

  test->setReferenceFrame(*frameRef);
  delete frameRef;
  test->detect(frameDet, position);

  BOOST_TEST(std::abs(position[0].x - 101) < PX_TOLERANCE);
  BOOST_TEST(std::abs(position[0].y - 329) < PX_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test02b_ObjectDetection) {
  ObjectDetection* test = new ObjDetSimple();
  ImageSource capRef{"test/camFrame-test2-01.png"};
  ImageSource capDet{"test/camFrame-test2-02.png"};
  PxPosList positions{};
  int positionsSize{};
  Status status{};

  cv::Mat* frameRef = new cv::Mat{};
  cv::Mat frameDet;
  capRef >> *frameRef;
  capDet >> frameDet;

  test->setReferenceFrame(*frameRef);
  delete frameRef;                      // check, if reference frame is copied to classes scope
  status = test->detect(frameDet, positions);
  positionsSize = positions.size();

  BOOST_TEST(status == Status::OK);
  BOOST_TEST(std::abs(positions[0].x - 954.0) < PX_TOLERANCE);
  BOOST_TEST(std::abs(positions[0].y - 760.0) < PX_TOLERANCE);

  capRef >> frameDet;                   // input the reference frame -> return Status::ERR
  status = test->detect(frameDet, positions);

  BOOST_TEST(status == Status::ERR);
  BOOST_TEST(positionsSize == positions.size()); // check, if positions vector does not increase size
}

BOOST_AUTO_TEST_CASE(test03_ObjectDetection) {
  ObjectDetection* objDet = new ObjDetSimple();
  ImageSource* src = new ImageSource{"test/camFrame-test1-01.png"};
  IntraSystemMessaging* intMsg = new IntraSystemMessagingStub{};
  CameraParams* cam = new CameraParams{"test/camConfig-test1.yml"};

  ImageProcessing imgProc{1, src, cam, objDet, intMsg};
  // ImageProcessing ctor is setting up the reference frame
  // then change the source of the ImageSource from reference frame to tracking image
  src->release();
  src->open("test/camFrame-test1-02.png");

  imgProc.evaluate();

  // check IntraSystemMessagingStub for correct IntraSysMsg input
  IntraSystemMessagingStub* messagingStub = static_cast<IntraSystemMessagingStub*>(intMsg);
  IntraSysMsg message = messagingStub->getMessage();

  //BOOST_TEST(std::abs(position[0].x - 101) < PX_TOLERANCE);
  //BOOST_TEST(std::abs(position[0].y - 329) < PX_TOLERANCE);
  BOOST_TEST(message.camID == 1);
  BOOST_TEST(message.trackingStatus == Status::OK);

  BOOST_TEST_REQUIRE(message.rayList.size() > 0);
  Pos3D dir = message.rayList[0].dir;
  Pos3D pos = message.rayList[0].pos;

  BOOST_TEST(pos.x == 8950);
  BOOST_TEST(pos.y == 3510);
  BOOST_TEST(pos.z == 2900);

  BOOST_TEST(std::abs(dir.x - -850) < PX_TOLERANCE);
  BOOST_TEST(std::abs(dir.y - 218) < PX_TOLERANCE);
  BOOST_TEST(std::abs(dir.z - 89) < PX_TOLERANCE);

  // delete handled by BOOST
}

#endif /* SRC_TESTCASES_H_ */
