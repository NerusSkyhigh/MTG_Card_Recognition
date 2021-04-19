#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include<cstring>
#include<cmath>
using namespace std;
using namespace cv;

#ifndef __CROPPING_H__
#define __CROPPING_H__

#define MARGINE_PERCENTAGE 10

#define MARGINE_BLACK 256*MARGINE_PERCENTAGE/100
#define MARGINE_WHITE 256*(100-MARGINE_PERCENTAGE)/100

#define X_PERC_CENTER_BOTTOM 0.059523
#define Y_PERC_CENTER_BOTTOM 0.92948

#define Y_BOTTOM_LENGHT_RATEO 1.13898
#define X_BOTTOM_HIGHT_RATEO 23.4


Mat get_cropping(Mat& img);
void calculate_points_and_hights();
Mat copy_rectangle(Mat& imm, Point& start, int hights[2]);
Mat find_bottom(Mat bottom, Mat top);



#endif
