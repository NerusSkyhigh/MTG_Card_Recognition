#include "set_recog.h"
/*#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<cstring>
*/
Mat templ;
Mat crop_img;
vector<string> fn;
vector<double> mins_crop_img;
const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols/*.png";
/**/
const int match_method = 0;//TM_SQDIFF
int radius_h;
int radius_l;
int scaling = 24;
int perse = 100;
//const double CONST_HIGHT_SYM = 0.589743;
const double CONST_HIGHT_SYM = 0.592;
//const double CONST_LENGHT_SYM = 0.863095238;
const double CONST_LENGHT_SYM = 0.835;
const char* template_window = "template_window";

const char* templ_probable_sim_path = "C:/Users/User/Desktop/templ_ric_verso.png";

Mat card;


//
//
//
//
string find_set(Mat img) {
	card = img;
	glob(png_dir, fn, true);
	if (fn.size() < 1) {
		cerr << "Can't find png/jpg in foulder" << endl;
		return "";
	}
	radius_h = card.rows / CONST_RADIUS;
	radius_l = radius_h * CONST_RADIUS_LON_H;
	Point prob_img_center;
	find_symbol_center(prob_img_center);
	//ONLY RIGHT IMAGE
	crop_img = copy_rectangle(card, prob_img_center, radius_l, radius_h);
	//Mat imsimilar=test_verse_1();
	recursive_testing(0);
	return get_name_of_set();

}


//returns the rectangle in a new Mat()
//@param image to crop
//@param the starting point
//@param the number of colomns
//@param the numbero of rows
//@return a new Mat() with the image cropped
Mat copy_rectangle(Mat imm, Point start, int cols, int rows) {
	Mat ret;
	Mat tmp = imm(cv::Rect(start.x, start.y, cols, rows));
	tmp.copyTo(ret);
	return ret;
}


//A function that "rates" in a very rudimental way
//the similarity of the template and the extract of the card (of "templ" and the parameter)
//@param an image that needs to have the same sizes of the "templ"
//@return an estimate of similarity
double somm_diff(Mat trov) {
	double ret = 0;
	double sott = 0;
	for (int y = 0; y < templ.rows; y++) {
		for (int x = 0; x < templ.cols; x++) {
			for (int i = 0;i < 3;i++) {
				//Occhio ai canali ter
				if (templ.at<Vec3b>(y, x)[i] < 255) {
					double loc = trov.at<Vec3b>(y, x)[i] - templ.at<Vec3b>(y, x)[i];
					if (loc > 0) {
						ret += loc;
					}
					else ret -= loc;
				}
				else {
					if (DIFFWITHWHITE)
						sott++;
					else {
						double loc = trov.at<Vec3b>(y, x)[i] - templ.at<Vec3b>(y, x)[i];
						if (loc > 0) {
							ret += loc;
						}
						else ret -= loc;
					}
				}
			}
		}
	}
	ret /= ((templ.cols * templ.rows * templ.channels()) - sott);
	return ret;
}




//calls somm_diff(@param) but with a different template
double somm_diff(Mat trov, Mat templ) {
	double ret = 0;
	double sott = 0;
	for (int y = 0; y < templ.rows; y++) {
		for (int x = 0; x < templ.cols; x++) {
			for (int i = 0;i < 3;i++) {
				//Occhio ai canali ter
				if (templ.at<Vec3b>(y, x)[i] < 255) {
					double loc = trov.at<Vec3b>(y, x)[i] - templ.at<Vec3b>(y, x)[i];
					if (loc > 0) {
						ret += loc;
					}
						else ret -= loc;
				}
				else {
					if (DIFFWITHWHITE)
						sott++;
					else {
						double loc = trov.at<Vec3b>(y, x)[i] - templ.at<Vec3b>(y, x)[i];
						if (loc > 0) {
							ret += loc;
					}
					else ret -= loc;
					}
				}
			}
		}
	}
	ret /= ((templ.cols * templ.rows * templ.channels()) - sott);
	return ret;
}




//control if the center is't out of bound (oob)
void bound_control(int& point, int lim) {
	if (point < 0) {
		point = 0;
	}
	if (point + radius_l - lim > 0) {
		point -= (point + radius_l) - lim;
	}
}

//finds the center of the symbol in the card
//@param center of the position of the symbol
//(if the image was rotated, yes im a dumass)
void find_symbol_center(Point ret) {
	ret.x = (int)(card.cols * CONST_LENGHT_SYM) - radius_l / 2;
	bound_control(ret.x, card.cols);
	ret.y = (int)((card.rows * CONST_HIGHT_SYM) - radius_h / 2);
	bound_control(ret.y, card.rows);
}

