#include"cropping.h"


//Constants
Mat card2;        //input card, it's called card2 because declaration in other files
Point points[2];  //points of the bottom black rectangle
int hights[2];    //heights of the bottom black rectangle

//Crops the card
//@param image of the card (needs to be the right proportions)
//@return image of the bottom black rectangle
Mat get_cropping(Mat& img) {
    //saving the reference in the general constant
    card2 = img;

    //getting the points of where the bottom black rectangle
    //could be
    calculate_points_and_hights();
    //copying the probable bottom black rectangles form the image
    Mat bottom_img = copy_rectangle(card2, points[0], hights);
    Mat top_img = copy_rectangle(card2, points[1], hights);

    //find the bottom black rectangle and returns it
    return find_bottom(bottom_img, top_img);
}

//calculate the points where the bottom black rectangle could be
//and their lenghts
void calculate_points_and_hights() {
    //divades the heights of the image for the rateo to get the
    //lenght (y) and hight (x)
    hights[0] = card2.cols / Y_BOTTOM_LENGHT_RATEO;
    hights[1] = card2.rows / X_BOTTOM_HIGHT_RATEO;

    //calculate the point of the top corner of the bottom rectangle
    points[0].x = card2.cols * X_PERC_CENTER_BOTTOM;
    points[0].y = card2.rows * Y_PERC_CENTER_BOTTOM;

    //calculate the point of the top point of the top rectangle
    points[1].x = card2.cols * (X_PERC_CENTER_BOTTOM);
    points[1].y = card2.rows * (1 - Y_PERC_CENTER_BOTTOM) - hights[1];
}

//@param image
//@param starting point
//@param hights
//@return image copied
//Copies the image in the rectangle pointed
Mat copy_rectangle(Mat& imm, Point& start, int hight[2]) {
    Mat ret;
    //For reference: takes the rectangle and gives if a submatrix of the card
    Mat tmp = imm(cv::Rect(start.x, start.y, hight[0], hight[1]));
    //Copy all the pixels in a new matrix different from the image
    //in a new matrix different from the card2 matrix
    tmp.copyTo(ret);
    return ret;
}


//@param bottom image
//@param top image
//@return the bottom black rectangle
//finds whitch direction is the card and returns the
//right bottom black rectangle flipped if needed
Mat find_bottom(Mat bottom, Mat top) {
    //This algorithm (hate to call this an algorithm) counts
    //the black pixels in the two matrix
    //NOTE: THIS METODE WORKS ONLY FOR CARDS PRINTED AFTER 2015
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
