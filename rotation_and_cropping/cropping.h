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

//The percentage of the point where the bottom black rectangle is
#define X_PERC_CENTER_BOTTOM 0.059523
#define Y_PERC_CENTER_BOTTOM 0.92948

//The rateo of the bottom black rectangle
#define Y_BOTTOM_LENGHT_RATEO 1.13898
#define X_BOTTOM_HIGHT_RATEO 23.4

//Crops the card
//@param image of the card (needs to be the right proportions)
//@return image of the bottom black rectangle
Mat get_cropping(Mat& img);

//calculate the points where the bottom black rectangle could be
//and their lenghts
void calculate_points_and_hights();

//@param image
//@param starting point
//@param hights
//@return image copied
//Copies the image in the rectangle pointed
Mat copy_rectangle(Mat& imm, Point& start, int hights[2]);

//@param bottom image
//@param top image
//@return the bottom black rectangle
//finds whitch direction is the card and returns the
//right bottom black rectangle flipped if needed
Mat find_bottom(Mat bottom, Mat top);

#endif
