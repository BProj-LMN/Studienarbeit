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
#include "Triangulation.h"

#define PX_TOLERANCE 3

BOOST_AUTO_TEST_CASE(ImageProcessing_test01a_CameraParams_calcObjRay) {
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

BOOST_AUTO_TEST_CASE(ImageProcessing_test01b_CameraParams_calcObjRay) {
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

BOOST_AUTO_TEST_CASE(ImageProcessing_test02a_ObjectDetection) {
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

BOOST_AUTO_TEST_CASE(ImageProcessing_test02b_ObjectDetection) {
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

BOOST_AUTO_TEST_CASE(ImageProcessing_test03_Integration) {
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

BOOST_AUTO_TEST_CASE(IntraSystemMessaging_test01) {
  IntraSystemMessaging* test = new IntraDirect{};
  VectRayList* vectList = new VectRayList{};
  VectRay ray1{Pos3D(11, 12, 13), Pos3D{110, 120, 130}};
  VectRay ray2{Pos3D(21, 22, 23), Pos3D{210, 220, 230}};
  vectList->push_back(ray1);
  vectList->push_back(ray2);
  IntraSysMsg msgSent{1, *vectList, Status::ERR};
  IntraSysMsg msgRecv{};

  test->send(msgSent);
  test->recv(msgRecv);

  BOOST_TEST(&msgSent != &msgRecv);
  BOOST_TEST(msgSent.camID == msgRecv.camID);
  BOOST_TEST(msgSent.trackingStatus == msgRecv.trackingStatus);
  BOOST_TEST(msgSent.rayList.size() == msgRecv.rayList.size());
  BOOST_TEST(msgSent.rayList == msgRecv.rayList);
}

BOOST_AUTO_TEST_CASE(ClusterMngmt_test01_Triangulation) {
  Triangulation test{};
  Pos3D position{};
  std::vector<int> triangulationDistances{};
  std::vector<IntraSysMsg> messages{};

  Pos3D pos1{0, 100, 300};
  Pos3D dir1{100, 20, 1};
  VectRay ray1{pos1, dir1};
  VectRayList list1{ray1};
  IntraSysMsg msg1{1, list1, Status::OK};
  messages.push_back(msg1);

  Pos3D pos2{0, 500, 300};
  Pos3D dir2{100, -20, 0};
  VectRay ray2{pos2, dir2};
  VectRayList list2{ray2};
  IntraSysMsg msg2{2, list2, Status::OK};
  messages.push_back(msg2);

  test.calculatePosition(messages, position, triangulationDistances);

  BOOST_TEST(triangulationDistances.size() == 1);
  BOOST_TEST(triangulationDistances[0] < 10);
  BOOST_TEST(triangulationDistances[0] > 0); // distance always positive
  BOOST_TEST(std::abs(position.x - 1000) < PX_TOLERANCE);
  BOOST_TEST(std::abs(position.y - 300) < PX_TOLERANCE);
  BOOST_TEST(std::abs(position.z - 302) < PX_TOLERANCE);
}

#endif /* SRC_TESTCASES_H_ */
