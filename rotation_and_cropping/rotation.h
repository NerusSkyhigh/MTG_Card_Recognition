#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include<cstring>
#include<cmath>
using namespace std;
using namespace cv;

#ifndef __ROTATION_H__
#define __ROTATION_H__

#define MARGINE_PERCENTAGE 10

#define MARGINE_BLACK 256*MARGINE_PERCENTAGE/100
#define MARGINE_WHITE 256*(100-MARGINE_PERCENTAGE)/100

const int thresh_min = 128;
const int thresh_max = 256;

Mat rotate_card(Mat img);
void control_borders();
void find_rectangle();
Mat copy_to_cropped_rectangle();
RotatedRect find_bigger_rectangle(vector<vector<Point>> contours);


#endif
