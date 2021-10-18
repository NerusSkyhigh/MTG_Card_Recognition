#ifndef __SET_RECOG_H__
#define __SET_RECOG_H__
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<vector>
//#include<cstring>
using namespace std;
using namespace cv;
//const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols_jpg/*.jpg";
//const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols_with_exeptios";/*.png";
/**/
#define CONST_RADIUS 16
#define CONST_RADIUS_LON_H 2.5


#define DIFFWITHWHITE true
#define SHOWOUTPUT true
#define PERCENTAGE false
#define SHOWSTARTINGIMAGE false
#define SHOWEXRACTS true
#define BLACKTOWHITE true


	String find_set(Mat img);
	Mat copy_rectangle(Mat imm, Point start, int cols, int rows);
	double somm_diff(Mat trov);
	double somm_diff(Mat trov, Mat templ);
	void find_symbol_center(Point ret);

	void bound_control(int& point, int lim);
	void recursive_testing(int i);
	String get_name_of_set();
	void get_templ(const String path);

	void match_templ(Mat extract, vector<float>& mins);


#endif
