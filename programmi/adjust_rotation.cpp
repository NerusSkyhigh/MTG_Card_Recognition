#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include<cstring>
#include<cmath>
using namespace std;
using namespace cv;


//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
const char* path_image = "C:/Users/User/source/repos/LOOOL/x64/Debug/Storta.png";
const char* input = "Card";
const char* output = "output";

const int thresh_min = 128;
const int thresh_max = 256;
Mat canny_output;
RotatedRect bigger_rect;

#define GUI true
#define SHOWOUTPUT true
#define SHOWINPUT true

Mat card2;


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
            cout<<"input sizes "<<img.size()<<endl;
        }
    }
    Mat card_rotated = rotate_card(img);
    if (GUI) {
        if (SHOWOUTPUT) {
            namedWindow(output, WINDOW_AUTOSIZE);
            imshow(output, card_rotated);
            cout<<"output sizes"<<card_rotated.size()<<endl;
            waitKey(0);
        }
    }
    return 0;
}



double lato(Point2f point1, Point2f point2) {
    double x_2 = pow(point1.x - point2.x, 2);
    double y_2 = pow(point1.y - point2.y, 2);
    double ret = sqrt(x_2 + y_2);
    return ret;
}



int area_rect_rotated(RotatedRect rect) {
    Point2f rect_points[4];
    rect.points(rect_points);
    double h = lato(rect_points[0], rect_points[1]);
    double l = lato(rect_points[1], rect_points[2]);
    int ret = l * h;
    return ret;
}

RotatedRect find_bigger_rectangle(vector<vector<Point>> contours) {
    RotatedRect minRect = minAreaRect(contours[0]);
    for (int i = 1;i < contours.size();i++) {
        RotatedRect rect_cont = minAreaRect(contours[i]);
        if (minRect.size.height+ minRect.size.width < rect_cont.size.height+ rect_cont.size.width) {
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
    starting_point.x = bigger_rect.center.x - bigger_rect.size.width / 2+1;
    starting_point.y = bigger_rect.center.y - bigger_rect.size.height / 2+1;
    Mat ret = card2(cv::Rect(starting_point.x, starting_point.y, bigger_rect.size.width, bigger_rect.size.height));

    return ret;
}

Mat rotate_card(Mat img) {
    card2 = img;
    find_rectangle();

    Mat r = getRotationMatrix2D(bigger_rect.center, bigger_rect.angle, 1.0);
    warpAffine(card2, card2, r, card2.size());
    return copy_to_cropped_rectangle();


}





//
