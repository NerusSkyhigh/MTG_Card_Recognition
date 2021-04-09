#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat src_gray;
int thresh = 100;
RNG rng(12345);



void modify_colors(int, void*);



int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Non abbastanza argomenti, selezionare immagine.\n";
        return -1;
    }
    char* pathfile = argv[1];
    Mat src = imread(pathfile, IMREAD_COLOR);
    if (src.empty()) {
        cerr << "This bitch empty! YEEET\n";
        return EXIT_FAILURE;
    }

    cvtColor(src, src_gray, COLOR_BGR2GRAY);


    namedWindow("Output", WINDOW_AUTOSIZE);
    const char* source_window = "Input";
    namedWindow(source_window);
    imshow(source_window, src);
    const int max_thresh = 255;
    createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, modify_colors);
    modify_colors(0, 0);



    imshow("Input", src);
    imshow("Output", src_gray);
    waitKey();

    return 0;
}


void modify_colors(int, void*){
  for (int y = 0; y < image.rows; y++) {
          for (int x = 0; x < image.cols; x++) {
              for (int c = 0; c < image.channels(); c++) {
                  new_image.at<Vec3b>(y, x)[c] =
                      saturate_cast<uchar>(alpha * image.at<Vec3b>(y, x)[c] + beta);
              }
          }
      }
      
}
