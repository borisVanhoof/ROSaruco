# ROSaruco

## Doel

Proberen een ROS-project te maken dat met ArUco en OpenCV werkt.

## comando's

### ROS 
in catkin_ws: catkin_make --pkg aruco_test
in catkin_ws/devel/lib/aruco_test: ./aruco_test [yml-file] [marker-size]

### GIT 
git add /ROSaruco
git commit -m "some comments"
git push origin master

## ChangeLog
05/03/17: identificeren id's van de ArUco markers

## To Do
CameraParameters voor extrinsics (YML file voor de raspberry pi camera v2)

## Handige Links

### installing OpenCV 2.4 
( https://gist.github.com/arthurbeggs/06df46af94af7f261513934e56103b30 )
### installing ArUco 
1. Download (https://sourceforge.net/projects/aruco/files/2.0.19/)
2. extract
3. mkdir build
4. cd build
5. cmake ..
6. make
7. make install

### github
https://www.howtoforge.com/tutorial/install-git-and-github-on-ubuntu-14.04/#-adding-repository-files-to-an-index

### ROS cheat sheet

https://www.clearpathrobotics.com/ros-robot-operating-system-cheat-sheet/

## AruCo
ArUco README ( http://www.uco.es/investiga/grupos/ava/sites/default/files/GarridoJurado2014.pdf )
AruCo example ( https://github.com/warp1337/ros_aruco ) 
