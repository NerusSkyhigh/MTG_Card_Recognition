#include"rotation.h"


//Constants
//It's the bigger rectangle calculated in the image
RotatedRect bigger_rect;
//Canny_output is the resulting variable from the canny algorithm
Mat canny_output;
//The starting image
Mat card;
//these are variables for calculate the time between the
//two function
double freque;
int64 starte;

void st_timer() {
    freque = getTickFrequency();
    starte = getTickCount();
}

double sp_timer() {
    return double(getTickCount() - starte) * 1000 / freque;
}

//@param first float to switch
//@param second float to switch
//A function that switch variables
void switch_variables(float &first, float &second){
  float tmp = first;
  first = second;
  second = tmp;
}

//@param the image that needs to be rotated
//@return the image rotated
Mat rotate_card(Mat img) {
    card = img;
    control_borders();
    find_rectangle();
    //Control if the rectangle is in the right direction
    if (bigger_rect.size.width > bigger_rect.size.height) {
        //Switch the heights and add 90 degrees to the angle
        switch_variables(bigger_rect.size.width,bigger_rect.size.height);
        bigger_rect.angle += 90;
    }

    //This is the part where warp the image, don't know exactly how
    Mat r = getRotationMatrix2D(bigger_rect.center, bigger_rect.angle, 1.0);
    warpAffine(card, card, r, card.size());

    //Gets only the rectangle where the card is
    Mat copied = copy_to_cropped_rectangle();
    return copied;
}

//Expands the image the border of the card
//for the recognition of the rectangle
void control_borders() {
    //for now the border gets added no matter what
    //in future implementation maybe I will add a control
    int border = 1;
    copyMakeBorder(card, card, border, border,
        border, border, BORDER_CONSTANT, 255);
}


//@param matrix where the line is drawn
//@param the row of the matrix
//@param the starting pixel of the colomn
//@param the ending pixel of the colomn
//@param the color of the line
//This func draw a line in the selected row
void draw_line(Mat silu, int row, int start, int end,int color) {
    for (int i = start;i < end;i++) {
        silu.at<uchar>(row, i) = color;
    }
}

//@param matrix where the external border is drawn
//@param the row of the matrix to control
//@return the left colomn border
//Finds the border in the selected row and makes the what the border is
//white in the selected matrix, what is not a border it makes it black
int find_left_border(Mat silu,int row) {
    int i = 0;
    for ( i = 0;i < card.cols;i++) {
        //using a margine for detecting the white saves me form error correction
        if (card.at<uchar>(row, i) < MARGINE_WHITE) {
            silu.at<uchar>(row, i) = WHITE;
            return i;
        }
        else {
            silu.at<uchar>(row, i) = BLACK;
        }
    }
    //if there is no border in this row
    //this returns always the number of colomns of the card
    return i;
}


//@param matrix where the external border is drawn
//@param the row of the matrix to control
//@return the right colomn border
//Finds the border in the selected row and makes the what the border is
//white in the selected matrix, what is not a border it makes it black
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
    //if there is no border in this row
    //this returns always -1
    return i;
}

//@return a silouette of the card
//This canny is slightly more officient than the canny of
//openCv, but it founds only one contour
Mat my_Canny() {
    //This is the silouette
    Mat ret;
    ret.create(card.size(), card.type());
    //Used for remember the points in the last row
    int max_left = card.cols, max_right = -1;
    for (int row = 0;row < card.rows - 1;row++) {
        int left = find_left_border(ret, row);
        int right = find_right_border(ret, row);
        //For the algorithm to find contours the white points needs to be
        //near each other, so if two points are distant it draws a line
        //in the previous row from the previous point to the point in this row
        if (max_left >= left && max_left <= max_right) {
            draw_line(ret, row - 1, left, max_left + 1, WHITE);
        }
        if (max_right <= right && max_left <= max_right) {
            draw_line(ret, row - 1, max_right+1, right + 1, WHITE);
        }
        if (left <= right) {
            //for the first border found draw a line between the two points
            //if it's not the first border found
            if (row != 0 && max_right < max_left) {
                draw_line(ret, row, left + 1, right, WHITE);
            }
            else {
                draw_line(ret, row, left + 1, right, BLACK);
            }
        }
        //remember the points for the next row
        max_left = left;
        max_right = right;
    }
    return ret;
}

//Find the rectangle of the card
void find_rectangle() {
    //With my_Canny findContours is much more efficient
    Mat silouette=my_Canny();
    vector<vector<Point> > contours;
    findContours(silouette, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    //if it finds more than one rectangle take only the bigger one
    bigger_rect = find_bigger_rectangle(contours);
}

/*void find_rectangle() {
    Canny(card, canny_output, thresh_min, thresh_max);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    bigger_rect = find_bigger_rectangle(contours);
}*/

//@return the rectangle cropped
//from the card copy only the bigger_rect found
Mat copy_to_cropped_rectangle() {
    Point starting_point;
    Mat ret;
    starting_point.x = bigger_rect.center.x - bigger_rect.size.width / 2 + 1;
    starting_point.y = bigger_rect.center.y - bigger_rect.size.height / 2 + 1;
    ret = card(cv::Rect(starting_point.x, starting_point.y, bigger_rect.size.width, bigger_rect.size.height));
    return ret;
}

double margine_error=1/REAL_L_C;
bool isSimilar(float x, float y){
  cout<<"Entra in isSimilar"<<endl;
  cout<<"margine_error="<<margine_error<<endl;
  if(x<y){
    cout<<"x e' piu' piccolo di y, CAMBIO!"<<endl;
    double change=y;
    y=x;
    x=change;
  }
  double rateo= x/y;
  cout<<"rateo:"<<rateo<<endl;
  cout<<"C_RATEO_HL:"<<C_RATEO_HL<<endl;
  if(rateo/C_RATEO_HL>1-margine_error&&rateo/C_RATEO_HL<1+margine_error){
    return true;
  }
  else return false;
}

//@param vector of vector of points of contours
//@return The points that has the bigger rectangle
//Finds the bigger rectangle in the vector of points
RotatedRect find_bigger_rectangle(vector<vector<Point>> contours) {
    RotatedRect minRect = minAreaRect(contours[0]);
    for (int i = 1;i < contours.size();i++) {
        RotatedRect rect_cont = minAreaRect(contours[i]);
        if(minRect.center.x<0||minRect.center.y<0)
            minRect=rect_cont;
        else
          if (minRect.size.height + minRect.size.width < rect_cont.size.height + rect_cont.size.width) {
            if(rect_cont.center.x>0&&rect_cont.center.y>0){
              if(isSimilar(rect_cont.size.width,rect_cont.size.height))
              minRect = rect_cont;
            }
          }
    }
    return minRect;
}
