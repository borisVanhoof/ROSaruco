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
bool readArguments( int argc, char *argv[]);

cv::Mat image;
CameraParameters TheCameraParameters;
MarkerDetector MDetector;
vector<Marker> TheMarkers;
float TheMarkerSize = -1;

double ThresParam1, ThresParam2;
int iThresParam1, iThresParam2;

string TheIntrinsicFile("");

cv::Mat rot_mat(3,3, cv::DataType<float>::type);
const float p_off = CV_PI;
const float r_off = CV_PI/2;
const float y_off = CV_PI/2;

void readFromXMLFile2(string filePath)
{
	cout << "test: " << filePath << endl;
     cv::FileStorage fs(filePath, cv::FileStorage::READ);
     int w=-1,h=-1;
     cv::Mat MCamera,MDist;

     fs["image_width"] >> w;
     fs["image_height"] >> h;
     fs["distortion_coefficients"] >> MDist;
     fs["camera_matrix"] >> MCamera;
 
	cout << "file = " << filePath << ", mcamera.cols = " << MCamera.cols << ", mcamera.rows = " << MCamera.rows << endl;
}

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

	float x_t, y_t, z_t;
	float roll, yaw, pitch;

	x_t = -TheMarkers[0].Tvec.at<Vec3f>(0,0)[0];
	y_t = -TheMarkers[0].Tvec.at<Vec3f>(0,0)[1];
	z_t = -TheMarkers[0].Tvec.at<Vec3f>(0,0)[2];
	
	cv::Rodrigues(TheMarkers[0].Rvec, rot_mat);

	pitch = -atan2(rot_mat.at<float>(2,0), rot_mat.at<float>(2,1));
	yaw = acos(rot_mat.at<float>(2,2));
	roll = -atan2(rot_mat.at<float>(0,2), rot_mat.at<float>(1,2));

	ROS_INFO("pitch: %f yaw: %f roll: %f\n", (pitch-p_off)*(180/CV_PI), (yaw-y_off)*(180/CV_PI), (roll-r_off)*(180/CV_PI));
	ROS_INFO("x_d: %f y_d: %f z_d: %f\n", x_t, y_t, z_t);


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

bool readArguments( int argc, char *argv[] )
{

	vector<string> filename;

	if(argc<2)
	{
		ROS_ERROR("Not enough arguments, usage:\n");
		ROS_INFO("[cam_param.xml or .yml] [marker size in meters (0.0038)]\n");
		return false;
	}

	if(argc>=2)
		TheIntrinsicFile = argv[1];
	
	if(argc>=3)
		TheMarkerSize = atof(argv[2]);
	
	return true;
}
