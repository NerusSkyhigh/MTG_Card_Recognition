#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<cstring>
using namespace std;
using namespace cv;

const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
const char* input = "Card";
const char* output = "output";

#define GUI true
#define SHOWOUTPUT true
#define SHOWINPUT true

#define MARGINE_PERCENTAGE 10

#define MARGINE 256*MARGINE_PERCENTAGE/100

#define X_PERC_CENTER_BOTTOM 0.059523
#define Y_PERC_CENTER_BOTTOM 0.92948

#define Y_BOTTOM_LENGHT_RATEO 1.13898
#define X_BOTTOM_HIGHT_RATEO 23.4

Mat card2;
Point points[2];
int hights[2];
void calculate_points_and_hights();


Mat copy_rectangle(Mat& imm, Point& start, int hights[2]);

Mat get_rotation(Mat &img);
Mat find_bottom(Mat bottom, Mat top);

int main(int argc, char* argv[]) {
    Mat img;
    if (argc < 2) {
        img = imread(path_image, IMREAD_GRAYSCALE);

    }
    else {
        img = imread(argv[1], IMREAD_GRAYSCALE);
    }
    if (img.empty()) {
        cerr << "Can't read the image" << endl;
        return EXIT_FAILURE;
    }
    flip(img, img, -1);

    if (GUI) {
        if (SHOWINPUT) {
            namedWindow(input, WINDOW_KEEPRATIO);
            imshow(input, img);
        }
    }
    Mat card_bottom = get_rotation(img);
    if (GUI) {
        if (SHOWOUTPUT) {
            namedWindow(output, WINDOW_AUTOSIZE);
            imshow( output, card_bottom);
            waitKey(0);
        }
    }
    return 0;
}




Mat copy_rectangle(Mat& imm, Point& start, int hights[2]) {
    Mat ret;
    Mat tmp = imm(cv::Rect(start.x, start.y, hights[0], hights[1]));
    tmp.copyTo(ret);
    return ret;
}

void calculate_points_and_hights() {
    hights[0] = card2.cols / Y_BOTTOM_LENGHT_RATEO;
    hights[1] = card2.rows / X_BOTTOM_HIGHT_RATEO;


    points[0].x = card2.cols * X_PERC_CENTER_BOTTOM;
    points[0].y = card2.rows * Y_PERC_CENTER_BOTTOM;


    points[1].x = card2.cols * ( X_PERC_CENTER_BOTTOM);
    points[1].y = card2.rows * (1 - Y_PERC_CENTER_BOTTOM)-hights[1];
    return;
}

Mat find_bottom(Mat bottom, Mat top) {
    int bottom_black_count = 0;
    int top_black_count = 0;
    for (int y = 0;y < bottom.cols;y++) {
        for (int x = 0;x < bottom.rows;x++) {
            if (bottom.at<uchar>(x,y) < MARGINE) {
                bottom_black_count++;
            }
            if (top.at<uchar>(x, y) < MARGINE) {
                top_black_count++;
            }
        }
    }
    if (bottom_black_count > top_black_count) {
        return bottom;
    }
    else {
        flip(top, top, -1);
        return top;
    }
}

Mat get_rotation(Mat& img) {
    card2 = img;
    calculate_points_and_hights();
    Mat bottom_img = copy_rectangle(card2, points[0], hights);
    Mat top_img = copy_rectangle(card2, points[1], hights);
    return find_bottom(bottom_img, top_img);
}
