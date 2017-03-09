cmake_minimum_required(VERSION 2.8.3)
project(calibration)


find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)


FIND_PACKAGE(OpenCV REQUIRED)


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
)

LIST(APPEND CMAKE_MODULE_PATH /usr/local/lib/cmake)

add_executable(calibration src/camera_calibration.cpp)

target_link_libraries(calibration 
  ${catkin_LIBRARIES} 
  ${OpenCV_LIBS}
)

