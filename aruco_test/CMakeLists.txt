cmake_minimum_required(VERSION 2.8.3)
project(aruco_test)


find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
  cv_bridge 
  image_transport
  geometry_msgs
)

SET(CMAKE_MODULE_PATH ${ARUCO_PATH}/lib/cmake)
SET(ARUCO_INCLUDE_DIRS ${ARUCO_PATH}/include/aruco)

FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(aruco REQUIRED)

#anders wilt em ni compileren
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -march=core-avx-i -O2") 

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES aruco_test
#  CATKIN_DEPENDS message_generation roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${ARUCO_INCLUDE_DIRS}
)

LIST(APPEND CMAKE_MODULE_PATH /usr/local/lib/cmake)

add_executable(aruco_test src/aruco_test.cpp)

target_link_libraries(aruco_test 
  ${catkin_LIBRARIES} 
  ${OpenCV_LIBS}
  ${aruco_LIBS}
)

