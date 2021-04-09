#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main (int argc, char* argv[]){
  if(argc<2){
    cout<<"Non abbastanza argomenti, selezionare immagine.\n";
    return -1;
  }
  char *pathfile=argv[1];
  Mat src=imread(pathfile,IMREAD_COLOR);
  if(src.empty()){
    cerr<<"This bitch empty! YEEET\n";
    return EXIT_FAILURE;
  }
  namedWindow("Input", WINDOW_AUTOSIZE);
  namedWindow("Output", WINDOW_AUTOSIZE);

  imshow("Input",src);
  Mat test_clone;
      test_clone.create(src.size(), src.type());
  int nChannels=src.channels();
  int srcnrows= src.rows;
  int srcncol=src.cols;

  for(int i=0;i<(srcnrows);i++){
    uchar *psrcrow=src.ptr<uchar>(i);
    uchar *pclonerow=test_clone.ptr<uchar>(srcnrows-i-1);
    for(int j=0;j<srcncol;j++){
      for(int k=0;k<nChannels;k++){
        *pclonerow++ = (psrcrow[srcncol-j+k-1]);
      }
    }
  }

  imshow("Output",test_clone);
  waitKey();

  return 0;
}
