#include "ros/ros.h"
#include <sstream>

#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "aruco_test2");
	ros::NodeHandle n;
	
	ROS_INFO("Hello World!\n");

	Mat image = cv::imread("lena.jpeg", CV_LOAD_IMAGE_COLOR);
	if(!image.data)
	{	
		ROS_INFO("kon image niet openen...\n");
		return -1;
	}

	cv::namedWindow("Display window", WINDOW_AUTOSIZE);
	cv::imshow("Display window", image);
	cv::waitKey(30);

	ros::spin();
	return 0;
}
