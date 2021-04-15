#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include<cstring>
#include<cmath>
using namespace std;
using namespace cv;


//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
//const char* path_image = "C:/Users/User/source/repos/LOOOL/x64/Debug/Storta.png";
const char* path_image = "C:/Users/User/source/repos/LOOOL/x64/Debug/Storta2.png";

const int thresh_min = 128;
const int thresh_max = 256;
Mat canny_output;
RotatedRect bigger_rect;

//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
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

Mat get_cropping(Mat& img);
Mat find_bottom(Mat bottom, Mat top);




Mat rotate_card(Mat img);

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

    if (GUI) {
        if (SHOWINPUT) {

            namedWindow(input, WINDOW_KEEPRATIO);
            imshow(input, img);
            cout << "input sizes " << img.size() << endl;
        }
    }
    Mat card_rotated = rotate_card(img);
    Mat cropped = get_cropping(card_rotated);
    if (GUI) {
        if (SHOWOUTPUT) {
            namedWindow(output, WINDOW_AUTOSIZE);
            imshow(output, cropped);
            cout << "output sizes" << card_rotated.size() << endl;
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


    points[1].x = card2.cols * (X_PERC_CENTER_BOTTOM);
    points[1].y = card2.rows * (1 - Y_PERC_CENTER_BOTTOM) - hights[1];
    return;
}

Mat find_bottom(Mat bottom, Mat top) {
    int bottom_black_count = 0;
    int top_black_count = 0;
    for (int y = 0;y < bottom.cols;y++) {
        for (int x = 0;x < bottom.rows;x++) {
            if (bottom.at<uchar>(x, y) < MARGINE) {
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

Mat get_cropping(Mat& img) {
    card2 = img;
    calculate_points_and_hights();
    Mat bottom_img = copy_rectangle(card2, points[0], hights);
    Mat top_img = copy_rectangle(card2, points[1], hights);
    return find_bottom(bottom_img, top_img);
}

RotatedRect find_bigger_rectangle(vector<vector<Point>> contours) {
    RotatedRect minRect = minAreaRect(contours[0]);
    for (int i = 1;i < contours.size();i++) {
        RotatedRect rect_cont = minAreaRect(contours[i]);
        if (minRect.size.height + minRect.size.width < rect_cont.size.height + rect_cont.size.width) {
            minRect = rect_cont;
        }
    }
    return minRect;
}

void find_rectangle() {
    Canny(card2, canny_output, thresh_min, thresh_max);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    bigger_rect = find_bigger_rectangle(contours);
}


Mat copy_to_cropped_rectangle() {
    Point starting_point;
    starting_point.x = bigger_rect.center.x - bigger_rect.size.width / 2 + 1;
    starting_point.y = bigger_rect.center.y - bigger_rect.size.height / 2 + 1;
    Mat ret = card2(cv::Rect(starting_point.x, starting_point.y, bigger_rect.size.width, bigger_rect.size.height));
    return ret;
}

Mat rotate_card(Mat img) {
    card2 = img;
    find_rectangle();
    if (bigger_rect.size.width > bigger_rect.size.height) {
        double prova = bigger_rect.size.width;
        bigger_rect.size.width = bigger_rect.size.height;
        bigger_rect.size.height = prova;
        bigger_rect.angle += 90;
    }
    Mat r = getRotationMatrix2D(bigger_rect.center, bigger_rect.angle, 1.0);
    warpAffine(card2, card2, r, card2.size());
    return copy_to_cropped_rectangle();


}
