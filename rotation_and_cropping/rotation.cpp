#include"rotation.h"



RotatedRect bigger_rect;
Mat canny_output;
Mat card;

double freque;
int64 starte;

void st_timer() {
    freque = getTickFrequency();
    starte = getTickCount();
}

double sp_timer() {
    return double(getTickCount() - starte) * 1000 / freque;
}



Mat rotate_card(Mat img) {
    card = img;
    control_borders();
    find_rectangle();
    if (bigger_rect.size.width > bigger_rect.size.height) {
        double prova = bigger_rect.size.width;
        bigger_rect.size.width = bigger_rect.size.height;
        bigger_rect.size.height = prova;
        bigger_rect.angle += 90;
    }

    Mat r = getRotationMatrix2D(bigger_rect.center, bigger_rect.angle, 1.0);
    warpAffine(card, card, r, card.size());
    Mat copied = copy_to_cropped_rectangle();
    return copied;
}

void control_borders() {
    int border = 1;
    copyMakeBorder(card, card, border, border,
        border, border, BORDER_CONSTANT, 255);
}


#define WHITE 255
#define BLACK 0

void draw_line(Mat silu, int row, int start, int end,int color) {
    for (int i = start;i < end;i++) {
        silu.at<uchar>(row, i) = color;
    }
}


int find_left_border(Mat silu,int row) {
    int i = 0;
    for ( i = 0;i < card.cols;i++) {
        if (card.at<uchar>(row, i) < MARGINE_WHITE) {
            silu.at<uchar>(row, i) = WHITE;
            return i;
        }
        else {
            silu.at<uchar>(row, i) = BLACK;
        }
    }
    return i;
}
int find_right_border(Mat silu, int row) {
    int i = card.cols-1;
    for (i = card.cols-1;i >= 0;i--) {
        if (card.at<uchar>(row, i) < MARGINE_WHITE) {
            silu.at<uchar>(row, i) = WHITE;
            return i;
        }
        else{
            silu.at<uchar>(row, i) = BLACK;
        }
    }
    //Questo ritorna sempre -1 poiché va in out of bounds per cercare un bordo
    return i;
}

Mat my_Canny() {
    Mat ret;
    ret.create(card.size(), card.type());
    int max_left = card.cols, max_right = -1;
    for (int row = 0;row < card.rows - 1;row++) {
        int left = find_left_border(ret, row);
        int right = find_right_border(ret, row);
        if (max_left >= left && max_left <= max_right) {
            draw_line(ret, row - 1, left, max_left + 1, WHITE);
        }
        if (max_right <= right && max_left <= max_right) {
            draw_line(ret, row - 1, max_right, right + 1, WHITE);
        }
        if (left <= right) {
            if (row != 0 && max_right < max_left) {
                draw_line(ret, row, left + 1, right, WHITE);
            }
            else {
                draw_line(ret, row, left + 1, right, BLACK);
            }
        }
        max_left = left;
        max_right = right;
    }
    return ret;
}

void find_rectangle() {
    Mat silouette=my_Canny();
    vector<vector<Point> > contours;
    findContours(silouette, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    bigger_rect = find_bigger_rectangle(contours);
}

/*void find_rectangle() {
    Canny(card, canny_output, thresh_min, thresh_max);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    bigger_rect = find_bigger_rectangle(contours);
}*/


Mat copy_to_cropped_rectangle() {
    Point starting_point;
    starting_point.x = bigger_rect.center.x - bigger_rect.size.width / 2 + 1;
    starting_point.y = bigger_rect.center.y - bigger_rect.size.height / 2 + 1;
    Mat ret = card(cv::Rect(starting_point.x, starting_point.y, bigger_rect.size.width, bigger_rect.size.height));
    return ret;
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
