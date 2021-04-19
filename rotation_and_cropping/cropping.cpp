#include"cropping.h"

Mat card2;
Point points[2];
int hights[2];


Mat get_cropping(Mat& img) {
    card2 = img;
    calculate_points_and_hights();
    Mat bottom_img = copy_rectangle(card2, points[0], hights);
    Mat top_img = copy_rectangle(card2, points[1], hights);
    return find_bottom(bottom_img, top_img);
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

Mat copy_rectangle(Mat& imm, Point& start, int hight[2]) {
    Mat ret;
    Mat tmp = imm(cv::Rect(start.x, start.y, hight[0], hight[1]));
    tmp.copyTo(ret);
    return ret;
}

Mat find_bottom(Mat bottom, Mat top) {
    int bottom_black_count = 0;
    int top_black_count = 0;
    for (int y = 0;y < bottom.cols;y++) {
        for (int x = 0;x < bottom.rows;x++) {
            if (bottom.at<uchar>(x, y) < MARGINE_BLACK) {
                bottom_black_count++;
            }
            if (top.at<uchar>(x, y) < MARGINE_BLACK) {
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
