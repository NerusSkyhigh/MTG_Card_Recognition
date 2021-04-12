#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
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



int thresh =127;




Mat card2;


Mat find_rotation(Mat img);


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
        }
    }
    Mat card_rotated = find_rotation(img);
    if (GUI) {
        if (SHOWOUTPUT) {
            namedWindow(output, WINDOW_AUTOSIZE);
            imshow(output, card_rotated);
            waitKey(0);
        }
    }
    return 0;
}

void find_rectangle() {
  Mat canny_output;
  Canny( card2, canny_output, thresh, thresh*2 );
  vector<vector<Point> > contours;
  findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

}


void calcolate_angle(){}
Mat rotate_card() {
    return Mat();
}

Mat find_rotation(Mat img) {
    card2 = img;
    find_rectangle();
    calcolate_angle();

    return rotate_card();
}





//
