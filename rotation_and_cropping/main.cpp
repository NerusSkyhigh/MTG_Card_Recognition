#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include<cstring>
#include<cmath>
#include"cropping.h"
#include"rotation.h"
using namespace std;
using namespace cv;

//Path for debugging images
//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
//const char* path_image = "C:/Users/User/source/repos/LOOOL/x64/Debug/Storta.png";
const char* path_image = "C:/Users/User/source/repos/LOOOL/x64/Debug/Storta.png";
//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";



//Constant name of output windows
const char* input = "Card";
const char* output = "output";

#define GUI true        //for all the output for debugging
#define SHOWOUTPUT true //toggle the output image for debugging
#define SHOWINPUT true  //toggle the input image for debugging


//Timer per calcolare il tempo delle funzioni
#define TIMER true
void start_timer();
double stop_timer();

//MAIN

int main(int argc, char* argv[]) {
    //The image is taken from the argv if there is once
    //if there is none it's taken from the variable
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
    //for now the image is not stored
    if (GUI) {
        if (SHOWINPUT) {
            namedWindow(input, WINDOW_KEEPRATIO);
            imshow(input, img);
            cout << "input sizes " << img.size() << endl;
        }
    }
    Mat card_rotated;
    Mat cropped;
    if (TIMER) {
        double time;
        start_timer();
        card_rotated = rotate_card(img);
        time = stop_timer();
        cout << "Time to rotate card: " << time << " ms" << endl;
        start_timer();
        cropped = get_cropping(card_rotated);
        time = stop_timer();
        cout << "Time to crop card:" << time << endl;
    }
    else {
        card_rotated = rotate_card(img);
        cropped = get_cropping(card_rotated);
    }
    if (GUI) {
        if (SHOWOUTPUT) {
            namedWindow("Card Rotated", WINDOW_KEEPRATIO);
            imshow("Card Rotated", card_rotated);
            namedWindow(output, WINDOW_AUTOSIZE);
            imshow(output, cropped);
            cout << "card rotated size " << card_rotated.size() << endl;
            cout << "output cropped size " << cropped.size() << endl;
            waitKey(0);
        }
    }
    return 0;
}

//Thanks openCv for the counting operation
double freq;
int64 start;

void start_timer() {
    freq = getTickFrequency();
    start = getTickCount();
}

double stop_timer() {
    return double(getTickCount() - start) * 1000 / freq;
}
