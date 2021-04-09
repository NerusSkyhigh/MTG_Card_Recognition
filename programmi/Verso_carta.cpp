#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<cstring>
using namespace std;
using namespace cv;

const char* template_window = "Template Window";
//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/common/2.jpg";
const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/rare/1.jpg";
//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/mythic/124.jpg";




int main(int argc, char *argv[]) {
    Mat img;
    if (argc < 2) {
        img = imread(path_image, IMREAD_COLOR);

    }
    else {
        img = imread(argv[1], IMREAD_COLOR);
    }
    glob(png_dir, fn, true);

    if (fn.size() < 1) {
        cerr << "Can't find png/jpg in foulder" << endl;
        return EXIT_FAILURE;
    }
    if (img.empty()) {
        cerr << "Can't read the image" << endl;
        return EXIT_FAILURE;
    }
    if (SHOWOUTPUT) {
        if (SHOWSTARTINGIMAGE) {
            namedWindow(window_src, WINDOW_KEEPRATIO);
            imshow(window_src, img);
            waitKey(0);
        }
        if (SHOWEXRACTS) {
            namedWindow("DX", WINDOW_AUTOSIZE);
            namedWindow("SX", WINDOW_AUTOSIZE);
        }
    }
    verse_and_set(img);
    return EXIT_SUCCESS;
}














//px differenza (trov-templ)^2
