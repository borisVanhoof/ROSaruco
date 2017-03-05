// STD
#include <sstream>
#include <iostream>

// OpenCv
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "cvdrawingutils.h"

// ArUco
#include "aruco.h"

// ROS
#include "ros/ros.h"
//#include <cv_bridge/cv_bridge.h>

using namespace cv;
using namespace aruco;

void cvTrackBarEvents(int pos, void*);
bool readArguments( int argc, char **argv);

cv::Mat image;
CameraParameters TheCameraParameters;
MarkerDetector MDetector;
vector<Marker> TheMarkers;
float TheMarkerSize = -1;

double ThresParam1, ThresParam2;
int iThresParam1, iThresParam2;

string TheIntrinsicFile;


int main(int argc, char *argv[])
{
	ros::init(argc, argv, "aruco_test2");
	ros::NodeHandle n;

	if(readArguments(argc, argv) == false)
		return -1;
	
	//camera parameters if passed
	if(TheIntrinsicFile != "")
	{
		try
		{
			TheCameraParameters.readFromXMLFile(TheIntrinsicFile);
			TheCameraParameters.resize(image.size());
		}
		catch ( cv::Exception &e )
		{
			const char* err_msg = e.what();
			ROS_ERROR("%s\n", err_msg);
			return -1;
		}		
	}
	
	//for now use .jpg file
	cv::Mat image = cv::imread("test_img1.jpg", CV_LOAD_IMAGE_COLOR);
	if(!image.data)
	{	
		ROS_INFO("could not open the image file...\n");
		return -1;
	}
	cv::resize(image, image, cv::Size(0,0), 0.5, 0.5, CV_INTER_LINEAR);

	// Create gui
	cv::namedWindow("Display window", WINDOW_AUTOSIZE);
	
	MDetector.getThresholdParams(ThresParam1, ThresParam2);
	MDetector.setCornerRefinementMethod(MarkerDetector::LINES);

	iThresParam1 = ThresParam1;
	iThresParam2 = ThresParam2;

 	cv::createTrackbar("ThresParam1", "Display window", &iThresParam1, 13, cvTrackBarEvents);
 	cv::createTrackbar("ThresParam2", "Display window", &iThresParam2, 13, cvTrackBarEvents);

	//To Do: pose publisher

	// Detection of markers in the image
	MDetector.detect(image, TheMarkers, TheCameraParameters, TheMarkerSize);

	if(TheMarkers.size()<0)
	{
		ROS_ERROR("no markers found!\n");
		return -1;
	}

	if(TheCameraParameters.isValid()) 
	{
		for(size_t i=0; i<TheMarkers.size(); i++)
		{
			CvDrawingUtils::draw3dCube(image, TheMarkers[i], TheCameraParameters);
			CvDrawingUtils::draw3dAxis(image, TheMarkers[i], TheCameraParameters);		
		}
	}

	for(size_t i=0; i<TheMarkers.size(); i++)
	{
		TheMarkers[i].draw(image, cv::Scalar(0,200,0), 1, true);
		ROS_INFO("id: %d\n", TheMarkers[i].id);

		try
		{
			TheMarkers[i].calculateExtrinsics(TheMarkerSize, TheCameraParameters);
		}
		catch ( cv::Exception &e )
		{
			const char* err_msg = e.what();
			ROS_ERROR("%s\n", err_msg);
		}
	}

	cv::imshow("Display window", image);
	waitKey(0);

	//ros::spin();
	return 0;
}

void cvTrackBarEvents(int pos, void*)
{
	if(iThresParam1<3) iThresParam1 = 3;
	if(iThresParam1%2 != 1) iThresParam1++;

	if(iThresParam2<1) iThresParam2 = 1;
	
	ThresParam1 = iThresParam1;
	ThresParam2 = iThresParam2;

	MDetector.setThresholdParams(ThresParam1, ThresParam2);
}

bool readArguments( int argc, char **argv )
{
/*
	if(argc<2)
	{
		ROS_ERROR("Niet genoeg argumenten\n");
		ROS_ERROR("[intrinsics.yml] [size]\n");
		return false;
	}
*/

	if(argc>=2)
		TheIntrinsicFile = argv[1];
	
	if(argc>=3)
		TheMarkerSize = atof(argv[2]);
	
	return true;
}