//recursive testing of templets
//Why recursive? because of yes
void recursive_testing(int i) {
	get_templ(fn[i]);
	if (SHOWOUTPUT && SHOWEXRACTS) {
		imshow(template_window, templ);
		cout << "testing" << i << ": " << fn[i] << endl;
	}
	if (SHOWOUTPUT && SHOWEXRACTS) {
		Mat tmp_crop_img = crop_img.clone();
		cout << "crop_img:";
		match_templ(tmp_crop_img, mins_crop_img);
		imshow("crop_img", tmp_crop_img);
		waitKey(0);
	}
	else {
		match_templ(crop_img, mins_crop_img);
	}
	if (PERCENTAGE && SHOWOUTPUT) {
		int perse_now = (i * 100) / fn.size();
		if (perse_now % 10 == 0 && perse_now != perse) {
			perse = perse_now;
			cout << "Sets tested: " << (int)(i * 100) / fn.size() << "%" << endl;
		}
	}
	if (fn.size() > i + 1)
		recursive_testing(++i);
}


//find the verse of the card with a comperison of the similarity
string get_name_of_set() {
	double minAss_crop_img = 4200;
	int pos_minAss_crop_img = 0;
	for (int i = 0;i < mins_crop_img.size();i++) {
		if (mins_crop_img.at(i) < minAss_crop_img) {
			pos_minAss_crop_img = i;
			minAss_crop_img = mins_crop_img.at(i);
		}
	}
	cout << "The set is ";
	cout << fn[pos_minAss_crop_img];
	cout << endl;
	return fn[pos_minAss_crop_img];
}


//Test on the verse of the card based on a card template
/*Mat test_verse_1() {
	cout << "Testo il verso:";
	Mat templ_probable_sim = imread(templ_probable_sim_path, IMREAD_COLOR);
	double sm_dx = somm_diff(dx, templ_probable_sim);
	double sm_sx = somm_diff(sx, templ_probable_sim);
	cout << " dx-" << sm_dx << " sx-" << sm_sx << endl;
	cout << " dx-" << sm_dx << " sx-" << sm_sx << endl;
}
*/

//trys to match the template "templ" and the image cropped
//for every row.
//adds the mininimal result of the matchTemplate algorithm in "mins"
//@param the extract of the card, I imagine
//@param stores the minimal value of the match
void match_templ(Mat extract, vector<double>& mins) {
	//This is how the algorithm works
	//it needs how many times the image will be applied
	//in the "extract"
	//(THE "extract" IS ALWAYS BIGGER THEN THE "templ"
	//('couse my program gud))
	int result_cols = extract.cols - templ.cols + 1;
	int result_rows = extract.rows - templ.rows + 1;
	Mat result;
	result.create(result_rows, result_cols, CV_32FC1);
	matchTemplate(extract, templ, result, match_method);
	//No need to normalized
	//	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	//finds the minima points in the resulting image
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	double matchVal = maxVal / minVal;
	//easier on the eyes
	matchLoc = minLoc;
	Mat img_extract = copy_rectangle(extract, matchLoc, templ.cols, templ.rows);
	//cout << i << ":" << "Max/Min: " << matchVal << endl;
	mins.push_back(somm_diff(img_extract));
	if (SHOWOUTPUT && SHOWEXRACTS) {
		cout << "Ranking:" << mins.back() << endl;
		rectangle(extract, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	}
}



//TODO: future intance of this funcion will have all the symbol already re-colorized NOT re-scaled


//Sets the global variable "templ" as a new set image,
//converts all white pixels in black pixels in white
//And scale the symbol accordingly to the dimention of the image
//@param the name of the symbol in the dirctoty where the images are stored
void get_templ(const string path) {
	//takes the image from the file
	//WARNING: path needs to change accordingly
	templ = imread(path, IMREAD_COLOR);
	//Converting black pixels in white pixels
	if (BLACKTOWHITE) {
		for (int y = 0; y < templ.rows; y++) {
			for (int x = 0; x < templ.cols; x++) {
				if (templ.at<Vec3b>(y, x)[0] == 0
					&& templ.at<Vec3b>(y, x)[1] == 0
					&& templ.at<Vec3b>(y, x)[2] == 0) {
					templ.at<Vec3b>(y, x)[0] = 255;
					templ.at<Vec3b>(y, x)[1] = 255;
					templ.at<Vec3b>(y, x)[2] = 255;
				}
			}
		}
	}

	//Scaling
	int max;
	if (templ.rows < templ.cols) {
		max = templ.cols;
	}
	else {
		max = templ.rows;
	}
	double templ_px = card.rows / scaling;
	double scale = templ.rows / templ_px;
	Size new_templ_size;
	new_templ_size.height = templ.size().height / scale;
	new_templ_size.width = templ.size().width / scale;
	resize(templ, templ, new_templ_size, scale, scale, INTER_AREA);
}

int main(){
	cout<<"it just works";
	return 2;
}
