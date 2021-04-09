#include "verse_and_set.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<cstring>

Mat templ;
Mat dx, sx;
vector<string> fn;
vector<double> mins_dx, mins_sx;
const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols/*.png";
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



void verse_and_set(Mat img) {
        card = img;
        glob(png_dir, fn, true);

        if (fn.size() < 1) {
            cerr << "Can't find png/jpg in foulder" << endl;
            return;
        }
        radius_h = card.rows / CONST_RADIUS;
        radius_l = radius_h * CONST_RADIUS_LON0H;
        Point centers[2];
        find_symbol_center(centers);
        dx = copy_rectangle(card, centers[0], radius_l, radius_h);
        sx = copy_rectangle(card, centers[1], radius_l, radius_h);
        //    Mat imsimilar=test_verse_1();
        recursive_testing(0);
        find_verse_set();

    }



    Mat copy_rectangle(Mat imm, Point start, int cols, int rows) {
        Mat ret;
        Mat tmp = imm(cv::Rect(start.x, start.y, cols, rows));
        tmp.copyTo(ret);
        return ret;
    }



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
    void find_symbol_center(Point ret[2]) {
        ret[0].x = (int)(card.cols * CONST_LENGHT_SYM) - radius_l / 2;
        bound_control(ret[0].x, card.cols);
        ret[0].y = (int)((card.rows * CONST_HIGHT_SYM) - radius_h / 2);
        bound_control(ret[0].y, card.rows);
        ret[1].x = (int)(card.cols * (1 - CONST_LENGHT_SYM)) - radius_l / 2;
        bound_control(ret[1].x, card.cols);
        ret[1].y = (int)(card.rows * (1 - CONST_HIGHT_SYM)) - radius_h / 2;
        bound_control(ret[1].y, card.rows);
    }

    void bound_control(int& point, int lim) {
        if (point < 0) {
            point = 0;
        }
        if (point + radius_l - lim > 0) {
            point -= (point + radius_l) - lim;
        }
    }
    void recursive_testing(int i) {
        get_templ(fn[i]);
        if (SHOWOUTPUT && SHOWEXRACTS) {
            imshow(template_window, templ);
            cout << "testing" << i << ": " << fn[i] << endl;
        }

        if (SHOWOUTPUT && SHOWEXRACTS) {
            Mat tmp_dx = dx.clone();
            Mat tmp_sx = sx.clone();
            cout << "DX:";
            match_templ(tmp_dx, mins_dx);
            cout << "SX:";
            match_templ(tmp_sx, mins_sx);
            imshow("DX", tmp_dx);
            imshow("SX", tmp_sx);
            waitKey(0);
        }
        else {
            match_templ(dx, mins_dx);
            match_templ(sx, mins_sx);
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

    string find_verse_set() {
        double minAss_sx = 4200;
        double minAss_dx = 4200;
        int pos_minAss_dx = 0;
        int pos_minAss_sx = 0;
        for (int i = 0;i < mins_dx.size();i++) {
            if (mins_dx.at(i) < minAss_dx) {
                pos_minAss_dx = i;
                minAss_dx = mins_dx.at(i);
            }
            if (mins_sx.at(i) < minAss_sx) {
                pos_minAss_sx = i;
                minAss_sx = mins_sx.at(i);
            }

        }
        if (minAss_dx < minAss_sx) {
            cout << pos_minAss_dx;
        }
        else {
            cout << pos_minAss_sx;
        }

        cout << " the verse is ";
        if (minAss_dx < minAss_sx) {
            cout << "straight";
        }
        else {
            cout << "reversed";
        }
        cout << " and the set is ";
        if (minAss_dx < minAss_sx) {
            cout << fn[pos_minAss_dx];
        }
        else {
            cout << fn[pos_minAss_sx];
        }
        cout << endl;
        return "";
    }

    Mat test_verse_1() {
        cout << "Testo il verso:";
        Mat templ_probable_sim = imread(templ_probable_sim_path, IMREAD_COLOR);
        double sm_dx = somm_diff(dx, templ_probable_sim);
        double sm_sx = somm_diff(sx, templ_probable_sim);
        cout << " dx-" << sm_dx << " sx-" << sm_sx << endl;
        return Mat();
    }

    void match_templ(Mat extract, vector<double>& mins) {
        int result_cols = extract.cols - templ.cols + 1;
        int result_rows = extract.rows - templ.rows + 1;
        Mat result;
        result.create(result_rows, result_cols, CV_32FC1);

        matchTemplate(extract, templ, result, match_method);

        //    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;
        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
        double matchVal = maxVal / minVal;
        matchLoc = minLoc;
        Mat img_extract = copy_rectangle(extract, matchLoc, templ.cols, templ.rows);
        //cout << i << ":" << "Max/Min: " << matchVal << endl;
        mins.push_back(somm_diff(img_extract));
        if (SHOWOUTPUT && SHOWEXRACTS) {
            cout << "Ranking:" << mins.back() << endl;
            rectangle(extract, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
        }

    }



    void get_templ(const string path) {
        templ = imread(path, IMREAD_COLOR);
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
