#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<cstring>
using namespace std;
using namespace cv;

const char *path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
const char *input="Card";
const char *output="output";

#define GUI true
#define SHOWOUTPUT true
#define SHOWINPUT true

#define MARGINE_PERCENTAGE 10

#define MARGINE 256*MARGINE_PERCENTAGE/100

#define X_PERC_CENTER_BOTTOM 0.059523
#define Y_PERC_CENTER_BOTTOM 0.92948

#define Y_BOTTOM_LENGHT_RATEO 1.13898
#define X_BOTTOM_HIGHT_RATEO 23.4

Mat card;
Point points[2];
int hights[2];
void calculate_points_and_hights();


Mat copy_rectangle(Mat &imm, Point &start, int &hights[2]);

Mat get_rotation(Mat img);

int main(int argv, char *argc[]){
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
  Mat card_bottom=get_rotation(img);
  if(GUI){
    if(SHOWINPUT){
      namedWindow(input,WINDOW_KEEPRATIO);
      imshow(img,input);
    }
    if(SHOWOUTPUT){
      namedWindow(output,WINDOW_AUTOSIZE);
      imshow(card_bottom,output);
    }
  }
  return 0;
}




Mat copy_rectangle(Mat &imm, Point &start, int &hights[2]) {
  Mat ret;
  Mat tmp = imm(cv::Rect(start.x, start.y, hights[0],hights[1]));
  tmp.copyTo(ret);
  return ret;
}

void calculate_points_and_hights(Point &points[2]){
  point[0].x=card.rows*X_PERC_CENTER_BOTTOM;
  point[0].y=card.cols*Y_PERC_CENTER_BOTTOM;
  point[1].x=card.rows*(1-X_PERC_CENTER_BOTTOM);
  point[1].y=card.cols*(1-Y_PERC_CENTER_BOTTOM);
  hights[0]=card.rows/X_BOTTOM_HIGHT_RATEO;
  hights[1]=card.cols/Y_BOTTOM_LENGHT_RATEO;
  return;
}

Mat find_bottom(Mat bottom, Mat top){
  int bottom_black_count=0;
  int top_black_count=0;
  for(int y=0;y<bottom.rows;y++){
    for(int x=0;x<bottom.cols;x++){
      if(bottom.at<Point>(x,y)<MARGINE){
        bottom_black_count++;
      }
      if(top.at<Point>(x,y)<MARGINE){
        top_black_count++;
      }
    }
  }
  if(bottom_black_count>top_black_count){
    return bottom;
  }
  else return top;
}

Mat get_rotation(Mat &img){
  card=img;
  calculate_points_and_hights();
  Mat bottom_img=copy_rectangle(card,points[0],hights);
  Mat top_img=copy_rectangle(card,points[0],hights);
  return find_bottom(bottom_img,top_img);
}
