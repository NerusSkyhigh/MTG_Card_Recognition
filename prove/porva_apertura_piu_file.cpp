#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include<cstring>
using namespace std;
using namespace cv;

int main() {
    const char* pathfile = "C:/Users/User/Desktop/cards_db/Amonkhet/rare/*.jpg";
    vector<string> fn;
    glob(pathfile, fn, true);
    Mat src;
    for (int i = 0;i < fn.size();i++) {
        src = imread(fn[i], IMREAD_COLOR);
    }
    if (src.empty()) {
        cerr << "This bitch empty! YEEET\n";
        return EXIT_FAILURE;
    }
    namedWindow("Input", WINDOW_AUTOSIZE);
    namedWindow("Output", WINDOW_AUTOSIZE);

    imshow("Input", src);
    Mat test_clone;
    test_clone.create(src.size(), src.type());
    int nChannels = src.channels();
    int srcnrows = src.rows;
    int srcncol = src.cols;

    for (int i = 0;i < (srcnrows);i++) {
        uchar* psrcrow = src.ptr<uchar>(i);
        uchar* pclonerow = test_clone.ptr<uchar>(srcnrows - i - 1);
        for (int j = 0;j < srcncol;j++) {
            for (int k = 0;k < nChannels;k++) {
                *pclonerow++ = (psrcrow[srcncol - j + k - 1]);
            }
        }
    }

    imshow("Output", test_clone);
    waitKey();

    return 0;
}
