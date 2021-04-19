#include"rotation.h"



RotatedRect bigger_rect;
Mat canny_output;
Mat card;



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


void find_rectangle() {
    Canny(card, canny_output, thresh_min, thresh_max);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    bigger_rect = find_bigger_rectangle(contours);
}


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
