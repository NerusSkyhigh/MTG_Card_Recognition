#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols/*.png";
const char* source_window = "Source";



Mat src_gray;
int thresh = 100;
RNG rng(12345);
void thresh_callback(int, void*);
void png_border_removal(Mat& img);
void scaling_to_40px(Mat& img);
int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "SPOSTATI DA VISUAL STUDIO!!!" << endl;
        return -1;
    }
    vector<string> fn;
    glob(png_dir, fn, true);

    Mat src = imread(argv[1], IMREAD_COLOR);
    if (!fn.size())
    {
        cerr << "No ghe n'casso!" << endl;
        return -1;
    }
    namedWindow(source_window);
    int i = 0;
    while (i < fn.size()) {
        Mat src = imread(fn[i], IMREAD_COLOR);
        png_border_removal(src);
        scaling_to_40px(src);
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
        blur(src_gray, src_gray, Size(3, 3));
        imshow(source_window, src);
        const int max_thresh = 255;
        createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, thresh_callback);
        thresh_callback(0, 0);
        waitKey();
        i++;
    }
    return 0;
}
void thresh_callback(int, void*)
{
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
    }
    imshow("Contours", drawing);
    cout<<"Contorni:"<<contours.size()<<endl;
}


void png_border_removal(Mat& img) {
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            if (img.at<Vec3b>(y, x)[0] == 0
                && img.at<Vec3b>(y, x)[1] == 0
                && img.at<Vec3b>(y, x)[2] == 0) {
                img.at<Vec3b>(y, x)[0] = 255;
                img.at<Vec3b>(y, x)[1] = 255;
                img.at<Vec3b>(y, x)[2] = 255;
            }
        }
    }
}
void scaling_to_40px(Mat& img) {
    double scale = img.rows / 40;
    Size new_img_size;
    new_img_size.height = img.size().height / scale;
    new_img_size.width = img.size().width / scale;
    resize(img, img, new_img_size, scale, scale, INTER_AREA);
}
