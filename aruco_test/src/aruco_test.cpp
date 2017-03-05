#include "ros/ros.h"
#include <sstream>

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "aruco_test2");
	ros::NodeHandle n;
	
	ROS_INFO("Hello World!\n");

	ros::spin();
	return 0;
}
