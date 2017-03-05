#include "ros/ros.h"
#include <sstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "aruco_test2");
	ros::NodeHandle n;
	
	ROS_INFO("Hello World!\n");

	cv::Mat image = cv::imread("test_img1.jpg", CV_LOAD_IMAGE_COLOR);
	if(!image.data)
	{	
		ROS_INFO("kon image niet openen...\n");
		return -1;
	}

	cv::resize(image, image, cv::Size(0,0), 0.25, 0.25, CV_INTER_LINEAR);

	cv::namedWindow("Display window", WINDOW_AUTOSIZE);
	cv::imshow("Display window", image);
	cv::waitKey(0);

	ros::spin();
	return 0;
}
